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
 * @param {string} html - The HTML content to search
 * @param {string} tagName - Tag name to find (e.g., 'script', 'meta')
 * @param {Object} attributes - Attributes to validate {name: value}
 * @returns {boolean} - True if element with attributes is found
 */
function parseHtmlForElement(html, tagName, attributes = {}) {
    // Build a flexible pattern that accepts attribute variations
    const attrPatterns = Object.entries(attributes).map(([key, value]) => {
        if (value === true) {
            // Just check if attribute exists (boolean attribute or any value)
            return `${key}(?:=|\\s|>)`;
        }
        // Accept single or double quotes, and escape special regex chars in value
        const escapedValue = value.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
        return `${key}\\s*=\\s*["']${escapedValue}["']`;
    });
    
    // Allow any attributes before, between, and after our required attributes
    const attrPattern = attrPatterns.length > 0 ? attrPatterns.join('[^>]*') : '';
    const pattern = new RegExp(`<${tagName}[^>]*${attrPattern}[^>]*>`, 'i');
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
console.log(`Results: ${passed} passed, ${failed} failed`);
console.log(`${'='.repeat(60)}`);

if (failed > 0) {
    console.log("\n✗ Integration tests failed");
    process.exit(1);
}

console.log("\n✅ All integration tests passed!");
process.exit(0);
