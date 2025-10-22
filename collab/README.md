# Collaborative Editing - Quick Start

## ⚠️ IMPORTANT SECURITY NOTICE

**This is for DEVELOPMENT and DEMONSTRATION purposes only.**

**DO NOT use in production or expose to public networks without:**
- Proper authentication and authorization
- HTTPS/WSS encryption
- Network access controls
- Security audit and hardening

## Quick Start (Safe Mode)

The server runs in **safe mode** by default, editing a workspace file (`hello.c.collab`) instead of the source file.

### 1. Install Dependencies
```bash
cd collab
npm install
```

### 2. Start Server (Safe Mode - Localhost Only)
```bash
npm start
```

The server will:
- ✓ Bind to `127.0.0.1` (localhost only)
- ✓ Edit `hello.c.collab` (workspace file)
- ✓ Create backups in `backups/` directory
- ✓ Apply rate limiting and validation

### 3. Open Editor
```bash
# Open in your browser
http://localhost:3000/editor.html
```

Open the same URL in multiple browser tabs to test collaborative editing!

## Testing

Run the test suite:
```bash
npm test
```

Tests cover:
- Operation validation
- Operational Transform algorithm
- Operation application

## Advanced Usage

### Unsafe Direct Write Mode

⚠️ **WARNING**: This mode writes directly to `../hello.c`

```bash
npm run start:unsafe
```

Use only if you:
- Need to modify the source file directly
- Have backups enabled (automatic in `backups/`)
- Understand the risks

### External Binding

⚠️ **WARNING**: This allows external network access

```bash
npm run start:external
```

Only use on trusted networks. Requires authentication for production use.

### Combined Flags

```bash
node server.js --unsafe-direct-write --bind-external
```

## File Locations

- **Source file**: `../hello.c` (not modified in safe mode)
- **Workspace file**: `hello.c.collab` (default editing target)
- **Backups**: `backups/` directory (up to 10 versions kept)
- **Server**: `server.js`
- **Editor**: `editor.html`
- **Tests**: `test.js`

## Features

### Safety Features
- ✓ Workspace/sandbox mode by default
- ✓ Localhost-only binding by default
- ✓ Input validation (type, bounds, size limits)
- ✓ Rate limiting (100 req/15min per IP, 100 ops/min per socket)
- ✓ Connection limits (max 50 concurrent users)
- ✓ Atomic file writes (temp file + rename)
- ✓ Automatic versioned backups

### Collaborative Features
- Real-time synchronization via WebSocket
- Operational Transform for conflict resolution
- Live user presence indicators
- Version tracking

## Security Best Practices

If you must use this in a production-like environment:

1. **Add authentication**: Implement proper user authentication
2. **Use HTTPS/WSS**: Enable TLS encryption
3. **Firewall**: Restrict network access
4. **Audit logs**: Log all operations
5. **Input sanitization**: Already implemented, but review
6. **Regular backups**: Backups are automatic, but test recovery
7. **Monitoring**: Add monitoring and alerting
8. **Security review**: Have a security expert review the code

## Troubleshooting

### Port already in use
```bash
PORT=8080 npm start
```

### Cannot write to file
- Check file permissions
- Ensure directory is writable
- Check disk space

### Connection refused
- Server binds to localhost by default
- Use `--bind-external` for network access (with caution)

## Architecture

```
User Browser 1 ←→ WebSocket ←→ Server ←→ hello.c.collab (or hello.c)
User Browser 2 ←→ WebSocket ←→   ↑
                                  ↓
                              Backups
```

## Limitations

- No authentication (must be added for production)
- No encryption (must use HTTPS/WSS for production)
- Basic OT algorithm (works for most cases, but not formally verified)
- File-based persistence (no database)
- Single-file editing only

## For Production Use

This is **NOT production-ready** as-is. Required additions:
- User authentication and session management
- HTTPS/TLS certificate configuration
- Database-backed persistence
- Comprehensive audit logging
- Security hardening and penetration testing
- Load balancing and high availability
- Backup and disaster recovery procedures

See `../COLLAB_DOCS.md` for more detailed documentation.
