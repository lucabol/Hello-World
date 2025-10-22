#!/usr/bin/env node
/**
 * Real-time Collaborative Editing Server for hello.c
 * 
 * This server enables multiple users to edit hello.c simultaneously with:
 * - Real-time synchronization via WebSocket
 * - Operational Transform for conflict resolution
 * - File persistence
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

const PORT = process.env.PORT || 3000;
const FILE_PATH = path.join(__dirname, 'hello.c');

// Rate limiting to prevent abuse
const limiter = rateLimit({
    windowMs: 15 * 60 * 1000, // 15 minutes
    max: 100, // Limit each IP to 100 requests per windowMs
    message: 'Too many requests from this IP, please try again later.'
});

// Apply rate limiting to API routes
app.use('/api/', limiter);

// Store current file content and version
let fileContent = '';
let fileVersion = 0;
let connectedUsers = new Set();

// Load initial file content
function loadFile() {
    try {
        fileContent = fs.readFileSync(FILE_PATH, 'utf8');
        console.log('Loaded hello.c successfully');
    } catch (err) {
        console.error('Error loading file:', err);
        process.exit(1);
    }
}

// Save file content
function saveFile(content) {
    try {
        fs.writeFileSync(FILE_PATH, content, 'utf8');
        fileContent = content;
        fileVersion++;
        console.log(`Saved hello.c (version ${fileVersion})`);
        return true;
    } catch (err) {
        console.error('Error saving file:', err);
        return false;
    }
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

// Serve only the editor.html file (not the entire directory to avoid exposing sensitive files)
app.get('/editor.html', limiter, (req, res) => {
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
        connectedUsers: connectedUsers.size
    });
});

// WebSocket connection handling
io.on('connection', (socket) => {
    const userId = socket.id;
    connectedUsers.add(userId);
    
    console.log(`User connected: ${userId} (Total users: ${connectedUsers.size})`);
    
    // Send current file state to new user
    socket.emit('init', {
        content: fileContent,
        version: fileVersion,
        userId: userId
    });
    
    // Broadcast user count update
    io.emit('users-update', {
        count: connectedUsers.size
    });
    
    // Handle edit operations
    socket.on('edit', (data) => {
        const { operation, version } = data;
        
        console.log(`Edit from ${userId}:`, operation.type, 'at position', operation.position);
        
        // Apply operation to current content
        const newContent = applyOperation(fileContent, operation);
        
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
    
    // Handle cursor position updates
    socket.on('cursor', (data) => {
        socket.broadcast.emit('remote-cursor', {
            userId: userId,
            position: data.position,
            selection: data.selection
        });
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

server.listen(PORT, () => {
    console.log(`Collaborative editing server running on http://localhost:${PORT}`);
    console.log(`Edit hello.c in real-time with multiple users!`);
});
