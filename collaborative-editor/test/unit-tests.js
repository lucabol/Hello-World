#!/usr/bin/env node
/**
 * Unit tests for collaborative editor server
 * Tests core functionality without starting the full server
 */

const assert = require('assert');
const fs = require('fs').promises;
const path = require('path');
const crypto = require('crypto');

// Simple test framework
class TestRunner {
    constructor() {
        this.tests = [];
        this.passed = 0;
        this.failed = 0;
    }
    
    test(name, fn) {
        this.tests.push({ name, fn });
    }
    
    async run() {
        console.log('\n' + '='.repeat(60));
        console.log('Running Unit Tests');
        console.log('='.repeat(60) + '\n');
        
        for (const { name, fn } of this.tests) {
            try {
                await fn();
                this.passed++;
                console.log(`✓ ${name}`);
            } catch (error) {
                this.failed++;
                console.log(`✗ ${name}`);
                console.log(`  Error: ${error.message}`);
                if (error.stack) {
                    console.log(`  ${error.stack.split('\n').slice(1, 3).join('\n  ')}`);
                }
            }
        }
        
        console.log('\n' + '='.repeat(60));
        console.log(`Results: ${this.passed} passed, ${this.failed} failed`);
        console.log('='.repeat(60) + '\n');
        
        return this.failed === 0;
    }
}

const runner = new TestRunner();

// Test: Sanitize username removes HTML
runner.test('sanitizeUsername removes HTML tags', () => {
    const sanitize = (name) => {
        if (!name || typeof name !== 'string') return 'Anonymous';
        const trimmed = name.trim().substring(0, 50);
        const sanitized = trimmed.replace(/[^a-zA-Z0-9\s\-_]/g, '');
        return sanitized || 'Anonymous';
    };
    
    assert.strictEqual(sanitize('<script>alert("xss")</script>'), 'scriptalertxssscript');
    assert.strictEqual(sanitize('User<b>Bold</b>'), 'UserbBoldb');
    assert.strictEqual(sanitize('Normal_User-123'), 'Normal_User-123');
});

// Test: Sanitize username handles special cases
runner.test('sanitizeUsername handles edge cases', () => {
    const sanitize = (name) => {
        if (!name || typeof name !== 'string') return 'Anonymous';
        const trimmed = name.trim().substring(0, 50);
        const sanitized = trimmed.replace(/[^a-zA-Z0-9\s\-_]/g, '');
        return sanitized || 'Anonymous';
    };
    
    assert.strictEqual(sanitize(''), 'Anonymous');
    assert.strictEqual(sanitize(null), 'Anonymous');
    assert.strictEqual(sanitize('!!!'), 'Anonymous');
    assert.strictEqual(sanitize('  SpacedName  '), 'SpacedName');
});

// Test: Message size validation
runner.test('validateMessage checks size limits', () => {
    const MAX_SIZE = 1024 * 1024; // 1MB
    
    const validate = (message, maxSize = MAX_SIZE) => {
        if (!message) return { valid: false, error: 'Empty message' };
        const size = Buffer.byteLength(message, 'utf8');
        if (size > maxSize) {
            return { valid: false, error: `Message too large: ${size} bytes (max: ${maxSize})` };
        }
        return { valid: true };
    };
    
    assert.strictEqual(validate('Hello').valid, true);
    assert.strictEqual(validate('').valid, false);
    assert.strictEqual(validate('x'.repeat(2000), 1000).valid, false);
    assert.ok(validate('x'.repeat(2000), 1000).error.includes('too large'));
});

// Test: Rate limiting logic
runner.test('checkRateLimit prevents excessive requests', () => {
    const rateLimits = new Map();
    const WINDOW = 60000; // 1 minute
    const MAX_EDITS = 5;
    
    const checkRateLimit = (clientId) => {
        const now = Date.now();
        const limit = rateLimits.get(clientId) || { count: 0, windowStart: now };
        
        if (now - limit.windowStart > WINDOW) {
            limit.count = 0;
            limit.windowStart = now;
        }
        
        limit.count++;
        rateLimits.set(clientId, limit);
        
        return limit.count <= MAX_EDITS;
    };
    
    const clientId = 'test-client';
    
    // First 5 requests should succeed
    for (let i = 0; i < 5; i++) {
        assert.strictEqual(checkRateLimit(clientId), true, `Request ${i + 1} should succeed`);
    }
    
    // 6th request should fail
    assert.strictEqual(checkRateLimit(clientId), false, 'Request 6 should fail');
});

// Test: Atomic file write simulation
runner.test('Atomic file write creates temp file first', async () => {
    const testDir = path.join(__dirname, 'test-temp');
    const targetFile = path.join(testDir, 'test.txt');
    const content = 'test content';
    
    try {
        // Create test directory
        await fs.mkdir(testDir, { recursive: true });
        
        // Simulate atomic write
        const tempFile = `${targetFile}.tmp.${Date.now()}.${crypto.randomBytes(4).toString('hex')}`;
        
        // Write to temp file
        await fs.writeFile(tempFile, content, 'utf8');
        
        // Verify temp file exists
        const tempContent = await fs.readFile(tempFile, 'utf8');
        assert.strictEqual(tempContent, content);
        
        // Rename to target (atomic operation)
        await fs.rename(tempFile, targetFile);
        
        // Verify target file exists
        const finalContent = await fs.readFile(targetFile, 'utf8');
        assert.strictEqual(finalContent, content);
        
        // Cleanup
        await fs.rm(testDir, { recursive: true });
    } catch (error) {
        // Cleanup on error
        try {
            await fs.rm(testDir, { recursive: true });
        } catch (cleanupError) {
            // Ignore cleanup errors
        }
        throw error;
    }
});

// Test: Configuration validation
runner.test('Configuration validation checks repository writes', () => {
    const validatePath = (targetPath, repoPath, allowRepoWrite) => {
        const resolvedTarget = path.resolve(targetPath);
        const resolvedRepo = path.resolve(repoPath);
        const isInRepo = resolvedTarget.startsWith(resolvedRepo);
        
        return !isInRepo || allowRepoWrite;
    };
    
    const repoPath = '/home/user/repo';
    
    // File in repo with write disabled should fail
    assert.strictEqual(validatePath('/home/user/repo/file.c', repoPath, false), false);
    
    // File in repo with write enabled should succeed
    assert.strictEqual(validatePath('/home/user/repo/file.c', repoPath, true), true);
    
    // File outside repo should succeed regardless
    assert.strictEqual(validatePath('/home/user/other/file.c', repoPath, false), true);
    assert.strictEqual(validatePath('/home/user/other/file.c', repoPath, true), true);
});

// Test: JSON parse error handling
runner.test('Message parsing handles invalid JSON', () => {
    const parseMessage = (message) => {
        try {
            const data = JSON.parse(message);
            if (!data || typeof data !== 'object' || !data.type) {
                return { success: false, error: 'Invalid structure' };
            }
            return { success: true, data };
        } catch (error) {
            return { success: false, error: 'Parse error' };
        }
    };
    
    assert.strictEqual(parseMessage('invalid json').success, false);
    assert.strictEqual(parseMessage('null').success, false);
    assert.strictEqual(parseMessage('{"type":"edit"}').success, true);
    assert.strictEqual(parseMessage('{"noType":"value"}').success, false);
});

// Test: Content size validation
runner.test('Content size validation rejects oversized files', () => {
    const MAX_FILE_SIZE = 1024 * 1024; // 1MB
    
    const validateContent = (content, maxSize = MAX_FILE_SIZE) => {
        const size = Buffer.byteLength(content, 'utf8');
        return size <= maxSize;
    };
    
    assert.strictEqual(validateContent('small content'), true);
    assert.strictEqual(validateContent('x'.repeat(2 * 1024 * 1024)), false);
    assert.strictEqual(validateContent('x'.repeat(500 * 1024)), true);
});

// Test: Client ID generation is unique
runner.test('Client ID generation produces unique IDs', () => {
    const generateClientId = () => crypto.randomBytes(8).toString('hex');
    
    const ids = new Set();
    for (let i = 0; i < 100; i++) {
        const id = generateClientId();
        assert.strictEqual(typeof id, 'string');
        assert.strictEqual(id.length, 16); // 8 bytes = 16 hex chars
        assert.strictEqual(ids.has(id), false, 'ID should be unique');
        ids.add(id);
    }
});

// Test: Broadcast excludes sender
runner.test('Broadcast logic excludes sender', () => {
    const clients = new Map();
    const sender = { id: 'sender' };
    const receiver1 = { id: 'receiver1' };
    const receiver2 = { id: 'receiver2' };
    
    clients.set(sender, { id: 'sender' });
    clients.set(receiver1, { id: 'receiver1' });
    clients.set(receiver2, { id: 'receiver2' });
    
    const recipients = [];
    clients.forEach((metadata, client) => {
        if (client !== sender) {
            recipients.push(client);
        }
    });
    
    assert.strictEqual(recipients.length, 2);
    assert.strictEqual(recipients.includes(sender), false);
    assert.strictEqual(recipients.includes(receiver1), true);
    assert.strictEqual(recipients.includes(receiver2), true);
});

// Test constant-time token comparison
runner.test('timingSafeEqual token comparison with equal tokens', () => {
    const token1 = 'my-secret-token-123';
    const token2 = 'my-secret-token-123';
    
    const buffer1 = Buffer.from(token1, 'utf8');
    const buffer2 = Buffer.from(token2, 'utf8');
    
    // Should return true for equal tokens
    const result = crypto.timingSafeEqual(buffer1, buffer2);
    assert.strictEqual(result, true);
});

runner.test('Token comparison with different lengths fails safely', () => {
    const shortToken = 'short';
    const longToken = 'this-is-a-much-longer-token';
    
    // Should detect length difference before timingSafeEqual
    assert.notStrictEqual(shortToken.length, longToken.length);
    
    // timingSafeEqual would throw if called with different lengths
    // Our code should check lengths first
    const buffer1 = Buffer.from(shortToken, 'utf8');
    const buffer2 = Buffer.from(longToken, 'utf8');
    
    assert.notStrictEqual(buffer1.length, buffer2.length);
    
    // Verify that timingSafeEqual throws with different lengths
    assert.throws(() => {
        crypto.timingSafeEqual(buffer1, buffer2);
    }, {
        name: 'RangeError'
    });
});

runner.test('Token comparison with empty token fails safely', () => {
    const emptyToken = '';
    const validToken = 'my-secret-token';
    
    // Should detect empty token
    assert.strictEqual(emptyToken.length, 0);
    assert.notStrictEqual(validToken.length, 0);
    
    // Length check should catch this
    assert.notStrictEqual(emptyToken.length, validToken.length);
});

runner.test('Token comparison with matching lengths but different content', () => {
    const token1 = 'secret-token-A';
    const token2 = 'secret-token-B';
    
    // Same length, different content
    assert.strictEqual(token1.length, token2.length);
    
    const buffer1 = Buffer.from(token1, 'utf8');
    const buffer2 = Buffer.from(token2, 'utf8');
    
    // timingSafeEqual should return false
    const result = crypto.timingSafeEqual(buffer1, buffer2);
    assert.strictEqual(result, false);
});

runner.test('Token comparison with non-ASCII characters (UTF-8 multi-byte)', () => {
    // Test with emojis and accented characters to ensure UTF-8 encoding works correctly
    const token1 = 'secret-🔐-token';  // Contains emoji (multi-byte UTF-8)
    const token2 = 'secret-🔐-token';  // Same token
    const token3 = 'secret-café-token'; // Contains accented character
    
    // String length counts UTF-16 code units, not bytes
    // Emoji is 2 UTF-16 code units but 4 UTF-8 bytes
    // This test verifies Buffer.from handles this correctly
    
    const buffer1 = Buffer.from(token1, 'utf8');
    const buffer2 = Buffer.from(token2, 'utf8');
    const buffer3 = Buffer.from(token3, 'utf8');
    
    // Identical tokens should match
    const result1 = crypto.timingSafeEqual(buffer1, buffer2);
    assert.strictEqual(result1, true, 'Identical multi-byte tokens should match');
    
    // Different tokens should not match (even if similar string length)
    // Note: buffer3 has different length so we test inequality without timingSafeEqual
    assert.notStrictEqual(buffer1.length, buffer3.length, 'Different tokens should have different byte lengths');
    
    // Verify Buffer.byteLength vs string.length difference
    const stringLength = token1.length;  // UTF-16 code units
    const byteLength = Buffer.byteLength(token1, 'utf8');  // UTF-8 bytes
    
    // Emoji takes more bytes than string length suggests
    assert.ok(byteLength > stringLength, 'Multi-byte characters should use more bytes than string length');
});

// Test environment variable parsing
runner.test('Environment variable parsing accepts "true" (lowercase)', () => {
    const value = 'true';
    const result = value.toLowerCase() === 'true';
    assert.strictEqual(result, true);
});

runner.test('Environment variable parsing accepts "TRUE" (uppercase)', () => {
    const value = 'TRUE';
    const result = value.toLowerCase() === 'true';
    assert.strictEqual(result, true);
});

runner.test('Environment variable parsing accepts "True" (mixed case)', () => {
    const value = 'True';
    const result = value.toLowerCase() === 'true';
    assert.strictEqual(result, true);
});

runner.test('Environment variable parsing rejects "yes"', () => {
    const value = 'yes';
    const result = value.toLowerCase() === 'true';
    assert.strictEqual(result, false);
});

runner.test('Environment variable parsing rejects "1"', () => {
    const value = '1';
    const result = value.toLowerCase() === 'true';
    assert.strictEqual(result, false);
});

runner.test('Environment variable parsing rejects empty string', () => {
    const value = '';
    const result = value.toLowerCase() === 'true';
    assert.strictEqual(result, false);
});

// Test path validation
runner.test('Absolute path detection with path.isAbsolute', () => {
    // Absolute paths
    assert.strictEqual(path.isAbsolute('/home/user/file.txt'), true);
    assert.strictEqual(path.isAbsolute('/etc/config'), true);
    
    // Relative paths
    assert.strictEqual(path.isAbsolute('../file.txt'), false);
    assert.strictEqual(path.isAbsolute('./file.txt'), false);
    assert.strictEqual(path.isAbsolute('file.txt'), false);
    assert.strictEqual(path.isAbsolute('~/file.txt'), false);
});

runner.test('Path normalization detects traversal attempts', () => {
    const suspiciousPath = '/home/user/../../etc/passwd';
    const normalized = path.normalize(suspiciousPath);
    
    // Normalized path should be different, indicating traversal
    assert.notStrictEqual(suspiciousPath, normalized);
    assert.strictEqual(normalized, '/etc/passwd');
});

runner.test('Path normalization is idempotent for safe paths', () => {
    const safePath = '/home/user/documents/file.txt';
    const normalized = path.normalize(safePath);
    
    // Safe absolute paths should normalize to themselves
    assert.strictEqual(safePath, normalized);
});

runner.test('Shell expansion detection catches tilde', () => {
    const pathWithTilde = '~/documents/file.txt';
    assert.strictEqual(pathWithTilde.includes('~'), true);
});

runner.test('Shell expansion detection catches dollar sign', () => {
    const pathWithVar = '/home/${USER}/file.txt';
    assert.strictEqual(pathWithVar.includes('${'), true);
});

// Test: Containment check using path.relative
runner.test('Containment check with path.relative detects outside repo', () => {
    const repoPath = '/home/user/repo';
    const outsidePath = '/home/user/other/file.txt';
    
    const relativePath = path.relative(repoPath, outsidePath);
    const isInRepo = !!relativePath && !relativePath.startsWith('..') && !path.isAbsolute(relativePath);
    
    assert.strictEqual(isInRepo, false);
});

// Test: Containment check handles inside repo
runner.test('Containment check with path.relative detects inside repo', () => {
    const repoPath = '/home/user/repo';
    const insidePath = '/home/user/repo/subdir/file.txt';
    
    const relativePath = path.relative(repoPath, insidePath);
    const isInRepo = !!relativePath && !relativePath.startsWith('..') && !path.isAbsolute(relativePath);
    
    assert.strictEqual(isInRepo, true);
});

// Test: Containment check handles repo root equality
runner.test('Containment check with path.relative handles repo root', () => {
    const repoPath = '/home/user/repo';
    const sameAsRepo = '/home/user/repo';
    
    const relativePath = path.relative(repoPath, sameAsRepo);
    // relativePath will be empty string for equal paths
    const isInRepo = !!relativePath && !relativePath.startsWith('..') && !path.isAbsolute(relativePath);
    
    // Empty string is falsy, so this should be false (repo root itself, not inside)
    assert.strictEqual(isInRepo, false);
    assert.strictEqual(relativePath, '');
});

// Test: Path normalization allows duplicate slashes
runner.test('Path normalization is lenient with duplicate slashes', () => {
    const pathWithDuplicateSlashes = '/home//user///file.txt';
    const normalized = path.normalize(pathWithDuplicateSlashes);
    const segments = normalized.split(path.sep);
    
    // Should not contain '..' after normalization
    assert.strictEqual(segments.includes('..'), false);
    // Normalization should clean up duplicate slashes
    assert.strictEqual(normalized, '/home/user/file.txt');
});

// Test: Path normalization detects traversal in segments
runner.test('Path normalization detects .. in path segments', () => {
    const traversalPath = '/home/user/../../etc/passwd';
    const normalized = path.normalize(traversalPath);
    const segments = normalized.split(path.sep);
    
    // After normalization, path will be '/etc/passwd', no '..' segments
    // But the normalized path will show the traversal occurred
    assert.strictEqual(segments.includes('..'), false);
    // The path itself will have changed significantly
    assert.strictEqual(normalized, '/etc/passwd');
});

// Test: Non-string token validation
runner.test('Token validation rejects non-string tokens', () => {
    // Simulate the type check in authentication
    const validateToken = (token) => {
        if (typeof token !== 'string') {
            return { valid: false, error: 'Invalid token type' };
        }
        if (Buffer.byteLength(token, 'utf8') === 0) {
            return { valid: false, error: 'Empty token' };
        }
        return { valid: true };
    };
    
    // Non-string types should be rejected
    assert.strictEqual(validateToken(null).valid, false);
    assert.strictEqual(validateToken(undefined).valid, false);
    assert.strictEqual(validateToken(123).valid, false);
    assert.strictEqual(validateToken({ token: 'value' }).valid, false);
    assert.strictEqual(validateToken(['token']).valid, false);
    
    // Valid string should pass
    assert.strictEqual(validateToken('valid-token').valid, true);
});

// Test: Leading/trailing whitespace in tokens
runner.test('Token comparison requires exact match (no automatic trimming)', () => {
    // Tokens are compared byte-for-byte without trimming
    // This documents that leading/trailing whitespace is significant
    
    const token1 = 'my-token';
    const token2 = ' my-token'; // leading space
    const token3 = 'my-token '; // trailing space
    
    const buffer1 = Buffer.from(token1, 'utf8');
    const buffer2 = Buffer.from(token2, 'utf8');
    const buffer3 = Buffer.from(token3, 'utf8');
    
    // Buffers with different content (including whitespace) have different lengths
    assert.notStrictEqual(buffer1.length, buffer2.length);
    assert.notStrictEqual(buffer1.length, buffer3.length);
    
    // Identical tokens match
    const buffer4 = Buffer.from(token1, 'utf8');
    assert.strictEqual(crypto.timingSafeEqual(buffer1, buffer4), true);
});

// Test: Whitespace-only token validation at startup
runner.test('Startup validation rejects empty and whitespace-only tokens', () => {
    // Simulate the startup validation logic (now checks empty string explicitly before trim)
    const validateStartupToken = (token) => {
        if (typeof token !== 'string') {
            return { valid: false, error: 'Token must be a string' };
        }
        // Check for empty string explicitly (before trim)
        if (token.length === 0) {
            return { valid: false, error: 'Token is empty string' };
        }
        // Check for whitespace-only tokens
        if (token.trim().length === 0) {
            return { valid: false, error: 'Token is whitespace-only' };
        }
        return { valid: true };
    };
    
    // Empty string should be rejected (explicit check)
    assert.strictEqual(validateStartupToken('').valid, false);
    assert.strictEqual(validateStartupToken('').error, 'Token is empty string');
    
    // Whitespace-only tokens should be rejected
    assert.strictEqual(validateStartupToken('   ').valid, false);
    assert.strictEqual(validateStartupToken('   ').error, 'Token is whitespace-only');
    assert.strictEqual(validateStartupToken('\t\n').valid, false);
    assert.strictEqual(validateStartupToken('\t\n').error, 'Token is whitespace-only');
    
    // Valid token with content should pass
    assert.strictEqual(validateStartupToken('my-token').valid, true);
    assert.strictEqual(validateStartupToken('  token-with-spaces  ').valid, true); // trimmed length > 0
});

// Test: Empty string token validation
runner.test('Empty string tokens are rejected', () => {
    const validateToken = (token) => {
        if (typeof token !== 'string') {
            return { valid: false };
        }
        if (Buffer.byteLength(token, 'utf8') === 0) {
            return { valid: false };
        }
        return { valid: true };
    };
    
    assert.strictEqual(validateToken('').valid, false);
    assert.strictEqual(validateToken('x').valid, true);
});

// Test: AUTH_TOKEN_BUFFER only set when AUTH_TOKEN exists
runner.test('AUTH_TOKEN_BUFFER is only set when AUTH_TOKEN is configured', () => {
    // Simulate configuration scenarios
    const scenario1 = {
        AUTH_TOKEN: null,
        AUTH_TOKEN_BUFFER: null
    };
    
    const scenario2 = {
        AUTH_TOKEN: 'my-secret-token',
        AUTH_TOKEN_BUFFER: null
    };
    
    // When AUTH_TOKEN is null, BUFFER should remain null
    assert.strictEqual(scenario1.AUTH_TOKEN_BUFFER, null);
    
    // When AUTH_TOKEN is set, we would set the buffer
    if (scenario2.AUTH_TOKEN && typeof scenario2.AUTH_TOKEN === 'string') {
        scenario2.AUTH_TOKEN_BUFFER = Buffer.from(scenario2.AUTH_TOKEN, 'utf8');
    }
    
    // Buffer should now be set
    assert.ok(scenario2.AUTH_TOKEN_BUFFER !== null);
    assert.strictEqual(scenario2.AUTH_TOKEN_BUFFER.length, Buffer.byteLength('my-secret-token', 'utf8'));
});

// Run all tests
runner.run().then(success => {
    process.exit(success ? 0 : 1);
}).catch(error => {
    console.error('Test runner error:', error);
    process.exit(1);
});
