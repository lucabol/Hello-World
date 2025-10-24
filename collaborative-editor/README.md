# Real-Time Collaborative Editor for hello.c

This collaborative editor allows multiple users to edit `hello.c` simultaneously in real-time with live updates and automatic conflict resolution.

## Features

- **Real-time synchronization**: Changes are instantly propagated to all connected users
- **Multi-user support**: See who else is editing the file
- **Conflict resolution**: Uses a simple last-write-wins strategy for automatic conflict handling
- **WebSocket-based**: Low-latency communication for responsive editing
- **Auto-save**: Changes are automatically saved after a brief delay
- **Cursor preservation**: Attempts to maintain cursor position during remote updates
- **Reconnection**: Automatically reconnects if connection is lost

## Prerequisites

- Node.js (version 14 or higher)
- npm (Node Package Manager)

## Installation

1. Navigate to the collaborative-editor directory:
```bash
cd collaborative-editor
```

2. Install dependencies:
```bash
npm install
```

## Usage

1. Start the server:
```bash
npm start
```

2. Open your web browser and navigate to:
```
http://localhost:3000
```

3. To test collaborative editing, open the same URL in multiple browser windows or tabs.

## Configuration

The server can be configured using environment variables:

- `PORT`: Server port (default: 3000)
  ```bash
  PORT=8080 npm start
  ```

## Architecture

### Backend (server.js)

- **Express server**: Serves the web interface and provides REST API
- **WebSocket server**: Handles real-time bidirectional communication
- **File management**: Reads and writes to hello.c
- **Conflict resolution**: Implements last-write-wins strategy with version tracking

### Frontend (public/)

- **index.html**: Main editor interface
- **style.css**: Dark-themed code editor styling
- **editor.js**: WebSocket client and editor logic

## How It Works

1. **Connection**: When a client connects, they receive the current file content and version number
2. **Editing**: As users type, changes are debounced and sent to the server
3. **Broadcasting**: The server saves changes and broadcasts them to all other connected clients
4. **Updates**: Remote updates are applied while attempting to preserve cursor position
5. **Conflict resolution**: The last edit received by the server wins (last-write-wins)

## Security Considerations

- This is a minimal implementation intended for local or trusted network use
- For production use, consider adding:
  - Authentication and authorization
  - Input validation and sanitization
  - Rate limiting
  - HTTPS/WSS encryption
  - More sophisticated conflict resolution (Operational Transformation or CRDTs)

## Limitations

- **Simple conflict resolution**: Uses last-write-wins, which may result in lost edits if multiple users edit the same line simultaneously
- **No edit history**: Changes are not versioned beyond a simple counter
- **No undo/redo**: Basic text editor functionality only
- **Cursor position**: Preservation is approximate and may not work perfectly for all edit scenarios

## Future Enhancements

- Implement Operational Transformation (OT) or Conflict-free Replicated Data Types (CRDTs) for better conflict resolution
- Add cursor position sharing to show where other users are typing
- Implement edit history and version control
- Add syntax highlighting for C code
- Add authentication and user management
- Implement file locking or section locking
- Add chat or commenting functionality

## Troubleshooting

### Server won't start
- Ensure Node.js is installed: `node --version`
- Check if port 3000 is already in use: `lsof -i :3000` (Unix/Mac) or `netstat -ano | findstr :3000` (Windows)
- Try a different port: `PORT=8080 npm start`

### Can't connect to WebSocket
- Ensure the server is running
- Check browser console for error messages
- Verify firewall settings aren't blocking the connection

### Changes not syncing
- Check WebSocket connection status in the UI
- Look for errors in browser console
- Verify hello.c file permissions allow writing

## License

MIT
