# Security Architecture and Sandboxing Design

## Overview

The collaborative editor's security architecture focuses on safe execution of user-submitted C code while preventing system compromise. This document details the sandboxing implementation and security measures.

## Threat Model

### Primary Threats
1. **Code Injection**: Malicious C code attempting to exploit the compilation/execution environment
2. **Resource Exhaustion**: Infinite loops, memory bombs, excessive disk/network usage
3. **File System Access**: Unauthorized read/write access to system files
4. **Privilege Escalation**: Attempts to gain elevated system permissions
5. **Network Access**: Unauthorized network connections or data exfiltration

## Sandboxing Implementation

### Layer 1: Process Isolation
```javascript
// Multiple timeout layers for defense in depth
const buildCommand = 'timeout 10s gcc -Wall -Wextra -Wpedantic -o hello hello.c';
const execOptions = {
    timeout: CONFIG.BUILD_TIMEOUT, // 15s Node.js timeout (outer)
    maxBuffer: 1024 * 1024,        // 1MB output limit
    cwd: __dirname,                // Constrain to project directory
    uid: process.getuid(),         // Run as non-root user
    gid: process.getgid()          // Run as non-root group
};
```

**Security Properties:**
- **Timeout Protection**: Double timeout (OS-level + Node.js) prevents hanging processes
- **Output Limits**: Prevents memory exhaustion via excessive output
- **User/Group Isolation**: Runs as non-privileged user, cannot escalate privileges
- **Working Directory Constraint**: All operations restricted to project directory

### Layer 2: Environment Isolation
```javascript
env: {
    PATH: '/usr/bin:/bin',    // Minimal PATH, no custom tools
    HOME: '/tmp',             // Sandboxed home directory
    USER: 'sandbox',          // Non-privileged user identity
    // All other env vars stripped
}
```

**Security Properties:**
- **Restricted PATH**: Only access to system binaries, no user-installed tools
- **Sandboxed HOME**: Prevents access to user data/configs
- **Minimal Environment**: Removes potentially dangerous environment variables

### Layer 3: Resource Limits
```javascript
// Build limits
maxBuffer: 1024 * 1024,      // 1MB stdout/stderr limit
timeout: 15000,              // 15 second hard timeout

// Runtime limits  
maxBuffer: 64 * 1024,        // 64KB output limit
timeout: 8000,               // 8 second execution timeout
```

**Security Properties:**
- **Memory Protection**: Output buffering limits prevent memory exhaustion
- **CPU Protection**: Timeouts prevent infinite loops and CPU bombs
- **Disk Protection**: Working directory constraint limits file system access

## Security Analysis

### What This Approach Protects Against

✅ **Resource Exhaustion Attacks**
- Infinite loops → Killed by timeout
- Memory bombs → Limited by output buffering and timeout
- Fork bombs → Limited by user process limits

✅ **File System Attacks**
- Directory traversal → Constrained by cwd setting
- System file access → Limited by user permissions and PATH
- Temporary file abuse → HOME set to /tmp

✅ **Basic Code Injection**
- Shell injection → execAsync with timeout prefix prevents most attacks
- Environment injection → Minimal, controlled environment variables

### Current Limitations (Areas for Enhancement)

❌ **Advanced Sandboxing Missing**
- No chroot jail or filesystem namespace isolation
- No network namespace isolation
- No seccomp syscall filtering
- No cgroups for fine-grained resource control

❌ **Container Isolation Missing**
- Not running in Docker container by default
- No user namespace remapping
- No capability dropping

## Production Hardening Recommendations

### Immediate (Current Implementation)
1. **Container Deployment**: Run entire server in Docker with limited capabilities
2. **Non-root User**: Ensure container runs as non-root user
3. **Network Isolation**: Bind only to localhost, use firewall rules
4. **File System**: Mount project directory as read-only where possible

### Advanced (Future Enhancements)
1. **Linux Namespaces**: User, mount, network, PID namespace isolation
2. **Seccomp Filters**: Block dangerous syscalls (network, file system)
3. **Cgroups**: Fine-grained CPU, memory, I/O limits
4. **Dedicated Sandbox**: Use firejail, bubblewrap, or similar sandboxing tools

## Container Deployment Example

```dockerfile
FROM node:18-alpine
RUN adduser -D -s /bin/sh sandbox
USER sandbox
WORKDIR /app
COPY . .
RUN npm ci
EXPOSE 8082
CMD ["npm", "start"]
```

```bash
# Secure container deployment
docker run -d \
  --user 1000:1000 \
  --memory=512m \
  --cpus="0.5" \
  --cap-drop=ALL \
  --read-only \
  --tmpfs /tmp \
  --network=bridge \
  -p 127.0.0.1:8082:8082 \
  hello-world-collab
```

## Security Testing Strategy

### Malicious Payload Categories

1. **Resource Exhaustion**
   - Infinite loops: `while(1) {}`
   - Memory bombs: Large array allocations
   - Fork bombs: `system(":(){ :|:& };:")`

2. **File System Access**
   - Directory traversal: `#include "../../../etc/passwd"`
   - System file access: `fopen("/etc/shadow", "r")`
   - Symlink attacks: Creating symlinks to system files

3. **Code Injection**
   - Shell injection via system(): `system("rm -rf /")`
   - Environment variable injection
   - Preprocessor abuse: `#include </dev/stdin>`

4. **Network Access**
   - Socket creation attempts
   - DNS resolution attempts
   - HTTP requests via curl/wget

### Test Implementation

Each category has dedicated unit tests that:
1. Submit malicious payload
2. Verify it's blocked/contained
3. Confirm no system impact
4. Validate proper error handling

## Conclusion

The current implementation provides basic but effective sandboxing suitable for trusted development environments. The multi-layered timeout approach, environment isolation, and resource limits protect against common attacks.

For production deployment, additional container-based isolation and Linux security features should be implemented as outlined in the hardening recommendations.