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
    TARGET_FILE: process.env.COLLAB_TARGET_FILE || path.join(__dirname, 'data', 'hello.c'),
    
    // Require explicit opt-in to write to repository files
    ALLOW_REPO_WRITE: process.env.COLLAB_ALLOW_REPO_WRITE === 'true',
    
    // Security limits
    MAX_MESSAGE_SIZE: parseInt(process.env.COLLAB_MAX_MESSAGE_SIZE || '1048576', 10), // 1MB
    MAX_FILE_SIZE: parseInt(process.env.COLLAB_MAX_FILE_SIZE || '1048576', 10), // 1MB
    RATE_LIMIT_WINDOW: 60000, // 1 minute
    RATE_LIMIT_MAX_EDITS: parseInt(process.env.COLLAB_RATE_LIMIT || '30', 10), // 30 edits per minute
    MAX_USERNAME_LENGTH: 50,
    
    // Optional authentication token
    AUTH_TOKEN: process.env.COLLAB_AUTH_TOKEN || null,
    
    // Logging
    LOG_LEVEL: process.env.COLLAB_LOG_LEVEL || 'info' // 'error', 'warn', 'info', 'debug'
};

// Validate configuration on startup
function validateConfig() {
    const targetPath = path.resolve(CONFIG.TARGET_FILE);
    const repoPath = path.resolve(__dirname, '..');
    
    // Check if target file is inside repository
    const isInRepo = targetPath.startsWith(repoPath);
    
    if (isInRepo && !CONFIG.ALLOW_REPO_WRITE) {
        logger.error('SECURITY: Target file is inside repository but COLLAB_ALLOW_REPO_WRITE is not set to "true"');
        logger.error(`Target: ${targetPath}`);
        logger.error(`Repo: ${repoPath}`);
        logger.error('To enable writing to repository files, set environment variable:');
        logger.error('  export COLLAB_ALLOW_REPO_WRITE=true');
        process.exit(1);
    }
    
    if (CONFIG.AUTH_TOKEN) {
        logger.info('Authentication enabled');
    } else {
        logger.warn('WARNING: No authentication configured. Set COLLAB_AUTH_TOKEN for production use.');
    }
    
    logger.info(`Server will bind to ${CONFIG.HOST}:${CONFIG.PORT}`);
    logger.info(`Target file: ${targetPath}`);
    logger.info(`Repository write: ${CONFIG.ALLOW_REPO_WRITE ? 'ENABLED' : 'DISABLED'}`);
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

// Save file content with atomic write (temp file + rename)
async function saveFile(content) {
    // Validate content size
    const size = Buffer.byteLength(content, 'utf8');
    if (size > CONFIG.MAX_FILE_SIZE) {
        logger.error(`Content too large: ${size} bytes (max: ${CONFIG.MAX_FILE_SIZE})`);
        return false;
    }
    
    const tempFile = `${CONFIG.TARGET_FILE}.tmp.${Date.now()}.${crypto.randomBytes(4).toString('hex')}`;
    
    try {
        // Write to temporary file first
        await fs.writeFile(tempFile, content, 'utf8');
        
        // Atomically rename temp file to target (this is atomic on POSIX systems)
        await fs.rename(tempFile, CONFIG.TARGET_FILE);
        
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
app.post('/api/auth', (req, res) => {
    if (!CONFIG.AUTH_TOKEN) {
        // No authentication required
        return res.json({ authenticated: true, message: 'No authentication required' });
    }
    
    // Extract token from Authorization: Bearer header
    const authHeader = req.headers['authorization'];
    if (!authHeader) {
        return res.status(401).json({ error: 'Missing Authorization header' });
    }
    
    const parts = authHeader.split(' ');
    if (parts.length !== 2 || parts[0] !== 'Bearer') {
        return res.status(401).json({ error: 'Invalid Authorization header format. Expected: Bearer <token>' });
    }
    
    const token = parts[1];
    if (token !== CONFIG.AUTH_TOKEN) {
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
    res.json({ authenticated: true, expiresAt: expiresAt });
});

// Logout endpoint
app.post('/api/logout', (req, res) => {
    const sessionId = parseCookie(req.headers.cookie)?.collab_session;
    if (sessionId) {
        sessions.delete(sessionId);
        logger.info('Session logged out');
    }
    
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
        validateConfig();
        
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
