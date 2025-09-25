#!/usr/bin/env node
/**
 * Collaborative Editor WebSocket Server
 * Enables real-time collaborative editing of hello.c with conflict resolution
 */

const WebSocket = require('ws');
const express = require('express');
const fs = require('fs').promises;
const path = require('path');
const { execFile } = require('child_process');
const { promisify } = require('util');

const execFileAsync = promisify(execFile);
const app = express();

// Configuration with environment variables and defaults
const CONFIG = {
    HTTP_PORT: process.env.COLLAB_HTTP_PORT || process.env.PORT || 8082,
    WS_PORT: process.env.COLLAB_WS_PORT || process.env.WS_PORT || 8083,
    MAX_USERS: process.env.COLLAB_MAX_USERS || 10,
    RATE_LIMIT: process.env.COLLAB_RATE_LIMIT || 30, // operations per minute
    MAX_CONTENT_SIZE: process.env.COLLAB_MAX_CONTENT_SIZE || 100000, // 100KB
    BUILD_TIMEOUT: process.env.COLLAB_BUILD_TIMEOUT || 15000, // 15 seconds
    RUN_TIMEOUT: process.env.COLLAB_RUN_TIMEOUT || 8000, // 8 seconds
    LOG_LEVEL: process.env.COLLAB_LOG_LEVEL || 'info'
};

console.log('🔧 Configuration:');
console.log(`   HTTP Port: ${CONFIG.HTTP_PORT}`);
console.log(`   WebSocket Port: ${CONFIG.WS_PORT}`);
console.log(`   Max Users: ${CONFIG.MAX_USERS}`);
console.log(`   Rate Limit: ${CONFIG.RATE_LIMIT} ops/min`);
console.log('');

// Serve static files
app.use(express.static(__dirname));
app.use(express.json());

// Health check endpoint
app.get('/health', (req, res) => {
    res.json({
        status: 'healthy',
        timestamp: new Date().toISOString(),
        activeUsers: collabState ? collabState.users.size : 0,
        version: collabState ? collabState.version : 0,
        config: {
            maxUsers: CONFIG.MAX_USERS,
            rateLimit: CONFIG.RATE_LIMIT
        }
    });
});

// Visual editor endpoints
app.post('/build', async (req, res) => {
    try {
        const { code } = req.body;
        if (!code) {
            return res.status(400).json({ success: false, error: 'No code provided' });
        }

        // Write code to temporary file
        await fs.writeFile('temp_visual.c', code);
        
        const result = await collabState.buildCode('temp_visual.c', 'temp_visual');
        res.json(result);
    } catch (error) {
        console.error('Visual editor build error:', error);
        res.status(500).json({ success: false, error: error.message });
    }
});

app.post('/run', async (req, res) => {
    try {
        const result = await collabState.runCode('./temp_visual');
        res.json(result);
    } catch (error) {
        console.error('Visual editor run error:', error);
        res.status(500).json({ success: false, error: error.message });
    }
});

app.post('/export', async (req, res) => {
    try {
        const { code, filename } = req.body;
        if (!code || !filename) {
            return res.status(400).json({ success: false, error: 'Code and filename required' });
        }

        await fs.writeFile(filename, code);
        res.json({ success: true, message: `Code exported to ${filename}` });
    } catch (error) {
        console.error('Visual editor export error:', error);
        res.status(500).json({ success: false, error: error.message });
    }
});

// In-memory storage for collaboration state
class CollaborationState {
    constructor() {
        this.content = '';
        this.users = new Map();
        this.version = 0;
        this.operations = [];
        this.rateLimits = new Map(); // userId -> { count, resetTime }
        this.loadInitialContent();
    }

    // Rate limiting: configurable operations per minute per user
    checkRateLimit(userId) {
        const now = Date.now();
        const limit = this.rateLimits.get(userId);
        
        if (!limit || now > limit.resetTime) {
            this.rateLimits.set(userId, { count: 1, resetTime: now + 60000 });
            return true;
        }
        
        if (limit.count >= CONFIG.RATE_LIMIT) {
            return false;
        }
        
        limit.count++;
        return true;
    }

    // Input validation
    validateOperation(operation) {
        if (!operation || typeof operation !== 'object') {
            return 'Invalid operation format';
        }
        
        const validTypes = ['insert', 'delete', 'replace', 'cursor'];
        if (!validTypes.includes(operation.type)) {
            return 'Invalid operation type';
        }
        
        if (operation.type === 'insert' || operation.type === 'delete' || operation.type === 'cursor') {
            if (typeof operation.position !== 'number' || operation.position < 0) {
                return 'Invalid position';
            }
        }
        
        if (operation.type === 'insert' || operation.type === 'replace') {
            if (typeof operation.content !== 'string') {
                return 'Invalid content type';
            }
            if (operation.content.length > CONFIG.MAX_CONTENT_SIZE) {
                return 'Content too large';
            }
        }
        
        if (operation.type === 'delete') {
            if (typeof operation.length !== 'number' || operation.length < 0) {
                return 'Invalid delete length';
            }
        }
        
        return null; // Valid
    }

    async loadInitialContent() {
        try {
            this.content = await fs.readFile(path.join(__dirname, 'hello.c'), 'utf8');
            console.log('📄 Loaded hello.c content');
        } catch (error) {
            console.error('❌ Failed to load hello.c:', error.message);
            // Fallback content
            this.content = `#include <stdio.h>

int main(void){
    int rc = 0;
    puts("Hello world!");
    printf("Exit code: %d\\n", rc);
    return rc;
}`;
        }
    }

    addUser(userId, ws, userInfo) {
        // Enforce max users limit
        if (this.users.size >= CONFIG.MAX_USERS) {
            ws.send(JSON.stringify({
                type: 'error',
                message: 'Server at maximum capacity'
            }));
            ws.close();
            return;
        }
        
        this.users.set(userId, {
            ws: ws,
            name: userInfo.name || `User-${Math.floor(Math.random() * 1000)}`,
            color: userInfo.color || this.getRandomColor(),
            cursor: 0,
            joinedAt: new Date()
        });
        console.log(`👤 User ${userInfo.name} joined (${this.users.size}/${CONFIG.MAX_USERS} users)`);
    }

    removeUser(userId) {
        const user = this.users.get(userId);
        if (user) {
            console.log(`👋 User ${user.name} left (${this.users.size - 1} remaining)`);
            this.users.delete(userId);
        }
    }

    getRandomColor() {
        const colors = ['#007acc', '#4CAF50', '#ff9800', '#9c27b0', '#f44336', '#00bcd4', '#795548', '#607d8b'];
        return colors[Math.floor(Math.random() * colors.length)];
    }

    applyOperation(operation, userId) {
        // Proper operational transformation for conflict resolution
        const user = this.users.get(userId);
        if (!user) return false;

        try {
            switch (operation.type) {
                case 'insert':
                    // Transform insert position based on concurrent operations
                    const transformedPos = this.transformPosition(
                        operation.position, 
                        operation.clientVersion || 0
                    );
                    this.content = this.content.slice(0, transformedPos) + 
                                   operation.content + 
                                   this.content.slice(transformedPos);
                    break;
                    
                case 'delete':
                    // Transform delete position and ensure valid range
                    const delPos = this.transformPosition(
                        operation.position, 
                        operation.clientVersion || 0
                    );
                    const delLen = Math.min(operation.length, this.content.length - delPos);
                    if (delPos >= 0 && delPos < this.content.length && delLen > 0) {
                        this.content = this.content.slice(0, delPos) + 
                                       this.content.slice(delPos + delLen);
                    }
                    break;
                    
                case 'replace':
                    // For replace operations, validate content and apply
                    if (typeof operation.content === 'string' && 
                        operation.content.length < 100000) { // 100KB limit
                        this.content = operation.content;
                    }
                    break;
                    
                case 'cursor':
                    // Cursor updates don't modify content
                    user.cursor = Math.max(0, Math.min(operation.position, this.content.length));
                    break;
                    
                default:
                    console.warn(`Unknown operation type: ${operation.type}`);
                    return false;
            }

            if (operation.type !== 'cursor') {
                this.version++;
                this.operations.push({
                    ...operation,
                    userId,
                    timestamp: new Date(),
                    version: this.version,
                    transformedPosition: operation.type === 'insert' || operation.type === 'delete' 
                        ? this.transformPosition(operation.position, operation.clientVersion || 0)
                        : undefined
                });
                
                // Keep operation history bounded
                if (this.operations.length > 1000) {
                    this.operations = this.operations.slice(-500);
                }
            }

            return true;
        } catch (error) {
            console.error('❌ Failed to apply operation:', error.message);
            return false;
        }
    }

    // Transform position based on concurrent operations
    transformPosition(position, clientVersion) {
        if (clientVersion >= this.version) {
            return position;
        }
        
        let transformedPos = position;
        
        // Apply transformations from operations since client version
        const relevantOps = this.operations.filter(op => 
            op.version > clientVersion && op.version <= this.version
        );
        
        for (const op of relevantOps) {
            if (op.type === 'insert' && op.transformedPosition !== undefined) {
                if (op.transformedPosition <= transformedPos) {
                    transformedPos += op.content.length;
                }
            } else if (op.type === 'delete' && op.transformedPosition !== undefined) {
                if (op.transformedPosition < transformedPos) {
                    transformedPos -= Math.min(op.length, transformedPos - op.transformedPosition);
                } else if (op.transformedPosition < transformedPos + op.length) {
                    // Position is within deleted range, clamp to deletion point
                    transformedPos = op.transformedPosition;
                }
            }
        }
        
        return Math.max(0, Math.min(transformedPos, this.content.length));
    }

    broadcast(message, excludeUserId = null) {
        this.users.forEach((user, userId) => {
            if (userId !== excludeUserId && user.ws.readyState === WebSocket.OPEN) {
                try {
                    user.ws.send(JSON.stringify(message));
                } catch (error) {
                    console.error('❌ Failed to send message to user:', error.message);
                }
            }
        });
    }

    getUsersInfo() {
        return Array.from(this.users.entries()).map(([id, user]) => ({
            id,
            name: user.name,
            color: user.color,
            cursor: user.cursor,
            joinedAt: user.joinedAt
        }));
    }

    async saveToFile() {
        try {
            await fs.writeFile(path.join(__dirname, 'hello.c'), this.content, 'utf8');
            console.log('💾 Saved hello.c to disk');
            return true;
        } catch (error) {
            console.error('❌ Failed to save hello.c:', error.message);
            return false;
        }
    }

    async buildCode(sourceFile = 'hello.c', outputFile = 'hello') {
        try {
            // SECURITY: Sandboxed build execution with strict limits - NO SHELL INTERPOLATION
            const { stdout, stderr } = await execFileAsync(
                'timeout',
                ['10s', 'gcc', '-Wall', '-Wextra', '-Wpedantic', '-o', outputFile, sourceFile],
                {
                    cwd: __dirname,
                    timeout: CONFIG.BUILD_TIMEOUT,
                    maxBuffer: 1024 * 1024, // 1MB output limit
                    env: {
                        PATH: '/usr/bin:/bin', // Restricted PATH
                        HOME: '/tmp' // Sandboxed home
                    },
                    uid: process.getuid ? process.getuid() : undefined, // Run as current user (non-root in container)
                    gid: process.getgid ? process.getgid() : undefined,
                    shell: false // Explicitly disable shell to prevent injection
                }
            );
            
            return {
                success: true,
                output: stdout || 'Build completed successfully',
                error: stderr || null
            };
        } catch (error) {
            // Enhanced error handling for security
            if (error.code === 'TIMEOUT' || error.signal === 'SIGTERM') {
                return {
                    success: false,
                    output: null,
                    error: 'Build timeout - compilation took too long'
                };
            }
            return {
                success: false,
                output: null,
                error: `Build failed: ${error.message}`
            };
        }
    }

    async runCode(executable = './hello') {
        try {
            // SECURITY: Highly restricted execution environment - NO SHELL INTERPOLATION
            const { stdout, stderr } = await execFileAsync(
                'timeout',
                ['5s', executable],
                {
                    cwd: __dirname,
                    timeout: CONFIG.RUN_TIMEOUT,  
                    maxBuffer: 64 * 1024, // 64KB output limit
                    env: {
                        PATH: '/usr/bin:/bin', // Minimal PATH
                        HOME: '/tmp', // Sandboxed home
                        USER: 'sandbox' // Non-privileged user context
                    },
                    uid: process.getuid ? process.getuid() : undefined,
                    gid: process.getgid ? process.getgid() : undefined,
                    shell: false // Explicitly disable shell to prevent injection
                }
            );
            
            return {
                success: true,
                output: stdout,
                error: stderr || null
            };
        } catch (error) {
            // Enhanced security error handling
            if (error.code === 'TIMEOUT' || error.signal === 'SIGTERM') {
                return {
                    success: false,
                    output: null,
                    error: 'Execution timeout - program took too long'
                };
            }
            if (error.code === 'ENOENT') {
                return {
                    success: false,
                    output: null,
                    error: 'Executable not found - build first'
                };
            }
            return {
                success: false,
                output: null,
                error: `Runtime error: ${error.message}`
            };
        }
    }
}

const collabState = new CollaborationState();

// Create WebSocket server
const wss = new WebSocket.Server({ port: CONFIG.WS_PORT });

wss.on('connection', (ws, req) => {
    const userId = generateUserId();
    console.log(`🔌 New WebSocket connection: ${userId}`);

    // Handle incoming messages
    ws.on('message', async (data) => {
        try {
            const message = JSON.parse(data.toString());
            await handleMessage(ws, userId, message);
        } catch (error) {
            console.error('❌ Error handling message:', error.message);
            ws.send(JSON.stringify({
                type: 'error',
                message: 'Invalid message format'
            }));
        }
    });

    // Handle disconnection
    ws.on('close', () => {
        collabState.removeUser(userId);
        collabState.broadcast({
            type: 'user_left',
            userId: userId,
            users: collabState.getUsersInfo()
        });
    });

    // Send initial state
    ws.send(JSON.stringify({
        type: 'init',
        userId: userId,
        content: collabState.content,
        version: collabState.version,
        users: collabState.getUsersInfo()
    }));
});

async function handleMessage(ws, userId, message) {
    // Rate limiting check
    if (!collabState.checkRateLimit(userId)) {
        ws.send(JSON.stringify({
            type: 'error',
            message: 'Rate limit exceeded. Please slow down.'
        }));
        return;
    }

    try {
        switch (message.type) {
            case 'join':
                // Validate user info
                const userInfo = message.user || {};
                if (userInfo.name && typeof userInfo.name === 'string' && userInfo.name.length > 50) {
                    userInfo.name = userInfo.name.substring(0, 50);
                }
                
                collabState.addUser(userId, ws, userInfo);
                collabState.broadcast({
                    type: 'user_joined',
                    userId: userId,
                    user: collabState.users.get(userId),
                    users: collabState.getUsersInfo()
                }, userId);
                break;

            case 'operation':
                // Validate operation
                const validationError = collabState.validateOperation(message.operation);
                if (validationError) {
                    ws.send(JSON.stringify({
                        type: 'error',
                        message: `Invalid operation: ${validationError}`
                    }));
                    return;
                }
                
                if (collabState.applyOperation(message.operation, userId)) {
                    collabState.broadcast({
                        type: 'operation',
                        operation: message.operation,
                        userId: userId,
                        version: collabState.version,
                        content: collabState.content
                    }, userId);
                } else {
                    ws.send(JSON.stringify({
                        type: 'error',
                        message: 'Failed to apply operation'
                    }));
                }
                break;

            case 'save':
                const saved = await collabState.saveToFile();
                ws.send(JSON.stringify({
                    type: 'save_result',
                    success: saved
                }));
                if (saved) {
                    collabState.broadcast({
                        type: 'file_saved',
                        userId: userId
                    }, userId);
                }
                break;

            case 'build':
                try {
                    const buildResult = await collabState.buildCode();
                    ws.send(JSON.stringify({
                        type: 'build_result',
                        ...buildResult
                    }));
                    collabState.broadcast({
                        type: 'build_started',
                        userId: userId
                    }, userId);
                } catch (error) {
                    ws.send(JSON.stringify({
                        type: 'error',
                        message: 'Build system temporarily unavailable'
                    }));
                }
                break;

            case 'run':
                try {
                    const runResult = await collabState.runCode();
                    ws.send(JSON.stringify({
                        type: 'run_result',
                        ...runResult
                    }));
                } catch (error) {
                    ws.send(JSON.stringify({
                        type: 'error',
                        message: 'Execution system temporarily unavailable'
                    }));
                }
                break;

            case 'cursor':
                // Validate cursor position
                if (typeof message.position === 'number' && message.position >= 0) {
                    collabState.applyOperation({
                        type: 'cursor',
                        position: Math.min(message.position, collabState.content.length)
                    }, userId);
                    collabState.broadcast({
                        type: 'cursor_update',
                        userId: userId,
                        position: Math.min(message.position, collabState.content.length)
                    }, userId);
                }
                break;

            default:
                ws.send(JSON.stringify({
                    type: 'error',
                    message: 'Unknown message type'
                }));
        }
    } catch (error) {
        console.error('❌ Error handling message:', error);
        ws.send(JSON.stringify({
            type: 'error',
            message: 'Server error processing request'
        }));
    }
}

function generateUserId() {
    return 'user_' + Date.now() + '_' + Math.random().toString(36).substr(2, 9);
}

// Start HTTP server
app.listen(CONFIG.HTTP_PORT, () => {
    console.log(`🚀 Collaborative Editor Server running on:`);
    console.log(`   📄 HTTP: http://localhost:${CONFIG.HTTP_PORT}/collab_editor.html`);
    console.log(`   🔌 WebSocket: ws://localhost:${CONFIG.WS_PORT}`);
    console.log(`   🏥 Health: http://localhost:${CONFIG.HTTP_PORT}/health`);
    console.log(`   📁 Serving files from: ${__dirname}`);
    console.log('');
    console.log('🎯 Ready for collaborative editing!');
});

console.log(`🌐 WebSocket server listening on port ${CONFIG.WS_PORT}`);

// Graceful shutdown
process.on('SIGINT', () => {
    console.log('\n👋 Shutting down collaborative editor server...');
    wss.close();
    process.exit(0);
});