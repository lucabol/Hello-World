# How It Works: Real-Time Collaborative Editing

This document explains the technical implementation of the real-time collaborative editing system for hello.c.

## Architecture Overview

```
┌─────────────┐         WebSocket          ┌──────────────┐
│  Browser 1  │◄──────────────────────────►│              │
│  (Client)   │                             │              │
└─────────────┘                             │              │
                                            │   Node.js    │
┌─────────────┐         WebSocket          │   Server     │◄──► hello.c
│  Browser 2  │◄──────────────────────────►│   (Express   │     (File)
│  (Client)   │                             │   + WS)      │
└─────────────┘                             │              │
                                            │              │
┌─────────────┐         WebSocket          │              │
│  Browser N  │◄──────────────────────────►│              │
│  (Client)   │                             └──────────────┘
└─────────────┘
```

## Communication Flow

### 1. Initial Connection

When a client connects:

1. **Client** opens WebSocket connection to server
2. **Server** assigns a unique client ID
3. **Server** sends initial state:
   - Current file content
   - Version number
   - List of connected users
4. **Server** broadcasts to other clients: "User X joined"
5. **Client** renders the editor with received content

### 2. Editing Flow

When a user types:

1. **Client** captures edit in textarea
2. **Client** debounces changes (500ms delay)
3. **Client** sends edit message to server:
   ```json
   {
     "type": "edit",
     "content": "# include <stdio.h>\n\nint main(){\n..."
   }
   ```
4. **Server** receives edit and writes to hello.c
5. **Server** increments version number
6. **Server** broadcasts to all other clients:
   ```json
   {
     "type": "update",
     "content": "...",
     "version": 123,
     "clientId": "abc123"
   }
   ```
7. **Other clients** receive update and apply to their editor
8. **Originating client** receives acknowledgment:
   ```json
   {
     "type": "ack",
     "version": 123
   }
   ```

### 3. Conflict Resolution

The system uses a **last-write-wins** strategy:

1. **No locking**: Multiple users can edit simultaneously
2. **Version tracking**: Each save increments a version counter
3. **Last write wins**: The most recent edit to reach the server is saved
4. **Broadcast**: All clients receive the latest version

**Example scenario:**
- User A types "hello" at position 10
- User B types "world" at position 20
- Both send edits ~simultaneously
- Server receives A's edit first → saves it (version 101)
- Server receives B's edit → overwrites with B's version (version 102)
- All clients receive version 102 (B's edit wins)

**Trade-offs:**
- ✅ Simple and fast
- ✅ No complex merging logic
- ✅ Works well when users edit different parts
- ⚠️ May lose edits if multiple users edit the same area simultaneously
- ⚠️ Best suited for low-conflict scenarios

### 4. User Presence

The server tracks connected users:

1. **On connect**: User added to active users map
2. **Username change**: Broadcast to all clients
3. **On disconnect**: User removed, broadcast to all clients

Each client shows:
- List of connected users
- Current user marked with "(you)"
- Green indicator for active users

## Message Types

### Client → Server

| Type | Purpose | Payload |
|------|---------|---------|
| `edit` | Send content change | `{ content: string }` |
| `cursor` | Share cursor position | `{ position: number }` |
| `set-name` | Update username | `{ name: string }` |

### Server → Client

| Type | Purpose | Payload |
|------|---------|---------|
| `init` | Initial connection state | `{ content, version, clientId, clients }` |
| `update` | Content change from another user | `{ content, version, clientId }` |
| `ack` | Acknowledge successful save | `{ version }` |
| `user-joined` | New user connected | `{ clientId, name }` |
| `user-left` | User disconnected | `{ clientId, name }` |
| `user-renamed` | User changed name | `{ clientId, oldName, newName }` |
| `error` | Error message | `{ message }` |

## File Synchronization

### Writing to hello.c

```javascript
async function saveFile(content) {
    await fs.writeFile(HELLO_C_PATH, content, 'utf8');
    fileContent = content;
    version++;
    return true;
}
```

**Atomicity**: Node.js `fs.writeFile` is atomic at the OS level, preventing partial writes.

**Version tracking**: Monotonically increasing counter ensures clients can detect out-of-order updates.

### Reading from hello.c

- Initial load: File read on server startup
- API endpoint: `/api/content` provides current content
- In-memory cache: Server maintains current content in memory

## Performance Considerations

### Debouncing

Client debounces keystrokes (500ms) to avoid overwhelming the server:

```javascript
let saveTimeout;
editor.addEventListener('input', () => {
    clearTimeout(saveTimeout);
    saveTimeout = setTimeout(() => {
        saveContent();
    }, 500);
});
```

### Connection Management

- **Reconnection**: Automatic reconnection with exponential backoff
- **Heartbeat**: WebSocket built-in ping/pong for connection health
- **Graceful degradation**: Editor becomes read-only when disconnected

### Scalability

Current implementation is suitable for:
- ✅ Small teams (2-10 users)
- ✅ Single file editing
- ✅ Low-frequency edits

For larger scale, consider:
- Message queuing (Redis, RabbitMQ)
- Horizontal scaling with sticky sessions
- Operational Transformation or CRDTs for better conflict resolution
- Database for persistence and history

## Security Considerations

### Current Implementation

- ✅ Input validation on WebSocket messages
- ✅ No SQL injection (no database)
- ✅ No XSS (plain text content)
- ✅ Patched dependencies (ws 8.17.1)

### Production Hardening

For production deployment, add:

1. **Authentication**
   - User login system
   - Session management
   - JWT tokens for WebSocket auth

2. **Authorization**
   - Read/write permissions
   - Role-based access control

3. **Rate Limiting**
   - Limit edits per user per minute
   - Prevent DoS attacks

4. **Encryption**
   - HTTPS for web interface
   - WSS (WebSocket Secure) for real-time communication
   - TLS certificates

5. **Input Validation**
   - Sanitize all user input
   - Validate message structure
   - Limit message size

6. **Audit Logging**
   - Log all edits with timestamps
   - Track user actions
   - Enable rollback capability

## Future Enhancements

### Operational Transformation (OT)

Replace last-write-wins with OT for better conflict resolution:

```
User A: Insert "x" at position 5
User B: Insert "y" at position 10

Transform operations so both apply correctly:
- A's operation applied to B's state
- B's operation transformed against A's operation
- Result: Both edits preserved
```

### Conflict-free Replicated Data Types (CRDTs)

Use CRDTs for mathematical conflict-free merging:
- Each character has unique ID
- Operations commute (order-independent)
- Guaranteed convergence

### Cursor Sharing

Show where other users are typing:
- Colored cursors for each user
- Username labels above cursors
- Selection highlighting

### Rich Editing Features

- Syntax highlighting for C code
- Auto-completion
- Find and replace
- Multiple file support
- Split view editing

## Testing the System

### Manual Testing

1. Open http://localhost:3000 in Browser 1
2. Open http://localhost:3000 in Browser 2
3. Type in Browser 1 → observe update in Browser 2
4. Type in Browser 2 → observe update in Browser 1
5. Change username in Browser 1 → observe in Browser 2 sidebar
6. Close Browser 1 → observe "User left" in Browser 2

### Automated Testing

See `test/test_collaborative_editor.sh` for integration tests:
- Server startup
- HTTP endpoint accessibility
- API endpoint functionality
- JSON response validation
- Content verification

## Troubleshooting

### Changes not syncing

1. Check WebSocket connection status (top right badge)
2. Open browser console for error messages
3. Verify server is running: `curl http://localhost:3000/api/content`
4. Check server logs for errors

### Lost edits

- Expected behavior with last-write-wins
- Reduce typing speed or coordinate with other users
- Consider upgrading to OT/CRDT for production use

### High latency

- Check network connection
- Reduce debounce delay (may increase server load)
- Consider local hosting for better performance

## References

- [WebSocket API (MDN)](https://developer.mozilla.org/en-US/docs/Web/API/WebSocket)
- [Operational Transformation](https://en.wikipedia.org/wiki/Operational_transformation)
- [Conflict-free Replicated Data Types](https://en.wikipedia.org/wiki/Conflict-free_replicated_data_type)
- [Real-time Collaborative Editing](https://www.ably.io/topic/collaborative-editing)
