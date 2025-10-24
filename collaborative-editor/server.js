const express = require('express');
const http = require('http');
const WebSocket = require('ws');
const fs = require('fs').promises;
const path = require('path');
const crypto = require('crypto');

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

// Middleware
app.use(express.json());

// Active sessions (in-memory store - for production use Redis/database)
const sessions = new Map(); // sessionId -> { createdAt, expiresAt }

// Configuration with secure defaults
const CONFIG = {
    // Server binds to localhost by default for security
    HOST: process.env.COLLAB_HOST || '127.0.0.1',
    PORT: process.env.COLLAB_PORT || 3000,
    
    // Target file path - must be explicitly configured
    // SECURITY: Only absolute paths are allowed (no relative paths, no ~ expansion)
    TARGET_FILE: process.env.COLLAB_TARGET_FILE || path.join(__dirname, 'data', 'hello.c'),
    
    // Require explicit TWO-STEP opt-in to write to repository files
    // Both environment variables must be set to 'true' (case-insensitive) for repository writes
    ALLOW_REPO_WRITE: (process.env.COLLAB_ALLOW_REPO_WRITE || '').toLowerCase() === 'true',
    CONFIRM_REPO_WRITE: (process.env.COLLAB_CONFIRM_REPO_WRITE || '').toLowerCase() === 'true',
    
    // Security limits
    MAX_MESSAGE_SIZE: parseInt(process.env.COLLAB_MAX_MESSAGE_SIZE || '1048576', 10), // 1MB
    MAX_FILE_SIZE: parseInt(process.env.COLLAB_MAX_FILE_SIZE || '1048576', 10), // 1MB
    RATE_LIMIT_WINDOW: 60000, // 1 minute
    RATE_LIMIT_MAX_EDITS: parseInt(process.env.COLLAB_RATE_LIMIT || '30', 10), // 30 edits per minute
    MAX_USERNAME_LENGTH: 50,
    
    // Optional authentication token
    // SECURITY: Store as string for now, will convert to Buffer during validation
    AUTH_TOKEN: process.env.COLLAB_AUTH_TOKEN || null,
    
    // This will be set during validation to a Buffer for constant-time comparison
    AUTH_TOKEN_BUFFER: null,
    
    // Logging
    LOG_LEVEL: process.env.COLLAB_LOG_LEVEL || 'info' // 'error', 'warn', 'info', 'debug'
};

// Validate configuration on startup
async function validateConfig() {
    // SECURITY: Validate target path
    const providedPath = CONFIG.TARGET_FILE;
    
    // Ensure target path is absolute (no relative paths like ../, no ~ expansion)
    if (!path.isAbsolute(providedPath)) {
        logger.error('SECURITY: COLLAB_TARGET_FILE must be an absolute path');
        logger.error(`Provided: ${providedPath}`);
        logger.error('Relative paths (../, ./, ~) are not allowed for security reasons');
        process.exit(1);
    }
    
    // Check for suspicious path patterns before normalization
    if (providedPath.includes('~') || providedPath.includes('${')) {
        logger.error('SECURITY: COLLAB_TARGET_FILE contains shell expansions');
        logger.error(`Provided: ${providedPath}`);
        logger.error('Shell expansions (~, $) are not allowed for security reasons');
        process.exit(1);
    }
    
    // Normalize the path and check for path traversal
    const normalizedPath = path.normalize(providedPath);
    const resolvedPath = path.resolve(normalizedPath);
    
    // Check for path traversal by looking for '..' segments
    // This is more lenient than requiring exact normalization match (allows duplicate slashes, etc.)
    const pathSegments = normalizedPath.split(path.sep);
    if (pathSegments.includes('..')) {
        logger.error('SECURITY: COLLAB_TARGET_FILE contains path traversal patterns (..)');
        logger.error(`Provided: ${providedPath}`);
        logger.error(`Normalized: ${normalizedPath}`);
        logger.error('Path traversal (parent directory references) are not allowed');
        process.exit(1);
    }
    
    // Get real paths to avoid symlink bypass
    const repoPath = path.resolve(__dirname, '..');
    let targetRealPath;
    let repoRealPath;
    
    try {
        repoRealPath = await fs.realpath(repoPath);
    } catch (error) {
        logger.error(`Failed to resolve repository path: ${error.message}`);
        process.exit(1);
    }
    
    // Target file may not exist yet, so get realpath of parent directory
    const targetDir = path.dirname(resolvedPath);
    try {
        const targetDirRealPath = await fs.realpath(targetDir);
        const targetBasename = path.basename(resolvedPath);
        targetRealPath = path.join(targetDirRealPath, targetBasename);
    } catch (error) {
        // Directory doesn't exist - use the resolved path
        logger.warn(`Target directory does not exist, using resolved path: ${resolvedPath}`);
        targetRealPath = resolvedPath;
    }
    
    // Check if target file is inside repository (using real paths to avoid symlink bypass)
    // Use path.relative for canonical containment check (safer than startsWith)
    const relativePath = path.relative(repoRealPath, targetRealPath);
    const isInRepo = !!relativePath && !relativePath.startsWith('..') && !path.isAbsolute(relativePath);
    
    if (isInRepo) {
        // SECURITY: Two-step opt-in required for repository writes
        if (!CONFIG.ALLOW_REPO_WRITE || !CONFIG.CONFIRM_REPO_WRITE) {
            logger.error('');
            logger.error('='.repeat(70));
            logger.error('SECURITY: Target file is inside repository');
            logger.error('='.repeat(70));
            logger.error(`Target: ${targetRealPath}`);
            logger.error(`Repo: ${repoRealPath}`);
            logger.error('');
            logger.error('Writing to repository files requires EXPLICIT TWO-STEP OPT-IN:');
            logger.error('  export COLLAB_ALLOW_REPO_WRITE=true');
            logger.error('  export COLLAB_CONFIRM_REPO_WRITE=true');
            logger.error('');
            logger.error('Both environment variables must be set to "true" (case-insensitive).');
            logger.error('This protection prevents accidental repository modifications.');
            logger.error('='.repeat(70));
            logger.error('');
            process.exit(1);
        }
        
        logger.warn('');
        logger.warn('⚠️  WARNING: Repository write mode is ENABLED');
        logger.warn('   This allows modifications to files inside the repository!');
        logger.warn('   Target: ' + targetRealPath);
        logger.warn('');
    }
    
    if (CONFIG.AUTH_TOKEN) {
        logger.info('Authentication enabled');
        // SECURITY: Never log the actual token value
        logger.debug('Token length: ' + CONFIG.AUTH_TOKEN.length + ' characters');
        
        // SECURITY: Pre-convert token to Buffer with explicit UTF-8 encoding
        // This ensures consistent encoding in comparison and avoids repeated conversions
        CONFIG.AUTH_TOKEN_BUFFER = Buffer.from(CONFIG.AUTH_TOKEN, 'utf8');
    } else {
        logger.warn('WARNING: No authentication configured. Set COLLAB_AUTH_TOKEN for production use.');
    }
    
    logger.info(`Server will bind to ${CONFIG.HOST}:${CONFIG.PORT}`);
    logger.info(`Target file: ${targetRealPath}`);
    logger.info(`Repository write: ${(CONFIG.ALLOW_REPO_WRITE && CONFIG.CONFIRM_REPO_WRITE) ? 'ENABLED (BOTH CONFIRMATIONS SET)' : 'DISABLED'}`);
}

// Simple structured logger
const LOG_LEVELS = { error: 0, warn: 1, info: 2, debug: 3 };
const logger = {
    _log(level, ...args) {
        if (LOG_LEVELS[level] <= LOG_LEVELS[CONFIG.LOG_LEVEL]) {
            const timestamp = new Date().toISOString();
            console.log(`[${timestamp}] [${level.toUpperCase()}]`, ...args);
        }
    },
    error: (...args) => logger._log('error', ...args),
    warn: (...args) => logger._log('warn', ...args),
    info: (...args) => logger._log('info', ...args),
    debug: (...args) => logger._log('debug', ...args)
};

// Store connected clients and their metadata
const clients = new Map();
// Track rate limiting per client
const rateLimits = new Map();
let fileContent = '';
let version = 0;

// Rate limiting helper
function checkRateLimit(clientId) {
    const now = Date.now();
    const limit = rateLimits.get(clientId) || { count: 0, windowStart: now };
    
    // Reset window if expired
    if (now - limit.windowStart > CONFIG.RATE_LIMIT_WINDOW) {
        limit.count = 0;
        limit.windowStart = now;
    }
    
    limit.count++;
    rateLimits.set(clientId, limit);
    
    if (limit.count > CONFIG.RATE_LIMIT_MAX_EDITS) {
        logger.warn(`Rate limit exceeded for client ${clientId}: ${limit.count} edits in window`);
        return false;
    }
    
    return true;
}

// Sanitize username to prevent XSS
function sanitizeUsername(name) {
    if (!name || typeof name !== 'string') {
        return 'Anonymous';
    }
    
    // Limit length
    const trimmed = name.trim().substring(0, CONFIG.MAX_USERNAME_LENGTH);
    
    // Remove any HTML tags and special characters that could be used for XSS
    // Allow only alphanumeric, spaces, dashes, underscores
    const sanitized = trimmed.replace(/[^a-zA-Z0-9\s\-_]/g, '');
    
    return sanitized || 'Anonymous';
}

// Validate message structure and size
function validateMessage(message, maxSize = CONFIG.MAX_MESSAGE_SIZE) {
    if (!message) {
        return { valid: false, error: 'Empty message' };
    }
    
    // Check message size (Buffer.byteLength handles UTF-8 correctly)
    const size = Buffer.byteLength(message, 'utf8');
    if (size > maxSize) {
        return { valid: false, error: `Message too large: ${size} bytes (max: ${maxSize})` };
    }
    
    return { valid: true };
}

// Load initial file content
async function loadFile() {
    try {
        // Ensure directory exists
        const dir = path.dirname(CONFIG.TARGET_FILE);
        await fs.mkdir(dir, { recursive: true });
        
        try {
            fileContent = await fs.readFile(CONFIG.TARGET_FILE, 'utf8');
            
            // Check file size
            if (Buffer.byteLength(fileContent, 'utf8') > CONFIG.MAX_FILE_SIZE) {
                throw new Error(`File too large (max: ${CONFIG.MAX_FILE_SIZE} bytes)`);
            }
            
            logger.info(`Loaded file successfully: ${CONFIG.TARGET_FILE}`);
        } catch (error) {
            if (error.code === 'ENOENT') {
                // File doesn't exist, create with default content
                fileContent = '# include <stdio.h>\n\nint main(){\n    printf("Hello world!");\n}\n';
                await saveFile(fileContent);
                logger.info(`Created new file: ${CONFIG.TARGET_FILE}`);
            } else {
                throw error;
            }
        }
    } catch (error) {
        logger.error('Error loading file:', error.message);
        throw error;
    }
}

// Save file content with atomic write and symlink protection
// SECURITY: Implements atomic writes using temp file + rename
// This is atomic on POSIX systems (Linux, macOS, BSD)
// On Windows, fs.rename may not be atomic; see documentation for limitations
async function saveFile(content) {
    // Validate content size
    const size = Buffer.byteLength(content, 'utf8');
    if (size > CONFIG.MAX_FILE_SIZE) {
        logger.error(`Content too large: ${size} bytes (max: ${CONFIG.MAX_FILE_SIZE})`);
        return false;
    }
    
    const targetPath = path.resolve(CONFIG.TARGET_FILE);
    const targetDir = path.dirname(targetPath);
    const tempFile = path.join(targetDir, `.collab-tmp-${Date.now()}-${crypto.randomBytes(4).toString('hex')}`);
    
    try {
        // SECURITY: Check if target is a symlink before writing
        // This protects against symlink swap attacks (TOCTOU)
        try {
            const stats = await fs.lstat(targetPath);
            if (stats.isSymbolicLink()) {
                logger.error('SECURITY: Target file is a symlink, refusing to write');
                logger.error(`Target: ${targetPath}`);
                return false;
            }
        } catch (error) {
            // File doesn't exist yet, which is OK (will be created)
            if (error.code !== 'ENOENT') {
                throw error;
            }
        }
        
        // Write to temporary file first (in same directory as target for atomic rename)
        await fs.writeFile(tempFile, content, 'utf8');
        
        // SECURITY: Verify temp file was created and is not a symlink
        const tempStats = await fs.lstat(tempFile);
        if (tempStats.isSymbolicLink()) {
            logger.error('SECURITY: Temp file became a symlink, aborting');
            await fs.unlink(tempFile);
            return false;
        }
        
        // Atomically rename temp file to target
        // NOTE: On POSIX this is atomic. On Windows rename semantics differ.
        // For Windows production use, consider using fs.replace or atomic-write libraries
        await fs.rename(tempFile, targetPath);
        
        // Update in-memory state only after successful write
        fileContent = content;
        version++;
        
        logger.info(`Saved file (version ${version})`);
        return true;
    } catch (error) {
        logger.error('Error saving file:', error.message);
        
        // Clean up temp file if it exists
        try {
            await fs.unlink(tempFile);
        } catch (cleanupError) {
            // Ignore cleanup errors
        }
        
        return false;
    }
}

// Broadcast message to all clients except sender
function broadcast(message, excludeClient = null) {
    try {
        const messageStr = JSON.stringify(message);
        clients.forEach((metadata, client) => {
            if (client !== excludeClient && client.readyState === WebSocket.OPEN) {
                try {
                    client.send(messageStr);
                } catch (error) {
                    logger.error(`Error broadcasting to client ${metadata.id}:`, error.message);
                }
            }
        });
    } catch (error) {
        logger.error('Error in broadcast:', error.message);
    }
}

// Simple cookie parser
function parseCookie(cookieHeader) {
    if (!cookieHeader) return {};
    
    return cookieHeader.split(';').reduce((cookies, cookie) => {
        const [name, value] = cookie.trim().split('=');
        cookies[name] = value;
        return cookies;
    }, {});
}

// Verify authentication token if configured
// SECURITY: Tokens must be sent via Authorization: Bearer header for initial auth
// WebSocket connections use session cookies (not URL parameters)
function verifyAuth(request) {
    if (!CONFIG.AUTH_TOKEN) {
        return true; // No auth required
    }
    
    // For WebSocket upgrade requests, check session cookie
    const cookies = parseCookie(request.headers.cookie);
    const sessionId = cookies.collab_session;
    
    if (!sessionId) {
        return false;
    }
    
    // Verify session exists and is not expired
    const session = sessions.get(sessionId);
    if (!session) {
        return false;
    }
    
    if (Date.now() > session.expiresAt) {
        sessions.delete(sessionId);
        return false;
    }
    
    return true;
}

// Handle WebSocket connections
wss.on('connection', (ws, request) => {
    // Verify authentication if required
    if (!verifyAuth(request)) {
        logger.warn('Unauthorized connection attempt');
        ws.close(1008, 'Authentication required');
        return;
    }
    
    const clientId = crypto.randomBytes(8).toString('hex');
    const clientInfo = {
        id: clientId,
        name: sanitizeUsername(`User-${clientId.substr(0, 4)}`),
        connectedAt: Date.now()
    };
    clients.set(ws, clientInfo);
    
    logger.info(`Client ${clientId} connected from ${request.socket.remoteAddress}. Total clients: ${clients.size}`);
    
    // Send initial state to new client
    try {
        ws.send(JSON.stringify({
            type: 'init',
            content: fileContent,
            version: version,
            clientId: clientId,
            clients: Array.from(clients.values()).map(c => ({ id: c.id, name: c.name }))
        }));
    } catch (error) {
        logger.error(`Error sending init to client ${clientId}:`, error.message);
        ws.close();
        return;
    }
    
    // Notify other clients about new user
    broadcast({
        type: 'user-joined',
        clientId: clientId,
        name: clientInfo.name
    }, ws);
    
    // Handle messages from client
    ws.on('message', async (message) => {
        try {
            // Validate message size before parsing
            const validation = validateMessage(message);
            if (!validation.valid) {
                logger.warn(`Invalid message from client ${clientId}: ${validation.error}`);
                ws.send(JSON.stringify({
                    type: 'error',
                    message: 'Message rejected: ' + validation.error
                }));
                return;
            }
            
            // Parse JSON safely
            let data;
            try {
                data = JSON.parse(message);
            } catch (parseError) {
                logger.warn(`JSON parse error from client ${clientId}:`, parseError.message);
                ws.send(JSON.stringify({
                    type: 'error',
                    message: 'Invalid message format'
                }));
                return;
            }
            
            // Validate data type
            if (!data || typeof data !== 'object' || !data.type) {
                ws.send(JSON.stringify({
                    type: 'error',
                    message: 'Invalid message structure'
                }));
                return;
            }
            
            switch (data.type) {
                case 'edit':
                    // Check rate limit
                    if (!checkRateLimit(clientId)) {
                        ws.send(JSON.stringify({
                            type: 'error',
                            message: 'Rate limit exceeded. Please slow down.'
                        }));
                        return;
                    }
                    
                    // Validate content
                    if (typeof data.content !== 'string') {
                        ws.send(JSON.stringify({
                            type: 'error',
                            message: 'Invalid content type'
                        }));
                        return;
                    }
                    
                    // Apply edit with simple conflict resolution (last write wins)
                    const success = await saveFile(data.content);
                    if (success) {
                        // Broadcast to all other clients
                        broadcast({
                            type: 'update',
                            content: data.content,
                            version: version,
                            clientId: clientId
                        }, ws);
                        
                        // Acknowledge to sender
                        ws.send(JSON.stringify({
                            type: 'ack',
                            version: version
                        }));
                        
                        logger.debug(`Client ${clientId} saved edit (version ${version})`);
                    } else {
                        ws.send(JSON.stringify({
                            type: 'error',
                            message: 'Failed to save file'
                        }));
                    }
                    break;
                    
                case 'cursor':
                    // Validate cursor position
                    if (typeof data.position === 'number' && data.position >= 0) {
                        // Broadcast cursor position to other clients
                        broadcast({
                            type: 'cursor',
                            clientId: clientId,
                            position: data.position
                        }, ws);
                    }
                    break;
                    
                case 'set-name':
                    // Validate and sanitize new name
                    if (typeof data.name === 'string') {
                        const oldName = clientInfo.name;
                        const newName = sanitizeUsername(data.name);
                        clientInfo.name = newName;
                        
                        broadcast({
                            type: 'user-renamed',
                            clientId: clientId,
                            oldName: oldName,
                            newName: newName
                        }, ws);
                        
                        logger.debug(`Client ${clientId} renamed from "${oldName}" to "${newName}"`);
                    }
                    break;
                    
                default:
                    logger.warn(`Unknown message type from client ${clientId}: ${data.type}`);
                    ws.send(JSON.stringify({
                        type: 'error',
                        message: 'Unknown message type'
                    }));
            }
        } catch (error) {
            logger.error(`Error handling message from client ${clientId}:`, error.message);
            try {
                ws.send(JSON.stringify({
                    type: 'error',
                    message: 'Internal server error'
                }));
            } catch (sendError) {
                logger.error(`Failed to send error message:`, sendError.message);
            }
        }
    });
    
    // Handle client disconnect
    ws.on('close', () => {
        const metadata = clients.get(ws);
        clients.delete(ws);
        rateLimits.delete(clientId);
        
        logger.info(`Client ${clientId} disconnected. Total clients: ${clients.size}`);
        
        // Notify other clients
        if (metadata) {
            broadcast({
                type: 'user-left',
                clientId: clientId,
                name: metadata.name
            });
        }
    });
    
    // Handle errors
    ws.on('error', (error) => {
        logger.error(`WebSocket error for client ${clientId}:`, error.message);
    });
});

// Add security headers
app.use((req, res, next) => {
    // Content Security Policy to prevent XSS
    res.setHeader('Content-Security-Policy', 
        "default-src 'self'; " +
        "script-src 'self' 'unsafe-inline'; " +
        "style-src 'self' 'unsafe-inline'; " +
        "connect-src 'self' ws: wss:; " +
        "img-src 'self' data:; " +
        "font-src 'self'; " +
        "frame-ancestors 'none'"
    );
    
    // Other security headers
    res.setHeader('X-Content-Type-Options', 'nosniff');
    res.setHeader('X-Frame-Options', 'DENY');
    res.setHeader('X-XSS-Protection', '1; mode=block');
    
    next();
});

// Authentication endpoint - must use Authorization: Bearer header
// SECURITY: Tokens are never passed via URL to prevent leaks
// SECURITY: Uses constant-time comparison to prevent timing attacks
app.post('/api/auth', (req, res) => {
    if (!CONFIG.AUTH_TOKEN) {
        // No authentication required
        return res.json({ authenticated: true, message: 'No authentication required' });
    }
    
    // Extract token from Authorization: Bearer header
    const authHeader = req.headers['authorization'];
    if (!authHeader) {
        logger.warn('Authentication attempt without Authorization header');
        return res.status(401).json({ error: 'Missing Authorization header' });
    }
    
    const parts = authHeader.split(' ');
    if (parts.length !== 2 || parts[0] !== 'Bearer') {
        logger.warn('Authentication attempt with invalid Authorization header format');
        return res.status(401).json({ error: 'Invalid Authorization header format. Expected: Bearer <token>' });
    }
    
    const providedToken = parts[1];
    
    // Validate token is not empty
    if (!providedToken || providedToken.length === 0) {
        logger.warn('Authentication attempt with empty token');
        return res.status(401).json({ error: 'Invalid token' });
    }
    
    // SECURITY: Constant-time comparison to prevent timing attacks
    // Pre-convert provided token to Buffer with explicit UTF-8 encoding
    const providedBuffer = Buffer.from(providedToken, 'utf8');
    const expectedBuffer = CONFIG.AUTH_TOKEN_BUFFER;
    
    // SECURITY: Check buffer lengths match (fail early but safely)
    // This prevents timingSafeEqual from throwing on different-length buffers
    // Comparing Buffer lengths reveals no timing information about the token content
    if (providedBuffer.length !== expectedBuffer.length) {
        logger.warn('Authentication failed: invalid token length');
        return res.status(401).json({ error: 'Invalid token' });
    }
    
    let isValid = false;
    try {
        // SECURITY: Use constant-time comparison
        // Both buffers have same length, so timingSafeEqual is safe to call
        isValid = crypto.timingSafeEqual(providedBuffer, expectedBuffer);
    } catch (error) {
        // Should not happen due to length check above, but handle gracefully
        logger.error('Authentication error during token comparison:', error.message);
        // SECURITY: Never log the actual tokens
        return res.status(401).json({ error: 'Invalid token' });
    }
    
    if (!isValid) {
        logger.warn('Authentication failed: token mismatch');
        return res.status(401).json({ error: 'Invalid token' });
    }
    
    // Create session
    const sessionId = crypto.randomBytes(32).toString('hex');
    const expiresAt = Date.now() + (24 * 60 * 60 * 1000); // 24 hours
    
    sessions.set(sessionId, {
        createdAt: Date.now(),
        expiresAt: expiresAt
    });
    
    // Set secure, httpOnly cookie
    res.cookie('collab_session', sessionId, {
        httpOnly: true,
        secure: req.secure || req.headers['x-forwarded-proto'] === 'https',
        sameSite: 'strict',
        maxAge: 24 * 60 * 60 * 1000 // 24 hours
    });
    
    logger.info('Authentication successful, session created');
    // SECURITY: Never log the token or session ID
    res.json({ authenticated: true, expiresAt: expiresAt });
});

// Logout endpoint - invalidates session server-side
app.post('/api/logout', (req, res) => {
    const sessionId = parseCookie(req.headers.cookie)?.collab_session;
    if (sessionId) {
        // SECURITY: Delete session from server-side store
        const deleted = sessions.delete(sessionId);
        if (deleted) {
            logger.info('Session invalidated on logout');
        }
    }
    
    // Clear cookie on client
    res.clearCookie('collab_session');
    res.json({ message: 'Logged out' });
});

// Serve static files
app.use(express.static(path.join(__dirname, 'public')));

// API endpoint to get current file content
app.get('/api/content', async (req, res) => {
    try {
        res.json({
            content: fileContent,
            version: version
        });
    } catch (error) {
        logger.error('Error in /api/content:', error.message);
        res.status(500).json({ error: 'Internal server error' });
    }
});

// Health check endpoint
app.get('/api/health', (req, res) => {
    res.json({
        status: 'ok',
        clients: clients.size,
        version: version,
        uptime: process.uptime()
    });
});

// Start server
async function start() {
    try {
        // Validate configuration
        await validateConfig();
        
        // Load initial file
        await loadFile();
        
        // Bind to configured host (localhost by default for security)
        server.listen(CONFIG.PORT, CONFIG.HOST, () => {
            logger.info('='.repeat(60));
            logger.info('Collaborative Editor Server Started');
            logger.info('='.repeat(60));
            logger.info(`Server URL: http://${CONFIG.HOST}:${CONFIG.PORT}`);
            logger.info(`WebSocket: ws://${CONFIG.HOST}:${CONFIG.PORT}`);
            logger.info(`Target file: ${CONFIG.TARGET_FILE}`);
            logger.info(`Authentication: ${CONFIG.AUTH_TOKEN ? 'ENABLED' : 'DISABLED (WARNING)'}`);
            logger.info(`Repository write: ${CONFIG.ALLOW_REPO_WRITE ? 'ENABLED' : 'DISABLED'}`);
            logger.info('='.repeat(60));
            
            if (!CONFIG.AUTH_TOKEN) {
                logger.warn('');
                logger.warn('⚠️  SECURITY WARNING: No authentication configured!');
                logger.warn('   Set COLLAB_AUTH_TOKEN environment variable for production use.');
                logger.warn('');
            }
            
            if (CONFIG.HOST === '0.0.0.0') {
                logger.warn('');
                logger.warn('⚠️  SECURITY WARNING: Server is bound to all interfaces (0.0.0.0)!');
                logger.warn('   This allows connections from any network interface.');
                logger.warn('   Use 127.0.0.1 for localhost-only access.');
                logger.warn('');
            }
        });
    } catch (error) {
        logger.error('Failed to start server:', error.message);
        process.exit(1);
    }
}

start();
