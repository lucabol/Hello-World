#!/usr/bin/env node
/**
 * Integration tests for the visual editor
 * Tests that editor.html properly integrates with the generator module
 * and validates security features are in place
 * 
 * Run with: node test/test_editor_integration.js
 * Dependencies: None - uses only built-in Node.js APIs
 * 
 * DESIGN DECISION: Zero-dependency HTML validation
 * ================================================
 * This test suite intentionally avoids external dependencies (jsdom, parse5, etc.)
 * to keep the project dependency-free and CI-friendly. The trade-off is that we
 * use carefully crafted regex patterns instead of a full HTML parser.
 * 
 * To maintain robustness, the patterns are designed to:
 * - Accept common variations (relative paths, attribute ordering, whitespace)
 * - Focus on structural validation rather than exact string matching
 * - Validate the presence and correctness of security-critical elements
 * 
 * If false negatives occur due to formatting changes, the patterns can be
 * updated without adding dependencies. For complex HTML validation needs,
 * consider switching to a proper parser library.
 */

const fs = require('fs');
const path = require('path');
const generator = require('../tools/editor/generator.js');

console.log("🧪 Running Visual Editor Integration Tests...\n");

// Test 0: Verify generator.js exports API shape for Node.js
console.log("=== Module Compatibility Tests ===\n");
console.log("Test 0: Verifying generator.js module exports...");
let moduleTestsPassed = 0;
let moduleTestsFailed = 0;

const expectedExports = ['generateCode', 'escapeHtml', 'escapePrintfFormat'];
const actualExports = Object.keys(generator);

const missingExports = expectedExports.filter(exp => !actualExports.includes(exp));
if (missingExports.length === 0 && typeof generator.generateCode === 'function') {
    console.log(`  ✓ All expected exports present: ${expectedExports.join(', ')}`);
    console.log(`  ✓ generator.generateCode is a function`);
    moduleTestsPassed++;
} else {
    console.log(`  ✗ Module exports incomplete`);
    if (missingExports.length > 0) {
        console.log(`    Missing: ${missingExports.join(', ')}`);
    }
    console.log(`    Found: ${actualExports.join(', ')}`);
    moduleTestsFailed++;
}

// Test that the module works in Node.js
console.log("\nTest 0b: Testing generator.generateCode() in Node.js...");
try {
    const testBlocks = [
        { id: 0, type: 'include', value: 'stdio.h' },
        { id: 1, type: 'printf', value: 'Test' }
    ];
    const code = generator.generateCode(testBlocks);
    if (code && code.includes('stdio.h') && code.includes('Test')) {
        console.log('  ✓ generator.generateCode() works correctly in Node.js');
        moduleTestsPassed++;
    } else {
        console.log('  ✗ generator.generateCode() produced unexpected output');
        console.log(`    Output: ${code.substring(0, 100)}...`);
        moduleTestsFailed++;
    }
} catch (err) {
    console.log('  ✗ generator.generateCode() threw an error:', err.message);
    moduleTestsFailed++;
}

console.log(`\n  Module Tests: ${moduleTestsPassed} passed, ${moduleTestsFailed} failed\n`);

// Read the editor.html file to verify integration
const editorPath = path.join(__dirname, '..', 'tools', 'editor', 'editor.html');

if (!fs.existsSync(editorPath)) {
    console.error("✗ ERROR: editor.html not found at", editorPath);
    process.exit(1);
}

const editorContent = fs.readFileSync(editorPath, 'utf8');

let passed = 0;
let failed = 0;

/**
 * Helper function to validate HTML elements with attributes
 * Tolerates common variations: whitespace, attribute order, quote styles
 * 
 * Uses lookaheads to make attribute matching order-insensitive, avoiding false
 * negatives when attributes appear in different orders in the HTML.
 * 
 * @param {string} html - The HTML content to search
 * @param {string} tagName - Tag name to find (e.g., 'script', 'meta')
 * @param {Object} attributes - Attributes to validate {name: value}
 * @returns {boolean} - True if element with attributes is found
 */
function parseHtmlForElement(html, tagName, attributes = {}) {
    // Escape the tag name to prevent regex injection
    const escapedTagName = tagName.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
    
    // Build lookahead patterns for each attribute (order-insensitive)
    const attrLookaheads = Object.entries(attributes).map(([key, value]) => {
        // Escape attribute name to prevent regex injection
        const escapedKey = key.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
        
        if (value === true) {
            // Boolean attribute: match presence with or without value
            // Accepts: attr, attr="", attr="value", attr='value', attr=value
            return `(?=(?:[^>]*\\b${escapedKey}(?:\\s*=\\s*(?:["'][^"']*["']|[^\\s>]+)|(?=\\s|>))))`;
        }
        
        // Escape special regex chars in value
        const escapedValue = String(value).replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
        
        // Match attribute with specific value (single or double quotes)
        return `(?=(?:[^>]*\\b${escapedKey}\\s*=\\s*["']${escapedValue}["']))`;
    });
    
    // Build final pattern: opening tag + all attribute lookaheads + anything until closing >
    const lookaheadPattern = attrLookaheads.join('');
    const pattern = new RegExp(`<${escapedTagName}${lookaheadPattern}[^>]*>`, 'i');
    
    return pattern.test(html);
}

/**
 * Count occurrences of a pattern in HTML
 * @param {string} html - The HTML content to search
 * @param {RegExp} pattern - The pattern to match
 * @returns {number} - Count of matches
 */
function countOccurrences(html, pattern) {
    const matches = html.match(pattern);
    return matches ? matches.length : 0;
}

// Unit tests for parseHtmlForElement helper
console.log("=== Unit Tests for parseHtmlForElement Helper ===\n");
let helperTestsPassed = 0;
let helperTestsFailed = 0;

const helperTests = [
    {
        name: 'Attributes in different order',
        html: '<meta content="test" http-equiv="X-Test">',
        tag: 'meta',
        attrs: { 'http-equiv': 'X-Test', content: 'test' },
        expected: true
    },
    {
        name: 'Single quotes vs double quotes',
        html: `<script src='test.js'></script>`,
        tag: 'script',
        attrs: { src: 'test.js' },
        expected: true
    },
    {
        name: 'Extra whitespace around attributes',
        html: '<div   role  =  "button"   aria-label = "Test"   >',
        tag: 'div',
        attrs: { role: 'button', 'aria-label': 'Test' },
        expected: true
    },
    {
        name: 'Boolean attribute present',
        html: '<input disabled>',
        tag: 'input',
        attrs: { disabled: true },
        expected: true
    },
    {
        name: 'Boolean attribute with value',
        html: '<input disabled="disabled">',
        tag: 'input',
        attrs: { disabled: true },
        expected: true
    },
    {
        name: 'Missing required attribute',
        html: '<div role="button">',
        tag: 'div',
        attrs: { role: 'button', 'aria-label': 'Test' },
        expected: false
    },
    {
        name: 'Attribute value mismatch',
        html: '<meta http-equiv="wrong-value">',
        tag: 'meta',
        attrs: { 'http-equiv': 'correct-value' },
        expected: false
    },
    {
        name: 'Special characters in attribute value',
        html: '<div data-test="value-with-dashes_and.dots">',
        tag: 'div',
        attrs: { 'data-test': 'value-with-dashes_and.dots' },
        expected: true
    },
    {
        name: 'Case-insensitive tag matching',
        html: '<SCRIPT SRC="test.js"></SCRIPT>',
        tag: 'script',
        attrs: { src: 'test.js' },
        expected: true
    },
    {
        name: 'Attributes in reverse order from test',
        html: '<meta name="viewport" content="width=device-width">',
        tag: 'meta',
        attrs: { content: 'width=device-width', name: 'viewport' },
        expected: true
    }
];

helperTests.forEach((test, idx) => {
    const result = parseHtmlForElement(test.html, test.tag, test.attrs);
    if (result === test.expected) {
        console.log(`  ✓ Helper test ${idx + 1}: ${test.name}`);
        helperTestsPassed++;
    } else {
        console.log(`  ✗ Helper test ${idx + 1}: ${test.name}`);
        console.log(`    Expected: ${test.expected}, Got: ${result}`);
        console.log(`    HTML: ${test.html}`);
        helperTestsFailed++;
    }
});

console.log(`\n  Helper Tests: ${helperTestsPassed} passed, ${helperTestsFailed} failed\n`);
console.log("=== Main Integration Tests ===\n");

// Test 1: Verify generator.js script tag is properly included
// Accepts variations: 'generator.js', './generator.js', relative paths
console.log("Test 1: Checking for generator.js integration...");
const scriptSrcPattern = /<script[^>]+src\s*=\s*["'](?:\.\/)?(?:[^"']*\/)?generator\.js["'][^>]*>/i;
if (scriptSrcPattern.test(editorContent)) {
    console.log("  ✓ generator.js script tag found with correct src attribute");
    passed++;
} else {
    console.log("  ✗ generator.js script tag not found or improperly configured");
    console.log("    Expected: <script src=\"generator.js\"> or <script src=\"./generator.js\">");
    failed++;
}

// Test 2: Verify CSP meta tag with proper http-equiv attribute
// Accepts case-insensitive attributes and various CSP content
console.log("\nTest 2: Checking for Content-Security-Policy...");
const cspMetaPattern = /<meta[^>]+http-equiv\s*=\s*["']Content-Security-Policy["'][^>]*>/i;
if (cspMetaPattern.test(editorContent)) {
    console.log("  ✓ CSP meta tag with http-equiv attribute found");
    passed++;
} else {
    console.log("  ✗ CSP meta tag missing or improperly configured");
    console.log("    Expected: <meta http-equiv=\"Content-Security-Policy\" content=\"...\">");
    failed++;
}

// Test 3: Verify CCodeGenerator module usage in JavaScript
console.log("\nTest 3: Checking for CCodeGenerator usage...");
const codeGeneratorPattern = /CCodeGenerator\s*\.\s*generateCode/;
if (codeGeneratorPattern.test(editorContent)) {
    console.log("  ✓ CCodeGenerator.generateCode() is called");
    passed++;
} else {
    console.log("  ✗ CCodeGenerator module not properly used");
    failed++;
}

// Test 4: Verify accessibility attributes - structural validation
// Check for required ARIA elements in each major section
console.log("\nTest 4: Checking for ARIA accessibility attributes...");
const ariaTests = [
    { 
        name: 'Palette region with buttons', 
        pattern: /<[^>]+role\s*=\s*["']region["'][^>]*aria-label\s*=\s*["']Block Palette["'][^>]*>[\s\S]*?<[^>]+role\s*=\s*["']button["'][^>]*>/i,
        desc: 'Block palette should be a region with labeled buttons'
    },
    { 
        name: 'Workspace region', 
        pattern: /<[^>]+role\s*=\s*["']region["'][^>]*aria-label\s*=\s*["']Workspace["'][^>]*>/i,
        desc: 'Workspace should be a labeled region'
    },
    { 
        name: 'Code preview with aria-live', 
        pattern: /<[^>]+role\s*=\s*["']log["'][^>]*aria-live\s*=\s*["']polite["'][^>]*>/i,
        desc: 'Code preview should have aria-live for screen readers'
    },
    {
        name: 'Interactive elements with aria-label',
        pattern: /aria-label\s*=\s*["'][^"']+["']/gi,
        minCount: 8,
        desc: 'At least 8 elements should have descriptive aria-labels'
    }
];

let ariaTestsPassed = 0;
let ariaTestsFailed = [];
ariaTests.forEach(test => {
    if (test.minCount) {
        const count = countOccurrences(editorContent, test.pattern);
        if (count >= test.minCount) {
            ariaTestsPassed++;
        } else {
            ariaTestsFailed.push(`${test.name}: found ${count}, expected at least ${test.minCount}`);
        }
    } else {
        if (test.pattern.test(editorContent)) {
            ariaTestsPassed++;
        } else {
            ariaTestsFailed.push(`${test.name}: ${test.desc}`);
        }
    }
});

if (ariaTestsPassed === ariaTests.length) {
    console.log(`  ✓ All ${ariaTests.length} ARIA accessibility requirements met`);
    passed++;
} else {
    console.log(`  ✗ Only ${ariaTestsPassed}/${ariaTests.length} ARIA tests passed`);
    ariaTestsFailed.forEach(failure => console.log(`    - ${failure}`));
    failed++;
}

// Test 5: Verify keyboard event handlers are properly attached
console.log("\nTest 5: Checking for keyboard navigation support...");
const hasOnKeyDown = /\.onkeydown\s*=/.test(editorContent);
const hasTabIndex = /tabindex=["']\d+["']/.test(editorContent);
if (hasOnKeyDown && hasTabIndex) {
    console.log("  ✓ Keyboard event handlers and tabindex properly configured");
    passed++;
} else {
    console.log("  ✗ Keyboard support incomplete");
    if (!hasOnKeyDown) console.log("    - Missing onkeydown handlers");
    if (!hasTabIndex) console.log("    - Missing tabindex attributes");
    failed++;
}

// Test 6: Verify security documentation in comments
console.log("\nTest 6: Checking for security documentation...");
const securityPatterns = [
    { name: 'Security comments', pattern: /\/\*\*[\s\S]*?Security:[\s\S]*?\*\// },
    { name: 'XSS protection docs', pattern: /XSS|Cross-Site Scripting/i },
    { name: 'Hardcoded filename docs', pattern: /Hardcoded filename|cannot be manipulated/i }
];

let securityDocsPassed = 0;
securityPatterns.forEach(test => {
    if (test.pattern.test(editorContent)) {
        securityDocsPassed++;
    }
});

if (securityDocsPassed >= 2) {
    console.log(`  ✓ Security documentation found (${securityDocsPassed}/${securityPatterns.length} types)`);
    passed++;
} else {
    console.log(`  ✗ Insufficient security documentation (${securityDocsPassed}/${securityPatterns.length} types)`);
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

// Test 8: Verify no unsafe innerHTML with user data using strict pattern matching
console.log("\nTest 8: Verifying safe DOM manipulation...");
const dangerousPatterns = [
    { pattern: /\.innerHTML\s*=\s*(?:block\.value|text|newValue)/g, desc: 'Direct innerHTML with user data' },
    { pattern: /\.innerHTML\s*\+=\s*(?:block\.value|text|newValue)/g, desc: 'Appending innerHTML with user data' }
];
let unsafeUsage = false;
let unsafeDetails = [];
dangerousPatterns.forEach(test => {
    if (test.pattern.test(editorContent)) {
        unsafeUsage = true;
        unsafeDetails.push(test.desc);
    }
});
if (!unsafeUsage) {
    console.log("  ✓ No unsafe innerHTML usage with user data");
    passed++;
} else {
    console.log("  ✗ Unsafe innerHTML usage detected:");
    unsafeDetails.forEach(detail => console.log(`    - ${detail}`));
    failed++;
}

// Test 9: Verify textContent is used for safe DOM manipulation
console.log("\nTest 9: Verifying textContent usage for safety...");
const textContentPattern = /\.textContent\s*=/g;
const textContentCount = countOccurrences(editorContent, textContentPattern);
if (textContentCount >= 3) {
    console.log(`  ✓ textContent used ${textContentCount} times (safe DOM manipulation)`);
    passed++;
} else {
    console.log(`  ✗ textContent used only ${textContentCount} times (expected at least 3)`);
    failed++;
}

// Test 10: Verify screen reader support
console.log("\nTest 10: Checking for screen reader announcements...");
const hasAriaLive = /aria-live=["'](?:polite|assertive)["']/.test(editorContent);
const hasScreenReaderFunc = /function\s+announceToScreenReader/.test(editorContent);
if (hasAriaLive || hasScreenReaderFunc) {
    console.log("  ✓ Screen reader support properly configured");
    passed++;
} else {
    console.log("  ✗ Screen reader support missing or incomplete");
    failed++;
}

// Summary
console.log(`\n${'='.repeat(60)}`);
const totalPassed = moduleTestsPassed + helperTestsPassed + passed;
const totalFailed = moduleTestsFailed + helperTestsFailed + failed;
const totalTests = totalPassed + totalFailed;
console.log(`Results: ${totalPassed}/${totalTests} tests passed`);
console.log(`  - Module compatibility: ${moduleTestsPassed}/2`);
console.log(`  - Helper functions: ${helperTestsPassed}/10`);
console.log(`  - Integration tests: ${passed}/10`);
console.log(`${'='.repeat(60)}`);

if (totalFailed > 0) {
    console.log(`\n✗ ${totalFailed} test(s) failed`);
    process.exit(1);
}

console.log("\n✅ All tests passed!");
process.exit(0);
