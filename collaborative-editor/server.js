const express = require('express');
const http = require('http');
const WebSocket = require('ws');
const fs = require('fs').promises;
const path = require('path');

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

const HELLO_C_PATH = path.join(__dirname, '..', 'hello.c');
const PORT = process.env.PORT || 3000;

// Store connected clients and their metadata
const clients = new Map();
let fileContent = '';
let version = 0;

// Load initial file content
async function loadFile() {
    try {
        fileContent = await fs.readFile(HELLO_C_PATH, 'utf8');
        console.log('Loaded hello.c successfully');
    } catch (error) {
        console.error('Error loading hello.c:', error);
        fileContent = '# include <stdio.h>\n\nint main(){\n    printf("Hello world!");\n}\n';
    }
}

// Save file content
async function saveFile(content) {
    try {
        await fs.writeFile(HELLO_C_PATH, content, 'utf8');
        fileContent = content;
        version++;
        console.log(`Saved hello.c (version ${version})`);
        return true;
    } catch (error) {
        console.error('Error saving hello.c:', error);
        return false;
    }
}

// Broadcast message to all clients except sender
function broadcast(message, excludeClient = null) {
    const messageStr = JSON.stringify(message);
    clients.forEach((metadata, client) => {
        if (client !== excludeClient && client.readyState === WebSocket.OPEN) {
            client.send(messageStr);
        }
    });
}

// Handle WebSocket connections
wss.on('connection', (ws) => {
    const clientId = Math.random().toString(36).substr(2, 9);
    clients.set(ws, { id: clientId, name: `User-${clientId.substr(0, 4)}` });
    
    console.log(`Client ${clientId} connected. Total clients: ${clients.size}`);
    
    // Send initial state to new client
    ws.send(JSON.stringify({
        type: 'init',
        content: fileContent,
        version: version,
        clientId: clientId,
        clients: Array.from(clients.values()).map(c => ({ id: c.id, name: c.name }))
    }));
    
    // Notify other clients about new user
    broadcast({
        type: 'user-joined',
        clientId: clientId,
        name: clients.get(ws).name
    }, ws);
    
    // Handle messages from client
    ws.on('message', async (message) => {
        try {
            const data = JSON.parse(message);
            
            switch (data.type) {
                case 'edit':
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
                    } else {
                        ws.send(JSON.stringify({
                            type: 'error',
                            message: 'Failed to save file'
                        }));
                    }
                    break;
                    
                case 'cursor':
                    // Broadcast cursor position to other clients
                    broadcast({
                        type: 'cursor',
                        clientId: clientId,
                        position: data.position
                    }, ws);
                    break;
                    
                case 'set-name':
                    // Update client name
                    const oldName = clients.get(ws).name;
                    clients.get(ws).name = data.name;
                    broadcast({
                        type: 'user-renamed',
                        clientId: clientId,
                        oldName: oldName,
                        newName: data.name
                    }, ws);
                    break;
            }
        } catch (error) {
            console.error('Error handling message:', error);
            ws.send(JSON.stringify({
                type: 'error',
                message: 'Invalid message format'
            }));
        }
    });
    
    // Handle client disconnect
    ws.on('close', () => {
        const metadata = clients.get(ws);
        clients.delete(ws);
        console.log(`Client ${clientId} disconnected. Total clients: ${clients.size}`);
        
        // Notify other clients
        broadcast({
            type: 'user-left',
            clientId: clientId,
            name: metadata.name
        });
    });
    
    // Handle errors
    ws.on('error', (error) => {
        console.error(`WebSocket error for client ${clientId}:`, error);
    });
});

// Serve static files
app.use(express.static(path.join(__dirname, 'public')));

// API endpoint to get current file content
app.get('/api/content', async (req, res) => {
    res.json({
        content: fileContent,
        version: version
    });
});

// Start server
async function start() {
    await loadFile();
    server.listen(PORT, () => {
        console.log(`Collaborative editor server running on http://localhost:${PORT}`);
        console.log(`WebSocket server ready for connections`);
    });
}

start().catch(error => {
    console.error('Failed to start server:', error);
    process.exit(1);
});
