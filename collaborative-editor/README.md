# Real-Time Collaborative Editor for hello.c

⚠️ **EXPERIMENTAL**: This is a prototype collaborative editor. See [SECURITY.md](SECURITY.md) for important security information before deploying.

This collaborative editor allows multiple users to edit `hello.c` simultaneously in real-time with live updates and automatic conflict resolution.

## Security Notice

**Default configuration is localhost-only and safe for local development.**

- Server binds to `127.0.0.1` (localhost) by default
- Repository file writes require explicit opt-in
- No authentication by default (not for production)
- See [SECURITY.md](SECURITY.md) for complete security and deployment guidance

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

3. Configure environment (optional but recommended):
```bash
# For local testing (safe defaults)
npm start

# For production or network access
export COLLAB_AUTH_TOKEN=$(openssl rand -hex 32)
export COLLAB_TARGET_FILE=/path/to/your/file.c
npm start
```

**Important**: Review [SECURITY.md](SECURITY.md) before deploying to production.

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

### Environment Variables

All configuration is done via environment variables for security:

- `COLLAB_HOST`: Server bind address (default: `127.0.0.1` for localhost-only)
- `COLLAB_PORT`: Server port (default: `3000`)
- `COLLAB_TARGET_FILE`: Path to file to edit (default: `./data/hello.c`)
- `COLLAB_ALLOW_REPO_WRITE`: Set to `true` to enable writing to repository files (default: `false`)
- `COLLAB_AUTH_TOKEN`: Authentication token (optional but recommended)
- `COLLAB_MAX_MESSAGE_SIZE`: Max message size in bytes (default: `1048576`)
- `COLLAB_MAX_FILE_SIZE`: Max file size in bytes (default: `1048576`)
- `COLLAB_RATE_LIMIT`: Max edits per minute per client (default: `30`)
- `COLLAB_LOG_LEVEL`: Logging level: `error`, `warn`, `info`, `debug` (default: `info`)

### Example Configurations

**Local development (default):**
```bash
npm start
# Server runs on http://127.0.0.1:3000
# Edits data/hello.c (outside repository)
```

**With authentication:**
```bash
export COLLAB_AUTH_TOKEN="your-secret-token"
npm start
# Access with: http://127.0.0.1:3000?token=your-secret-token
```

**Edit repository file:**
```bash
export COLLAB_ALLOW_REPO_WRITE=true
export COLLAB_TARGET_FILE=/path/to/repo/hello.c
npm start
# WARNING: This modifies repository files
```

See [SECURITY.md](SECURITY.md) for complete configuration guidance.

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

## Testing

### Unit Tests

Run unit tests for core server logic:

```bash
npm test
```

Tests cover:
- Input validation and sanitization
- Rate limiting logic
- Atomic file writes
- Message parsing and validation
- Configuration validation
- XSS prevention

### Concurrent Editing Tests

Run integration tests with multiple simultaneous clients:

```bash
npm run test:concurrent
```

Tests cover:
- Multiple client connections
- Edit broadcasting to all clients
- Version number incrementing
- Concurrent edit handling (last-write-wins)
- Rate limiting under load
- Large message rejection
- Invalid message format handling
- Client disconnection cleanup
- XSS prevention in usernames

### Run All Tests

```bash
npm run test:all
```

This runs both unit tests and concurrent editing tests.

### Integration Tests

Run integration tests (requires server to be stopped):

```bash
../test/test_collaborative_editor.sh
```

### Linting

Check code style and potential issues:

```bash
npm run lint
```

### Manual Testing

1. Start the server: `npm start`
2. Open http://127.0.0.1:3000 in multiple browser windows
3. Type in one window and verify changes appear in others
4. Test conflict resolution by typing simultaneously in multiple windows
5. Test authentication (if configured) by connecting with/without token
6. Test rate limiting by rapidly sending edits

### CI Integration

The test suite is designed to run in CI environments:

```yaml
# Example GitHub Actions workflow
- name: Install dependencies
  run: cd collaborative-editor && npm ci
  
- name: Run unit tests
  run: cd collaborative-editor && npm test
  
- name: Run linting
  run: cd collaborative-editor && npm run lint
  
- name: Run integration tests
  run: ./test/test_collaborative_editor.sh
```

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
