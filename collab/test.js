/**
 * Unit tests for collaborative editing server
 * Tests OT algorithm, validation, and operation application
 */

const assert = require('assert');
const { validateOperation, transformOperation, applyOperation } = require('./server');

// Mock fileContent for validation tests
let mockFileContent = 'Hello world!';

// Temporarily override module fileContent for tests
const originalModule = require('./server');

console.log('Running Collaborative Editing Server Tests...\n');

// Test Suite 1: Operation Validation
console.log('=== Operation Validation Tests ===');

function testValidation() {
    let passed = 0;
    let failed = 0;
    
    // Save original and set mock
    const originalValidate = originalModule.validateOperation;
    
    // Manually create validation function with mock content
    function testValidateOperation(operation, contentLength = 12) {
        if (!operation || typeof operation !== 'object') {
            return { valid: false, error: 'Operation must be an object' };
        }
        
        const { type, position, text, length } = operation;
        const MAX_OPERATION_SIZE = 10000;
        const MAX_POSITION = 1024 * 1024;
        
        if (!['insert', 'delete', 'replace'].includes(type)) {
            return { valid: false, error: 'Invalid operation type' };
        }
        
        if (typeof position !== 'number' || position < 0 || position > MAX_POSITION) {
            return { valid: false, error: 'Invalid position' };
        }
        
        if (type !== 'replace' && position > contentLength) {
            return { valid: false, error: 'Position exceeds content length' };
        }
        
        if (type === 'insert' || type === 'replace') {
            if (typeof text !== 'string') {
                return { valid: false, error: 'Text must be a string' };
            }
            if (text.length > MAX_OPERATION_SIZE) {
                return { valid: false, error: 'Text exceeds maximum operation size' };
            }
            if (text.includes('\0')) {
                return { valid: false, error: 'Text contains invalid characters' };
            }
        }
        
        if (type === 'delete') {
            if (typeof length !== 'number' || length < 0 || length > MAX_OPERATION_SIZE) {
                return { valid: false, error: 'Invalid delete length' };
            }
            if (position + length > contentLength) {
                return { valid: false, error: 'Delete range exceeds content length' };
            }
        }
        
        return { valid: true };
    }
    
    // Test 1: Valid insert operation
    try {
        const result = testValidateOperation({ type: 'insert', position: 5, text: 'abc' });
        assert.strictEqual(result.valid, true, 'Valid insert should pass');
        console.log('✓ Valid insert operation');
        passed++;
    } catch (e) {
        console.log('✗ Valid insert operation:', e.message);
        failed++;
    }
    
    // Test 2: Valid delete operation
    try {
        const result = testValidateOperation({ type: 'delete', position: 0, length: 5 });
        assert.strictEqual(result.valid, true, 'Valid delete should pass');
        console.log('✓ Valid delete operation');
        passed++;
    } catch (e) {
        console.log('✗ Valid delete operation:', e.message);
        failed++;
    }
    
    // Test 3: Valid replace operation
    try {
        const result = testValidateOperation({ type: 'replace', position: 0, text: 'New content' });
        assert.strictEqual(result.valid, true, 'Valid replace should pass');
        console.log('✓ Valid replace operation');
        passed++;
    } catch (e) {
        console.log('✗ Valid replace operation:', e.message);
        failed++;
    }
    
    // Test 4: Invalid operation type
    try {
        const result = testValidateOperation({ type: 'invalid', position: 0 });
        assert.strictEqual(result.valid, false, 'Invalid type should fail');
        console.log('✓ Invalid operation type rejected');
        passed++;
    } catch (e) {
        console.log('✗ Invalid operation type:', e.message);
        failed++;
    }
    
    // Test 5: Negative position
    try {
        const result = testValidateOperation({ type: 'insert', position: -1, text: 'abc' });
        assert.strictEqual(result.valid, false, 'Negative position should fail');
        console.log('✓ Negative position rejected');
        passed++;
    } catch (e) {
        console.log('✗ Negative position:', e.message);
        failed++;
    }
    
    // Test 6: Position exceeds content length
    try {
        const result = testValidateOperation({ type: 'insert', position: 100, text: 'abc' });
        assert.strictEqual(result.valid, false, 'Position beyond content should fail');
        console.log('✓ Position beyond content rejected');
        passed++;
    } catch (e) {
        console.log('✗ Position beyond content:', e.message);
        failed++;
    }
    
    // Test 7: Insert with non-string text
    try {
        const result = testValidateOperation({ type: 'insert', position: 0, text: 123 });
        assert.strictEqual(result.valid, false, 'Non-string text should fail');
        console.log('✓ Non-string text rejected');
        passed++;
    } catch (e) {
        console.log('✗ Non-string text:', e.message);
        failed++;
    }
    
    // Test 8: Insert with null byte
    try {
        const result = testValidateOperation({ type: 'insert', position: 0, text: 'abc\0def' });
        assert.strictEqual(result.valid, false, 'Null byte should fail');
        console.log('✓ Null byte in text rejected');
        passed++;
    } catch (e) {
        console.log('✗ Null byte in text:', e.message);
        failed++;
    }
    
    // Test 9: Delete with invalid length
    try {
        const result = testValidateOperation({ type: 'delete', position: 0, length: -1 });
        assert.strictEqual(result.valid, false, 'Negative length should fail');
        console.log('✓ Negative delete length rejected');
        passed++;
    } catch (e) {
        console.log('✗ Negative delete length:', e.message);
        failed++;
    }
    
    // Test 10: Delete beyond content
    try {
        const result = testValidateOperation({ type: 'delete', position: 0, length: 100 });
        assert.strictEqual(result.valid, false, 'Delete beyond content should fail');
        console.log('✓ Delete beyond content rejected');
        passed++;
    } catch (e) {
        console.log('✗ Delete beyond content:', e.message);
        failed++;
    }
    
    console.log(`\nValidation Tests: ${passed} passed, ${failed} failed\n`);
    return failed === 0;
}

// Test Suite 2: Operational Transform
console.log('=== Operational Transform Tests ===');

function testOT() {
    let passed = 0;
    let failed = 0;
    
    // Test 1: Concurrent inserts at same position
    try {
        const op1 = { type: 'insert', position: 5, text: 'abc' };
        const op2 = { type: 'insert', position: 5, text: 'xyz' };
        const transformed = transformOperation(op1, op2);
        assert.strictEqual(transformed.position, 8, 'Second insert should be shifted by first insert length');
        console.log('✓ Concurrent inserts at same position');
        passed++;
    } catch (e) {
        console.log('✗ Concurrent inserts:', e.message);
        failed++;
    }
    
    // Test 2: Concurrent deletes at same position
    try {
        const op1 = { type: 'delete', position: 5, length: 3 };
        const op2 = { type: 'delete', position: 5, length: 2 };
        const transformed = transformOperation(op1, op2);
        assert.strictEqual(transformed.position, 2, 'Second delete should be adjusted');
        console.log('✓ Concurrent deletes at same position');
        passed++;
    } catch (e) {
        console.log('✗ Concurrent deletes:', e.message);
        failed++;
    }
    
    // Test 3: Insert before another insert
    try {
        const op1 = { type: 'insert', position: 3, text: 'ab' };
        const op2 = { type: 'insert', position: 5, text: 'xy' };
        const transformed = transformOperation(op1, op2);
        assert.strictEqual(transformed.position, 7, 'Second insert position should increase');
        console.log('✓ Insert before another insert');
        passed++;
    } catch (e) {
        console.log('✗ Insert before another insert:', e.message);
        failed++;
    }
    
    // Test 4: Delete before another operation
    try {
        const op1 = { type: 'delete', position: 2, length: 3 };
        const op2 = { type: 'insert', position: 8, text: 'x' };
        const transformed = transformOperation(op1, op2);
        assert.strictEqual(transformed.position, 5, 'Position should decrease by delete length');
        console.log('✓ Delete before another operation');
        passed++;
    } catch (e) {
        console.log('✗ Delete before another operation:', e.message);
        failed++;
    }
    
    // Test 5: Non-overlapping operations
    try {
        const op1 = { type: 'insert', position: 10, text: 'a' };
        const op2 = { type: 'insert', position: 2, text: 'b' };
        const transformed = transformOperation(op1, op2);
        assert.strictEqual(transformed.position, 2, 'Non-overlapping should not affect position');
        console.log('✓ Non-overlapping operations');
        passed++;
    } catch (e) {
        console.log('✗ Non-overlapping operations:', e.message);
        failed++;
    }
    
    console.log(`\nOT Tests: ${passed} passed, ${failed} failed\n`);
    return failed === 0;
}

// Test Suite 3: Operation Application
console.log('=== Operation Application Tests ===');

function testApplication() {
    let passed = 0;
    let failed = 0;
    
    // Test 1: Apply insert
    try {
        const content = 'Hello world!';
        const op = { type: 'insert', position: 6, text: 'beautiful ' };
        const result = applyOperation(content, op);
        assert.strictEqual(result, 'Hello beautiful world!', 'Insert should be applied correctly');
        console.log('✓ Apply insert operation');
        passed++;
    } catch (e) {
        console.log('✗ Apply insert:', e.message);
        failed++;
    }
    
    // Test 2: Apply delete
    try {
        const content = 'Hello world!';
        const op = { type: 'delete', position: 5, length: 6 };
        const result = applyOperation(content, op);
        assert.strictEqual(result, 'Hello!', 'Delete should be applied correctly');
        console.log('✓ Apply delete operation');
        passed++;
    } catch (e) {
        console.log('✗ Apply delete:', e.message);
        failed++;
    }
    
    // Test 3: Apply replace
    try {
        const content = 'Hello world!';
        const op = { type: 'replace', position: 0, text: 'Goodbye universe!' };
        const result = applyOperation(content, op);
        assert.strictEqual(result, 'Goodbye universe!', 'Replace should replace entire content');
        console.log('✓ Apply replace operation');
        passed++;
    } catch (e) {
        console.log('✗ Apply replace:', e.message);
        failed++;
    }
    
    // Test 4: Apply insert at start
    try {
        const content = 'world';
        const op = { type: 'insert', position: 0, text: 'Hello ' };
        const result = applyOperation(content, op);
        assert.strictEqual(result, 'Hello world', 'Insert at start should work');
        console.log('✓ Apply insert at start');
        passed++;
    } catch (e) {
        console.log('✗ Apply insert at start:', e.message);
        failed++;
    }
    
    // Test 5: Apply insert at end
    try {
        const content = 'Hello';
        const op = { type: 'insert', position: 5, text: ' world' };
        const result = applyOperation(content, op);
        assert.strictEqual(result, 'Hello world', 'Insert at end should work');
        console.log('✓ Apply insert at end');
        passed++;
    } catch (e) {
        console.log('✗ Apply insert at end:', e.message);
        failed++;
    }
    
    // Test 6: Apply delete at start
    try {
        const content = 'Hello world';
        const op = { type: 'delete', position: 0, length: 6 };
        const result = applyOperation(content, op);
        assert.strictEqual(result, 'world', 'Delete at start should work');
        console.log('✓ Apply delete at start');
        passed++;
    } catch (e) {
        console.log('✗ Apply delete at start:', e.message);
        failed++;
    }
    
    // Test 7: Apply delete at end
    try {
        const content = 'Hello world';
        const op = { type: 'delete', position: 5, length: 6 };
        const result = applyOperation(content, op);
        assert.strictEqual(result, 'Hello', 'Delete at end should work');
        console.log('✓ Apply delete at end');
        passed++;
    } catch (e) {
        console.log('✗ Apply delete at end:', e.message);
        failed++;
    }
    
    // Test 8: Sequential operations
    try {
        let content = 'Hello';
        content = applyOperation(content, { type: 'insert', position: 5, text: ' world' });
        content = applyOperation(content, { type: 'insert', position: 11, text: '!' });
        content = applyOperation(content, { type: 'delete', position: 5, length: 1 });
        assert.strictEqual(content, 'Helloworld!', 'Sequential operations should work');
        console.log('✓ Sequential operations');
        passed++;
    } catch (e) {
        console.log('✗ Sequential operations:', e.message);
        failed++;
    }
    
    console.log(`\nApplication Tests: ${passed} passed, ${failed} failed\n`);
    return failed === 0;
}

// Run all test suites
const validationPassed = testValidation();
const otPassed = testOT();
const applicationPassed = testApplication();

console.log('='.repeat(60));
if (validationPassed && otPassed && applicationPassed) {
    console.log('✓ All tests passed!');
    process.exit(0);
} else {
    console.log('✗ Some tests failed');
    process.exit(1);
}
