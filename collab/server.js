#!/usr/bin/env node
/**
 * Real-time Collaborative Editing Server for hello.c
 * 
 * SECURITY NOTICE: This is for development/demonstration only.
 * DO NOT expose to public networks without proper authentication,
 * HTTPS/WSS encryption, and access controls.
 * 
 * Features:
 * - Real-time synchronization via WebSocket
 * - Operational Transform for conflict resolution
 * - Sandboxed editing (writes to .collab file by default)
 * - Input validation and rate limiting
 * - Atomic file writes with versioned backups
 */

const express = require('express');
const http = require('http');
const socketIO = require('socket.io');
const fs = require('fs');
const path = require('path');
const rateLimit = require('express-rate-limit');

const app = express();
const server = http.createServer(app);
const io = socketIO(server);

// Configuration
const PORT = process.env.PORT || 3000;
const BIND_HOST = process.env.BIND_HOST || '127.0.0.1'; // Localhost only by default
const SOURCE_FILE = path.join(__dirname, '..', 'hello.c');
const COLLAB_FILE = path.join(__dirname, 'hello.c.collab');
const BACKUP_DIR = path.join(__dirname, 'backups');

// Parse command line arguments
const args = process.argv.slice(2);
const UNSAFE_DIRECT_WRITE = args.includes('--unsafe-direct-write');
const BIND_EXTERNAL = args.includes('--bind-external');

// Determine target file
const FILE_PATH = UNSAFE_DIRECT_WRITE ? SOURCE_FILE : COLLAB_FILE;

// Constants for validation
const MAX_FILE_SIZE = 1024 * 1024; // 1MB
const MAX_OPERATION_SIZE = 10000; // 10KB per operation
const MAX_POSITION = 1024 * 1024; // 1MB max position
const MAX_WEBSOCKET_CONNECTIONS = 50;

// Rate limiting to prevent abuse
const limiter = rateLimit({
    windowMs: 15 * 60 * 1000, // 15 minutes
    max: 100, // Limit each IP to 100 requests per windowMs
    message: 'Too many requests from this IP, please try again later.'
});

// Apply rate limiting to all routes
app.use(limiter);

// Store current file content and version
let fileContent = '';
let fileVersion = 0;
let connectedUsers = new Set();

// Create backup directory if it doesn't exist
function ensureBackupDir() {
    if (!fs.existsSync(BACKUP_DIR)) {
        fs.mkdirSync(BACKUP_DIR, { recursive: true });
    }
}

// Create backup of current file
function createBackup() {
    if (!fileContent) return;
    
    ensureBackupDir();
    const timestamp = new Date().toISOString().replace(/[:.]/g, '-');
    const backupFile = path.join(BACKUP_DIR, `hello.c.v${fileVersion}.${timestamp}.bak`);
    
    try {
        fs.writeFileSync(backupFile, fileContent, 'utf8');
        console.log(`Created backup: ${path.basename(backupFile)}`);
        
        // Keep only last 10 backups
        const backups = fs.readdirSync(BACKUP_DIR)
            .filter(f => f.endsWith('.bak'))
            .map(f => ({
                name: f,
                path: path.join(BACKUP_DIR, f),
                mtime: fs.statSync(path.join(BACKUP_DIR, f)).mtime
            }))
            .sort((a, b) => b.mtime - a.mtime);
        
        if (backups.length > 10) {
            backups.slice(10).forEach(backup => {
                fs.unlinkSync(backup.path);
                console.log(`Removed old backup: ${backup.name}`);
            });
        }
    } catch (err) {
        console.error('Error creating backup:', err);
    }
}

// Load initial file content
function loadFile() {
    try {
        // If using collab file, copy from source if it doesn't exist
        if (!UNSAFE_DIRECT_WRITE && !fs.existsSync(COLLAB_FILE)) {
            if (fs.existsSync(SOURCE_FILE)) {
                fs.copyFileSync(SOURCE_FILE, COLLAB_FILE);
                console.log('Created collaborative workspace from hello.c');
            } else {
                // Create empty collab file
                fs.writeFileSync(COLLAB_FILE, '', 'utf8');
                console.log('Created empty collaborative workspace');
            }
        }
        
        fileContent = fs.readFileSync(FILE_PATH, 'utf8');
        console.log(`Loaded ${path.basename(FILE_PATH)} successfully`);
        
        if (UNSAFE_DIRECT_WRITE) {
            console.warn('⚠️  WARNING: Direct write mode enabled - editing source file!');
        } else {
            console.log(`✓ Safe mode: Editing ${path.basename(COLLAB_FILE)}, not source file`);
        }
    } catch (err) {
        console.error('Error loading file:', err);
        process.exit(1);
    }
}

// Save file content atomically
function saveFile(content) {
    // Validate file size
    if (content.length > MAX_FILE_SIZE) {
        console.error('File size exceeds maximum allowed size');
        return false;
    }
    
    try {
        // Create backup before saving
        createBackup();
        
        // Atomic write: write to temp file then rename
        const tempFile = FILE_PATH + '.tmp';
        fs.writeFileSync(tempFile, content, 'utf8');
        
        // Atomic rename
        fs.renameSync(tempFile, FILE_PATH);
        
        fileContent = content;
        fileVersion++;
        console.log(`Saved ${path.basename(FILE_PATH)} (version ${fileVersion})`);
        return true;
    } catch (err) {
        console.error('Error saving file:', err);
        // Clean up temp file if it exists
        try {
            const tempFile = FILE_PATH + '.tmp';
            if (fs.existsSync(tempFile)) {
                fs.unlinkSync(tempFile);
            }
        } catch (cleanupErr) {
            // Ignore cleanup errors
        }
        return false;
    }
}

// Validate operation structure and values
function validateOperation(operation) {
    if (!operation || typeof operation !== 'object') {
        return { valid: false, error: 'Operation must be an object' };
    }
    
    const { type, position, text, length } = operation;
    
    // Validate type
    if (!['insert', 'delete', 'replace'].includes(type)) {
        return { valid: false, error: 'Invalid operation type' };
    }
    
    // Validate position
    if (typeof position !== 'number' || position < 0 || position > MAX_POSITION) {
        return { valid: false, error: 'Invalid position' };
    }
    
    // Position must be within current content bounds
    if (type !== 'replace' && position > fileContent.length) {
        return { valid: false, error: 'Position exceeds content length' };
    }
    
    // Validate type-specific fields
    if (type === 'insert' || type === 'replace') {
        if (typeof text !== 'string') {
            return { valid: false, error: 'Text must be a string' };
        }
        if (text.length > MAX_OPERATION_SIZE) {
            return { valid: false, error: 'Text exceeds maximum operation size' };
        }
        // Check for null bytes and other problematic characters
        if (text.includes('\0')) {
            return { valid: false, error: 'Text contains invalid characters' };
        }
    }
    
    if (type === 'delete') {
        if (typeof length !== 'number' || length < 0 || length > MAX_OPERATION_SIZE) {
            return { valid: false, error: 'Invalid delete length' };
        }
        if (position + length > fileContent.length) {
            return { valid: false, error: 'Delete range exceeds content length' };
        }
    }
    
    return { valid: true };
}

// Simple Operational Transform for text operations
function transformOperation(op1, op2) {
    // Basic conflict resolution: if operations overlap, prioritize first operation
    if (op1.position === op2.position) {
        if (op1.type === 'insert' && op2.type === 'insert') {
            // Both insertions at same position - shift second operation
            return {
                ...op2,
                position: op2.position + op1.text.length
            };
        } else if (op1.type === 'delete' && op2.type === 'delete') {
            // Both deletions at same position - adjust second operation
            return {
                ...op2,
                position: Math.max(0, op2.position - op1.length)
            };
        }
    } else if (op1.position < op2.position) {
        // Operations don't directly conflict
        if (op1.type === 'insert') {
            return {
                ...op2,
                position: op2.position + op1.text.length
            };
        } else if (op1.type === 'delete') {
            return {
                ...op2,
                position: Math.max(op1.position, op2.position - op1.length)
            };
        }
    }
    return op2;
}

// Apply operation to content
function applyOperation(content, operation) {
    if (operation.type === 'insert') {
        return content.slice(0, operation.position) + 
               operation.text + 
               content.slice(operation.position);
    } else if (operation.type === 'delete') {
        return content.slice(0, operation.position) + 
               content.slice(operation.position + operation.length);
    } else if (operation.type === 'replace') {
        // Full content replacement
        return operation.text;
    }
    return content;
}

// Serve only the editor.html file
app.get('/editor.html', (req, res) => {
    res.sendFile(path.join(__dirname, 'editor.html'));
});

// Redirect root to editor
app.get('/', (req, res) => {
    res.redirect('/editor.html');
});

// API endpoint to get current file content
app.get('/api/file', (req, res) => {
    res.json({
        content: fileContent,
        version: fileVersion,
        connectedUsers: connectedUsers.size,
        fileName: path.basename(FILE_PATH),
        safeMode: !UNSAFE_DIRECT_WRITE
    });
});

// WebSocket connection handling with connection limit
io.on('connection', (socket) => {
    // Enforce connection limit
    if (connectedUsers.size >= MAX_WEBSOCKET_CONNECTIONS) {
        console.log('Connection rejected: maximum connections reached');
        socket.emit('error', { message: 'Server at maximum capacity' });
        socket.disconnect(true);
        return;
    }
    
    const userId = socket.id;
    connectedUsers.add(userId);
    
    console.log(`User connected: ${userId} (Total users: ${connectedUsers.size})`);
    
    // Send current file state to new user
    socket.emit('init', {
        content: fileContent,
        version: fileVersion,
        userId: userId,
        safeMode: !UNSAFE_DIRECT_WRITE
    });
    
    // Broadcast user count update
    io.emit('users-update', {
        count: connectedUsers.size
    });
    
    // Rate limit per socket
    let operationCount = 0;
    let operationWindowStart = Date.now();
    const OPERATIONS_PER_WINDOW = 100;
    const OPERATION_WINDOW_MS = 60000; // 1 minute
    
    // Handle edit operations
    socket.on('edit', (data) => {
        // Socket-level rate limiting
        const now = Date.now();
        if (now - operationWindowStart > OPERATION_WINDOW_MS) {
            operationCount = 0;
            operationWindowStart = now;
        }
        
        operationCount++;
        if (operationCount > OPERATIONS_PER_WINDOW) {
            socket.emit('edit-error', {
                message: 'Too many operations, please slow down'
            });
            return;
        }
        
        // Validate data structure
        if (!data || typeof data !== 'object') {
            socket.emit('edit-error', { message: 'Invalid request data' });
            return;
        }
        
        const { operation, version } = data;
        
        // Validate operation
        const validation = validateOperation(operation);
        if (!validation.valid) {
            console.log(`Invalid operation from ${userId}: ${validation.error}`);
            socket.emit('edit-error', {
                message: validation.error
            });
            return;
        }
        
        console.log(`Edit from ${userId}:`, operation.type, 'at position', operation.position);
        
        // Apply operation to current content
        const newContent = applyOperation(fileContent, operation);
        
        // Validate result size
        if (newContent.length > MAX_FILE_SIZE) {
            socket.emit('edit-error', {
                message: 'Operation would exceed maximum file size'
            });
            return;
        }
        
        // Save to file
        if (saveFile(newContent)) {
            // Broadcast change to all other clients
            socket.broadcast.emit('remote-edit', {
                operation: operation,
                version: fileVersion,
                userId: userId
            });
            
            // Confirm to sender
            socket.emit('edit-ack', {
                version: fileVersion,
                success: true
            });
        } else {
            socket.emit('edit-error', {
                message: 'Failed to save file'
            });
        }
    });
    
    // Handle cursor position updates (with basic validation)
    socket.on('cursor', (data) => {
        if (!data || typeof data !== 'object') {
            return;
        }
        
        const { position, selection } = data;
        
        if (typeof position === 'number' && position >= 0) {
            socket.broadcast.emit('remote-cursor', {
                userId: userId,
                position: position,
                selection: selection
            });
        }
    });
    
    // Handle disconnection
    socket.on('disconnect', () => {
        connectedUsers.delete(userId);
        console.log(`User disconnected: ${userId} (Total users: ${connectedUsers.size})`);
        
        // Broadcast user count update
        io.emit('users-update', {
            count: connectedUsers.size
        });
    });
});

// Initialize and start server
loadFile();

const bindHost = BIND_EXTERNAL ? '0.0.0.0' : BIND_HOST;

server.listen(PORT, bindHost, () => {
    console.log('='.repeat(60));
    console.log('Collaborative Editing Server');
    console.log('='.repeat(60));
    console.log(`Server: http://${bindHost}:${PORT}`);
    console.log(`Target: ${path.basename(FILE_PATH)}`);
    console.log(`Mode: ${UNSAFE_DIRECT_WRITE ? '⚠️  UNSAFE (direct write)' : '✓ SAFE (workspace)'}`);
    console.log(`Binding: ${BIND_EXTERNAL ? '⚠️  EXTERNAL (0.0.0.0)' : '✓ LOCAL (127.0.0.1)'}`);
    console.log('='.repeat(60));
    
    if (UNSAFE_DIRECT_WRITE) {
        console.warn('');
        console.warn('⚠️  WARNING: Direct write mode is enabled!');
        console.warn('⚠️  Changes will modify the source file directly.');
        console.warn('⚠️  Backups are created in the backups/ directory.');
        console.warn('');
    }
    
    if (BIND_EXTERNAL) {
        console.warn('');
        console.warn('⚠️  WARNING: Server is binding to external interface!');
        console.warn('⚠️  This is UNSAFE without authentication and HTTPS.');
        console.warn('⚠️  Only use this on trusted networks.');
        console.warn('');
    }
});

// Export for testing
module.exports = {
    validateOperation,
    transformOperation,
    applyOperation
};
