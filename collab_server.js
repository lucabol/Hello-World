#!/usr/bin/env node
/**
 * Collaborative Editor WebSocket Server
 * Enables real-time collaborative editing of hello.c with conflict resolution
 */

const WebSocket = require('ws');
const express = require('express');
const fs = require('fs').promises;
const path = require('path');
const { exec } = require('child_process');
const { promisify } = require('util');

const execAsync = promisify(exec);
const app = express();
const PORT = process.env.PORT || 8082;
const WS_PORT = process.env.WS_PORT || 8083;

// Serve static files
app.use(express.static(__dirname));
app.use(express.json());

// In-memory storage for collaboration state
class CollaborationState {
    constructor() {
        this.content = '';
        this.users = new Map();
        this.version = 0;
        this.operations = [];
        this.loadInitialContent();
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
        this.users.set(userId, {
            ws: ws,
            name: userInfo.name || `User-${Math.floor(Math.random() * 1000)}`,
            color: userInfo.color || this.getRandomColor(),
            cursor: 0,
            joinedAt: new Date()
        });
        console.log(`👤 User ${userInfo.name} joined (${this.users.size} total users)`);
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
        // Simple operational transformation for conflict resolution
        const user = this.users.get(userId);
        if (!user) return false;

        try {
            switch (operation.type) {
                case 'insert':
                    this.content = this.content.slice(0, operation.position) + 
                                   operation.content + 
                                   this.content.slice(operation.position);
                    break;
                case 'delete':
                    this.content = this.content.slice(0, operation.position) + 
                                   this.content.slice(operation.position + operation.length);
                    break;
                case 'replace':
                    this.content = operation.content;
                    break;
                case 'cursor':
                    user.cursor = operation.position;
                    break;
            }

            if (operation.type !== 'cursor') {
                this.version++;
                this.operations.push({
                    ...operation,
                    userId,
                    timestamp: new Date(),
                    version: this.version
                });
            }

            return true;
        } catch (error) {
            console.error('❌ Failed to apply operation:', error.message);
            return false;
        }
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

    async buildCode() {
        try {
            const { stdout, stderr } = await execAsync('gcc -Wall -Wextra -Wpedantic -o hello hello.c', {
                cwd: __dirname
            });
            
            return {
                success: true,
                output: stdout || 'Build completed successfully',
                error: stderr || null
            };
        } catch (error) {
            return {
                success: false,
                output: null,
                error: error.message
            };
        }
    }

    async runCode() {
        try {
            const { stdout, stderr } = await execAsync('./hello', {
                cwd: __dirname,
                timeout: 5000
            });
            
            return {
                success: true,
                output: stdout,
                error: stderr || null
            };
        } catch (error) {
            return {
                success: false,
                output: null,
                error: error.message
            };
        }
    }
}

const collabState = new CollaborationState();

// Create WebSocket server
const wss = new WebSocket.Server({ port: WS_PORT });

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
    switch (message.type) {
        case 'join':
            collabState.addUser(userId, ws, message.user);
            collabState.broadcast({
                type: 'user_joined',
                userId: userId,
                user: collabState.users.get(userId),
                users: collabState.getUsersInfo()
            }, userId);
            break;

        case 'operation':
            if (collabState.applyOperation(message.operation, userId)) {
                collabState.broadcast({
                    type: 'operation',
                    operation: message.operation,
                    userId: userId,
                    version: collabState.version,
                    content: collabState.content
                }, userId);
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
            const buildResult = await collabState.buildCode();
            ws.send(JSON.stringify({
                type: 'build_result',
                ...buildResult
            }));
            collabState.broadcast({
                type: 'build_started',
                userId: userId
            }, userId);
            break;

        case 'run':
            const runResult = await collabState.runCode();
            ws.send(JSON.stringify({
                type: 'run_result',
                ...runResult
            }));
            break;

        case 'cursor':
            collabState.applyOperation({
                type: 'cursor',
                position: message.position
            }, userId);
            collabState.broadcast({
                type: 'cursor_update',
                userId: userId,
                position: message.position
            }, userId);
            break;

        default:
            console.warn('⚠️ Unknown message type:', message.type);
    }
}

function generateUserId() {
    return 'user_' + Date.now() + '_' + Math.random().toString(36).substr(2, 9);
}

// Start HTTP server
app.listen(PORT, () => {
    console.log(`🚀 Collaborative Editor Server running on:`);
    console.log(`   📄 HTTP: http://localhost:${PORT}/collab_editor.html`);
    console.log(`   🔌 WebSocket: ws://localhost:${WS_PORT}`);
    console.log(`   📁 Serving files from: ${__dirname}`);
    console.log('');
    console.log('🎯 Ready for collaborative editing!');
});

console.log(`🌐 WebSocket server listening on port ${WS_PORT}`);

// Graceful shutdown
process.on('SIGINT', () => {
    console.log('\n👋 Shutting down collaborative editor server...');
    wss.close();
    process.exit(0);
});