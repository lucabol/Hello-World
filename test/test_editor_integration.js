#!/usr/bin/env node
/**
 * Integration tests for the visual editor
 * Tests that editor.html properly integrates with the generator module
 * and validates security features are in place
 * 
 * Run with: node test/test_editor_integration.js
 * Dependencies: None - uses only built-in Node.js APIs
 */

const fs = require('fs');
const path = require('path');
const generator = require('../tools/editor/generator.js');

console.log("🧪 Running Visual Editor Integration Tests...\n");

// Read the editor.html file to verify integration
const editorPath = path.join(__dirname, '..', 'tools', 'editor', 'editor.html');

if (!fs.existsSync(editorPath)) {
    console.error("✗ ERROR: editor.html not found at", editorPath);
    process.exit(1);
}

const editorContent = fs.readFileSync(editorPath, 'utf8');

let passed = 0;
let failed = 0;

// Test 1: Verify generator.js is included
console.log("Test 1: Checking for generator.js integration...");
if (editorContent.includes('generator.js')) {
    console.log("  ✓ generator.js script included");
    passed++;
} else {
    console.log("  ✗ generator.js script not included");
    failed++;
}

// Test 2: Verify CSP meta tag exists
console.log("\nTest 2: Checking for Content-Security-Policy...");
if (editorContent.includes('Content-Security-Policy')) {
    console.log("  ✓ CSP meta tag found");
    passed++;
} else {
    console.log("  ✗ CSP meta tag missing");
    failed++;
}

// Test 3: Verify CCodeGenerator is used
console.log("\nTest 3: Checking for CCodeGenerator usage...");
if (editorContent.includes('CCodeGenerator')) {
    console.log("  ✓ CCodeGenerator module is used");
    passed++;
} else {
    console.log("  ✗ CCodeGenerator module not used");
    failed++;
}

// Test 4: Verify accessibility attributes
console.log("\nTest 4: Checking for ARIA accessibility attributes...");
const ariaChecks = [
    'aria-label',
    'role="region"',
    'role="button"',
    'aria-live'
];
let ariaFound = 0;
ariaChecks.forEach(attr => {
    if (editorContent.includes(attr)) {
        ariaFound++;
    }
});
if (ariaFound === ariaChecks.length) {
    console.log(`  ✓ All ${ariaChecks.length} ARIA attributes found`);
    passed++;
} else {
    console.log(`  ✗ Only ${ariaFound}/${ariaChecks.length} ARIA attributes found`);
    failed++;
}

// Test 5: Verify keyboard support
console.log("\nTest 5: Checking for keyboard navigation support...");
if (editorContent.includes('onkeydown') && editorContent.includes('tabindex')) {
    console.log("  ✓ Keyboard support found");
    passed++;
} else {
    console.log("  ✗ Keyboard support incomplete");
    failed++;
}

// Test 6: Verify security comments
console.log("\nTest 6: Checking for security documentation...");
const securityComments = [
    'Security:',
    'XSS',
    'Hardcoded filename'
];
let commentFound = 0;
securityComments.forEach(term => {
    if (editorContent.includes(term)) {
        commentFound++;
    }
});
if (commentFound >= 2) {
    console.log(`  ✓ Security documentation found (${commentFound}/${securityComments.length} markers)`);
    passed++;
} else {
    console.log(`  ✗ Insufficient security documentation (${commentFound}/${securityComments.length} markers)`);
    failed++;
}

// Test 7: Test code generation through the generator module directly
console.log("\nTest 7: Testing code generation with edge case inputs...");
const testCases = [
    { 
        blocks: [
            { id: 0, type: 'include', value: 'stdio.h' },
            { id: 1, type: 'printf', value: 'Hello% World' }
        ],
        expected: 'Hello%% World',
        desc: 'percent sign escaping'
    },
    { 
        blocks: [
            { id: 0, type: 'include', value: 'stdio.h' },
            { id: 1, type: 'printf', value: '<script>alert("xss")</script>' }
        ],
        expected: '&lt;script&gt;',
        desc: 'HTML script tag escaping'
    },
    { 
        blocks: [
            { id: 0, type: 'include', value: 'stdio.h' },
            { id: 1, type: 'printf', value: '"quotes"' }
        ],
        expected: '&quot;quotes&quot;',
        desc: 'double quotes escaping'
    },
    {
        blocks: [
            { id: 0, type: 'printf', value: 'Test' }
        ],
        expected: '#include <stdio.h>',
        desc: 'auto-include stdio.h'
    }
];

let edgeCasesPassed = 0;
testCases.forEach((tc, i) => {
    const code = generator.generateCode(tc.blocks);
    if (code.includes(tc.expected)) {
        edgeCasesPassed++;
    } else {
        console.log(`  ✗ Test case ${i + 1} failed (${tc.desc})`);
        console.log(`    Expected to find: ${tc.expected}`);
        console.log(`    In code: ${code.substring(0, 100)}...`);
    }
});

if (edgeCasesPassed === testCases.length) {
    console.log(`  ✓ All ${testCases.length} edge case tests passed`);
    passed++;
} else {
    console.log(`  ✗ Only ${edgeCasesPassed}/${testCases.length} edge case tests passed`);
    failed++;
}

// Test 8: Verify no unsafe innerHTML with user data
console.log("\nTest 8: Verifying safe DOM manipulation...");
const dangerousPatterns = [
    /innerHTML\s*=\s*block\.value/,
    /innerHTML\s*=\s*text/,
    /innerHTML\s*=\s*newValue/
];
let unsafeUsage = false;
dangerousPatterns.forEach(pattern => {
    if (pattern.test(editorContent)) {
        unsafeUsage = true;
    }
});
if (!unsafeUsage) {
    console.log("  ✓ No unsafe innerHTML usage with user data");
    passed++;
} else {
    console.log("  ✗ Unsafe innerHTML usage detected");
    failed++;
}

// Test 9: Verify textContent is used
console.log("\nTest 9: Verifying textContent usage for safety...");
const textContentCount = (editorContent.match(/textContent\s*=/g) || []).length;
if (textContentCount >= 3) {
    console.log(`  ✓ textContent used ${textContentCount} times (safe DOM manipulation)`);
    passed++;
} else {
    console.log(`  ✗ textContent used only ${textContentCount} times (expected at least 3)`);
    failed++;
}

// Test 10: Verify announceToScreenReader function
console.log("\nTest 10: Checking for screen reader announcements...");
if (editorContent.includes('announceToScreenReader') || editorContent.includes('aria-live')) {
    console.log("  ✓ Screen reader support found");
    passed++;
} else {
    console.log("  ✗ Screen reader support missing");
    failed++;
}

// Summary
console.log(`\n${'='.repeat(60)}`);
console.log(`Results: ${passed} passed, ${failed} failed`);
console.log(`${'='.repeat(60)}`);

if (failed > 0) {
    console.log("\n✗ Integration tests failed");
    process.exit(1);
}

console.log("\n✅ All integration tests passed!");
process.exit(0);
