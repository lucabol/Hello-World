#!/usr/bin/env node
/**
 * Unit tests for the escapeHtml function used in editor.html
 * This test can be run with Node.js: node test/test_escape_html.js
 * 
 * Dependencies: None - uses only built-in Node.js APIs (no npm install required)
 * Minimum Node.js version: 14.x (ES6+ features)
 */

// Check if running in Node.js or browser
const isNode = typeof process !== 'undefined' && process.versions && process.versions.node;

// escapeHtml implementation (same as in editor.html)
function escapeHtml(text) {
    if (isNode) {
        // Node.js implementation using basic string replacement
        return text
            .replace(/&/g, '&amp;')
            .replace(/</g, '&lt;')
            .replace(/>/g, '&gt;')
            .replace(/"/g, '&quot;')
            .replace(/'/g, '&#039;');
    } else {
        // Browser implementation using DOM
        const div = document.createElement('div');
        div.textContent = text;
        return div.innerHTML;
    }
}

// Test cases
const testCases = [
    {
        name: "XSS script tag",
        input: "<script>alert('XSS')</script>",
        expected: "&lt;script&gt;alert(&#039;XSS&#039;)&lt;/script&gt;"
    },
    {
        name: "HTML img tag with onerror",
        input: "<img src=x onerror=alert('XSS')>",
        expected: "&lt;img src=x onerror=alert(&#039;XSS&#039;)&gt;"
    },
    {
        name: "Double quotes",
        input: 'printf("Hello world!");',
        expected: 'printf(&quot;Hello world!&quot;);'
    },
    {
        name: "Single quotes",
        input: "printf('Hello');",
        expected: "printf(&#039;Hello&#039;);"
    },
    {
        name: "Ampersand",
        input: "x & y",
        expected: "x &amp; y"
    },
    {
        name: "Less than and greater than",
        input: "x < y && y > z",
        expected: "x &lt; y &amp;&amp; y &gt; z"
    },
    {
        name: "Mixed special characters",
        input: "<div class=\"test\" onclick='alert(\"XSS\")'>",
        expected: "&lt;div class=&quot;test&quot; onclick=&#039;alert(&quot;XSS&quot;)&#039;&gt;"
    },
    {
        name: "Normal text without special chars",
        input: "Hello world",
        expected: "Hello world"
    },
    {
        name: "Include directive",
        input: "stdio.h",
        expected: "stdio.h"
    },
    {
        name: "Empty string",
        input: "",
        expected: ""
    }
];

// Run tests
let passed = 0;
let failed = 0;

console.log("🧪 Running escapeHtml() unit tests...\n");

testCases.forEach((testCase, index) => {
    const result = escapeHtml(testCase.input);
    const success = result === testCase.expected;
    
    if (success) {
        console.log(`✓ Test ${index + 1}: ${testCase.name}`);
        passed++;
    } else {
        console.log(`✗ Test ${index + 1}: ${testCase.name}`);
        console.log(`  Input:    "${testCase.input}"`);
        console.log(`  Expected: "${testCase.expected}"`);
        console.log(`  Got:      "${result}"`);
        failed++;
    }
});

console.log(`\n${passed} passed, ${failed} failed`);

if (failed > 0) {
    process.exit(1);
}

console.log("\n✅ All escapeHtml() tests passed!");
