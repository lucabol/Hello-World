# Security and Threat Model

## Overview

The collaborative editor allows multiple users to edit hello.c in real-time with build and execution capabilities. This document outlines the security measures, threat model, and mitigations implemented.

## Threat Model

### Assets Protected
- **Source Code**: hello.c file content and integrity
- **Server Resources**: CPU, memory, disk, network bandwidth
- **User Data**: Minimal - only temporary session data (usernames, edit history)
- **System Files**: File system access beyond project directory

### Threat Actors
- **Malicious Users**: Users attempting to exploit build/run functionality
- **Resource Exhaustion**: Users consuming excessive server resources
- **Network Attackers**: Attempting to exploit WebSocket protocol vulnerabilities

### Attack Vectors

#### 1. Code Injection via Build/Run (HIGH RISK)
**Threat**: Users submit malicious C code that attempts to exploit the compilation or execution environment.

**Mitigations**:
- ✅ **Sandboxed execution**: Build and run operations use restricted environment variables
- ✅ **Timeout protection**: 15s build timeout, 8s execution timeout  
- ✅ **Resource limits**: 1MB build output, 64KB runtime output limits
- ✅ **Command isolation**: Uses `timeout` command for additional protection
- ✅ **Non-privileged execution**: Runs as current user (non-root in container)
- ❌ **MISSING**: Container/chroot isolation (see Future Enhancements)

#### 2. File System Access (MEDIUM RISK)
**Threat**: Users attempt to read/write files outside project directory.

**Mitigations**:
- ✅ **Restricted PATH**: Limited to `/usr/bin:/bin`
- ✅ **Working directory constraint**: All operations in project directory
- ✅ **File validation**: Only hello.c can be modified
- ❌ **MISSING**: Proper chroot jail (see Future Enhancements)

#### 3. Resource Exhaustion (MEDIUM RISK)
**Threat**: Users consume excessive CPU, memory, or network resources.

**Mitigations**:
- ✅ **Rate limiting**: 30 operations per minute per user
- ✅ **User limits**: Maximum 10 concurrent users
- ✅ **Content size limits**: 100KB maximum file size
- ✅ **Process timeouts**: Build and execution timeouts
- ✅ **Output limits**: Bounded stdout/stderr capture

#### 4. WebSocket Protocol Attacks (LOW-MEDIUM RISK)
**Threat**: Malformed messages, connection flooding, message injection.

**Mitigations**:
- ✅ **Input validation**: All operations validated before processing
- ✅ **Message size limits**: JSON parsing with size constraints
- ✅ **Connection limits**: Maximum user limit prevents connection flooding
- ✅ **Error handling**: Graceful handling of malformed messages

#### 5. Operational Transformation Attacks (LOW RISK)
**Threat**: Malicious edit sequences to corrupt file state.

**Mitigations**:
- ✅ **Operation validation**: Position and content validation
- ✅ **Version tracking**: Prevents replay attacks
- ✅ **Bounded history**: Limited operation history to prevent memory exhaustion

## Security Controls Implemented

### Input Validation
```javascript
// All operations validated for type, position, content size
validateOperation(operation) {
    // Type validation
    // Position bounds checking  
    // Content size limits
    // Length validation for deletes
}
```

### Rate Limiting
```javascript
// 30 operations per minute per user
checkRateLimit(userId) {
    // Time-window based limiting
    // Per-user tracking
    // Configurable limits
}
```

### Sandboxed Execution
```javascript
// Restricted environment for build/run
execAsync('timeout 10s gcc ...', {
    env: { PATH: '/usr/bin:/bin', HOME: '/tmp' },
    timeout: 15000,
    maxBuffer: 1024 * 1024
})
```

### Resource Limits
- **Max Users**: 10 concurrent (configurable)
- **Rate Limit**: 30 ops/minute (configurable) 
- **File Size**: 100KB maximum (configurable)
- **Build Timeout**: 15 seconds (configurable)
- **Run Timeout**: 8 seconds (configurable)
- **Output Limits**: 1MB build, 64KB runtime

## Configuration Security

### Environment Variables
```bash
COLLAB_MAX_USERS=5          # Reduce concurrent users
COLLAB_RATE_LIMIT=10        # Stricter rate limiting
COLLAB_BUILD_TIMEOUT=5000   # Shorter build timeout
COLLAB_RUN_TIMEOUT=2000     # Shorter run timeout
```

### Deployment Security
- **Non-root execution**: Never run as root user
- **Network isolation**: Use firewall rules to restrict access
- **Container deployment**: Deploy in Docker container with limited capabilities
- **TLS termination**: Use reverse proxy with HTTPS/WSS

## Monitoring and Logging

### Health Checks
- `/health` endpoint reports server status
- Active user count monitoring
- Resource utilization tracking

### Security Events Logged
- Rate limit violations
- Invalid operation attempts  
- Build/run failures and timeouts
- Connection attempts and failures
- File save operations

## Future Security Enhancements

### HIGH PRIORITY
1. **Container Isolation**: Deploy in Docker with limited capabilities
2. **Proper Sandboxing**: Use firejail, bubblewrap, or similar
3. **File System Isolation**: Implement proper chroot jail
4. **Authentication**: Add user authentication system

### MEDIUM PRIORITY  
5. **TLS/WSS Encryption**: Encrypt all communications
6. **Audit Logging**: Comprehensive security event logging
7. **Resource Monitoring**: Real-time resource usage alerts
8. **Network Segmentation**: Isolate build environment

### LOW PRIORITY
9. **Code Analysis**: Static analysis before compilation
10. **Vulnerability Scanning**: Regular dependency scanning
11. **Intrusion Detection**: Anomaly detection for user behavior

## Production Deployment Guidelines

### Minimum Security Requirements
1. **Deploy in container** with limited Linux capabilities
2. **Run as non-privileged user** (not root)
3. **Enable TLS/HTTPS** for all communications  
4. **Configure firewall** to restrict access
5. **Monitor resource usage** and set alerts
6. **Regular security updates** of dependencies

### Recommended Security Stack
```bash
# Docker deployment with security
docker run -d \
    --name collab-editor \
    --user 1000:1000 \
    --memory=512m \
    --cpus="0.5" \
    --cap-drop=ALL \
    --read-only \
    --tmpfs /tmp \
    -e COLLAB_MAX_USERS=5 \
    -e COLLAB_RATE_LIMIT=10 \
    -p 8082:8082 \
    hello-world-collab
```

## Incident Response

### Security Incident Types
1. **Resource exhaustion**: Monitor CPU/memory usage
2. **Malicious code execution**: Check build/run logs  
3. **File corruption**: Verify hello.c integrity
4. **Connection flooding**: Monitor connection counts

### Response Procedures
1. **Immediate**: Stop server if under active attack
2. **Investigation**: Review logs for attack vectors
3. **Recovery**: Restore hello.c from backup if corrupted
4. **Prevention**: Update security controls based on findings

## Risk Assessment Summary

| Threat | Likelihood | Impact | Risk Level | Mitigations |
|--------|------------|--------|------------|-------------|
| Code Injection | Medium | High | **HIGH** | Sandboxing, timeouts |
| File System Access | Low | High | Medium | Path restrictions |
| Resource Exhaustion | High | Medium | Medium | Rate limits, timeouts |
| Protocol Attacks | Medium | Low | Low | Input validation |
| OT Corruption | Low | Low | Low | Operation validation |

## Compliance Notes

This system processes minimal user data (temporary session info only) and does not store persistent user information. However, for production deployment consider:

- **Data Protection**: Ensure user session data handling complies with local privacy laws
- **Access Control**: Implement proper authentication for regulated environments  
- **Audit Requirements**: Enhanced logging may be required for compliance
- **Security Standards**: Consider ISO 27001, NIST frameworks for enterprise deployment

## Conclusion

The current implementation provides basic security controls suitable for development and trusted environments. For production deployment, additional containerization, authentication, and monitoring controls are strongly recommended.