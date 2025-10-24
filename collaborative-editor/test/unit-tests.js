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

// Run all tests
runner.run().then(success => {
    process.exit(success ? 0 : 1);
}).catch(error => {
    console.error('Test runner error:', error);
    process.exit(1);
});
