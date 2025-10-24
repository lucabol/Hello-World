# Security and Deployment Guide

## ⚠️ SECURITY STATUS: EXPERIMENTAL

**This collaborative editor is a prototype and should NOT be deployed to production without additional hardening.**

## Default Security Posture

### ✅ Secure Defaults

The server is configured with security-first defaults:

1. **Localhost Binding**: Server binds to `127.0.0.1` (localhost) by default, preventing external access
2. **Repository Protection**: Writing to repository files is disabled by default
3. **Configurable Target**: Target file path must be explicitly configured
4. **Rate Limiting**: Built-in rate limiting (30 edits/minute per client)
5. **Input Validation**: Message size limits, JSON validation, type checking
6. **XSS Prevention**: Content Security Policy headers, username sanitization
7. **Atomic Writes**: File writes use temp file + rename for atomicity

### ⚠️ Current Limitations

1. **No Authentication**: Authentication is optional and disabled by default
2. **Unencrypted WebSocket**: Uses `ws://` not `wss://` (requires reverse proxy for TLS)
3. **Simple Conflict Resolution**: Last-write-wins may lose concurrent edits
4. **No Audit Log**: No persistent logging of user actions
5. **No Access Control**: All authenticated users have full read/write access

## Configuration

All configuration is done via environment variables:

### Required Configuration

```bash
# Target file to edit (required)
export COLLAB_TARGET_FILE=/path/to/your/file.c

# If target is in repository, must explicitly enable
export COLLAB_ALLOW_REPO_WRITE=true
```

### Security Configuration

```bash
# Authentication token (HIGHLY RECOMMENDED)
export COLLAB_AUTH_TOKEN="your-secret-token-here"

# Server binding (default: 127.0.0.1 for localhost-only)
export COLLAB_HOST=127.0.0.1

# Port (default: 3000)
export COLLAB_PORT=3000
```

### Performance Configuration

```bash
# Maximum message size in bytes (default: 1MB)
export COLLAB_MAX_MESSAGE_SIZE=1048576

# Maximum file size in bytes (default: 1MB)
export COLLAB_MAX_FILE_SIZE=1048576

# Rate limit: max edits per minute (default: 30)
export COLLAB_RATE_LIMIT=30

# Log level: error, warn, info, debug (default: info)
export COLLAB_LOG_LEVEL=info
```

## Running Safely

### Local Development (Safe)

```bash
# Default configuration - localhost only, no repo write
cd collaborative-editor
npm install
npm start

# Access at http://127.0.0.1:3000
```

### With Repository Write (Use with Caution)

```bash
# Enable writing to repository files
export COLLAB_ALLOW_REPO_WRITE=true
export COLLAB_TARGET_FILE=/path/to/repo/hello.c
npm start
```

### With Authentication

```bash
# Generate a secure token
export COLLAB_AUTH_TOKEN=$(openssl rand -hex 32)

# Start server
npm start

# Connect with token
# In browser: http://127.0.0.1:3000?token=YOUR_TOKEN
# Or set X-Auth-Token header
```

### Behind Reverse Proxy (Recommended for Network Access)

Use nginx or Apache as a reverse proxy to add:
- TLS/SSL encryption (wss://)
- Additional authentication
- Rate limiting
- IP filtering

Example nginx configuration:

```nginx
server {
    listen 443 ssl http2;
    server_name editor.example.com;
    
    ssl_certificate /path/to/cert.pem;
    ssl_certificate_key /path/to/key.pem;
    
    # Security headers
    add_header Strict-Transport-Security "max-age=31536000" always;
    add_header X-Frame-Options "DENY" always;
    add_header X-Content-Type-Options "nosniff" always;
    
    # Proxy to collaborative editor
    location / {
        proxy_pass http://127.0.0.1:3000;
        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection "upgrade";
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto $scheme;
    }
    
    # WebSocket support
    location /ws {
        proxy_pass http://127.0.0.1:3000;
        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection "upgrade";
    }
}
```

## Security Checklist for Production

Before deploying to production, ensure you have:

- [ ] Set `COLLAB_AUTH_TOKEN` to a strong, random token
- [ ] Configured TLS/SSL (use reverse proxy with valid certificates)
- [ ] Set up proper firewall rules
- [ ] Configured appropriate rate limits
- [ ] Set up monitoring and alerting
- [ ] Implemented proper backup strategy for target file
- [ ] Tested disaster recovery procedures
- [ ] Set up structured logging to persistent storage
- [ ] Configured log rotation
- [ ] Implemented access control based on your requirements
- [ ] Reviewed and understood the conflict resolution strategy
- [ ] Tested the system under load
- [ ] Set up health monitoring (`/api/health` endpoint)

## Known Security Issues

### 1. Authentication is Optional
**Risk**: Anyone who can reach the server can edit files  
**Mitigation**: Always set `COLLAB_AUTH_TOKEN` and use TLS

### 2. Simple Conflict Resolution
**Risk**: Last-write-wins can lose concurrent edits  
**Mitigation**: Educate users, use for low-conflict scenarios, or implement OT/CRDT

### 3. No Fine-grained Access Control
**Risk**: All users have same permissions  
**Mitigation**: Implement custom authentication/authorization layer

### 4. No Persistent Audit Log
**Risk**: Can't track who made what changes  
**Mitigation**: Implement custom logging to database or log aggregation service

### 5. No Undo/Versioning
**Risk**: Mistakes can't be easily reverted  
**Mitigation**: Implement external version control or backup strategy

## Testing Security

### Test Rate Limiting

```bash
# Send rapid requests (should be rate limited)
for i in {1..50}; do
    curl -X POST http://127.0.0.1:3000/api/content &
done
```

### Test Message Size Limits

```bash
# Generate large payload (should be rejected)
dd if=/dev/zero bs=2M count=1 | base64 > /tmp/large.txt
curl -X POST -d @/tmp/large.txt http://127.0.0.1:3000/api/content
```

### Test XSS Prevention

```javascript
// In browser console, try setting malicious username
// Should be sanitized on server
const ws = new WebSocket('ws://127.0.0.1:3000');
ws.onopen = () => {
    ws.send(JSON.stringify({
        type: 'set-name',
        name: '<script>alert("XSS")</script>'
    }));
};
```

### Test Authentication

```bash
# Without token (should fail if AUTH_TOKEN is set)
curl http://127.0.0.1:3000/

# With token (should succeed)
curl http://127.0.0.1:3000/?token=YOUR_TOKEN
```

## Incident Response

If you suspect a security incident:

1. **Immediately stop the server**: `pkill -f "node server.js"`
2. **Review logs**: Check for suspicious activity
3. **Check file integrity**: Verify target file hasn't been maliciously modified
4. **Rotate credentials**: Change `COLLAB_AUTH_TOKEN`
5. **Update access controls**: Review and restrict network access
6. **Investigate**: Determine scope and impact
7. **Restore from backup**: If file was modified maliciously

## Reporting Security Issues

Security vulnerabilities should be reported privately to the repository maintainers. Do not open public issues for security problems.

## Future Security Enhancements

Planned improvements:

1. **Multi-factor Authentication**: Add MFA support
2. **Role-based Access Control**: Different permission levels
3. **Audit Logging**: Persistent log of all actions
4. **Operational Transformation**: Better conflict resolution
5. **File Locking**: Prevent concurrent edits to same section
6. **Rate Limiting per Session**: More sophisticated rate limiting
7. **IP Whitelisting**: Restrict access by IP address
8. **Session Management**: Proper session handling with expiration
9. **Content Validation**: Validate file content matches expected format

## Compliance Considerations

This system does NOT currently meet requirements for:

- HIPAA (healthcare data)
- PCI DSS (payment card data)
- SOC 2 (enterprise security)
- GDPR (personal data protection) - without additional measures

Consult security and compliance experts before using with sensitive data.

## Additional Resources

- [OWASP WebSocket Security](https://cheatsheetseries.owasp.org/cheatsheets/WebSocket_Security_Cheat_Sheet.html)
- [Node.js Security Best Practices](https://nodejs.org/en/docs/guides/security/)
- [Express Security Best Practices](https://expressjs.com/en/advanced/best-practice-security.html)
- [Content Security Policy Guide](https://developer.mozilla.org/en-US/docs/Web/HTTP/CSP)
