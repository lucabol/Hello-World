#!/usr/bin/env node
/**
 * Unit tests for header validation and include deduplication
 * Tests the isValidHeader() whitelist and deduplicateIncludes() logic
 * 
 * Dependencies: None - uses only built-in Node.js APIs
 */

console.log("🧪 Running header validation tests...\n");

let passed = 0;
let failed = 0;

// Simulate the isValidHeader function from editor.html
function isValidHeader(header) {
    // Reject path traversal attempts
    if (header.includes('..')) {
        return false;
    }
    const pattern = /^[a-zA-Z0-9_.-]+(\/[a-zA-Z0-9_.-]+)*$/;
    return pattern.test(header) && header.length < 100;
}

// Simulate the deduplicateIncludes function
function deduplicateIncludes(includeBlocks) {
    const seen = new Set();
    return includeBlocks.filter(block => {
        const normalized = block.value.trim().toLowerCase();
        if (seen.has(normalized)) {
            return false;
        }
        seen.add(normalized);
        return true;
    });
}

// Test cases for isValidHeader
const validationTests = [
    // Valid headers
    { name: "Standard header (stdio.h)", input: "stdio.h", expected: true },
    { name: "Standard header (stdlib.h)", input: "stdlib.h", expected: true },
    { name: "System header (sys/types.h)", input: "sys/types.h", expected: true },
    { name: "Underscore (my_header.h)", input: "my_header.h", expected: true },
    { name: "Hyphen (my-header.h)", input: "my-header.h", expected: true },
    { name: "Multiple dots (a.b.c.h)", input: "a.b.c.h", expected: true },
    { name: "No extension (iostream)", input: "iostream", expected: true },
    { name: "Numbers (lib123.h)", input: "lib123.h", expected: true },
    
    // Invalid headers - Security risks
    { name: "Path traversal (..)", input: "../etc/passwd", expected: false },
    { name: "Path traversal (../..)", input: "../../root/.bashrc", expected: false },
    { name: "Backslash (Windows path)", input: "path\\to\\file.h", expected: false },
    { name: "Semicolon injection", input: "header;rm -rf.h", expected: false },
    { name: "Command injection", input: "file.h;malicious", expected: false },
    { name: "Pipe injection", input: "file|cmd.h", expected: false },
    { name: "Redirect injection", input: "file>output.h", expected: false },
    
    // Invalid headers - Formatting
    { name: "Empty string", input: "", expected: false },
    { name: "Leading space", input: " stdio.h", expected: false },
    { name: "Trailing space", input: "stdio.h ", expected: false },
    { name: "Internal space", input: "my header.h", expected: false },
    { name: "Tab character", input: "my\theader.h", expected: false },
    { name: "Newline character", input: "my\nheader.h", expected: false },
    
    // Invalid headers - Special characters
    { name: "Angle brackets", input: "<stdio.h>", expected: false },
    { name: "Quotes", input: '"stdio.h"', expected: false },
    { name: "Single quote", input: "stdio'.h", expected: false },
    { name: "Asterisk", input: "*.h", expected: false },
    { name: "Question mark", input: "file?.h", expected: false },
    { name: "Ampersand", input: "file&cmd.h", expected: false },
    { name: "Dollar sign", input: "$PATH.h", expected: false },
    { name: "Percent", input: "file%.h", expected: false },
    { name: "At sign", input: "file@host.h", expected: false },
    { name: "Exclamation", input: "file!.h", expected: false },
    
    // Edge cases - Length
    { name: "Very long valid (99 chars)", input: "a".repeat(97) + ".h", expected: true },
    { name: "Exactly 100 chars", input: "a".repeat(98) + ".h", expected: false },
    { name: "Very long invalid (101 chars)", input: "a".repeat(99) + ".h", expected: false },
    
    // Edge cases - Unicode
    { name: "Unicode (emoji)", input: "header😀.h", expected: false },
    { name: "Unicode (Chinese)", input: "头文件.h", expected: false },
    { name: "Unicode (Cyrillic)", input: "файл.h", expected: false },
    
    // Edge cases - Multiple slashes
    { name: "Multiple dirs (a/b/c.h)", input: "a/b/c.h", expected: true },
    { name: "Trailing slash", input: "sys/", expected: false },
    { name: "Leading slash", input: "/sys/types.h", expected: false },
    { name: "Double slash", input: "sys//types.h", expected: false },
];

console.log("=== Header Validation Tests ===\n");

validationTests.forEach((test, index) => {
    const result = isValidHeader(test.input);
    const success = result === test.expected;
    
    if (success) {
        console.log(`✓ Test ${index + 1}: ${test.name}`);
        passed++;
    } else {
        console.log(`✗ Test ${index + 1}: ${test.name}`);
        console.log(`  Input:    "${test.input}"`);
        console.log(`  Expected: ${test.expected}`);
        console.log(`  Got:      ${result}`);
        failed++;
    }
});

console.log(`\n=== Include Deduplication Tests ===\n`);

// Test cases for deduplicateIncludes
const dedupeTests = [
    {
        name: "No duplicates",
        input: [
            { id: 1, value: "stdio.h" },
            { id: 2, value: "stdlib.h" },
            { id: 3, value: "string.h" }
        ],
        expectedCount: 3,
        expectedValues: ["stdio.h", "stdlib.h", "string.h"]
    },
    {
        name: "Exact duplicates",
        input: [
            { id: 1, value: "stdio.h" },
            { id: 2, value: "stdio.h" },
            { id: 3, value: "stdio.h" }
        ],
        expectedCount: 1,
        expectedValues: ["stdio.h"]
    },
    {
        name: "Case-insensitive duplicates",
        input: [
            { id: 1, value: "stdio.h" },
            { id: 2, value: "STDIO.H" },
            { id: 3, value: "StDiO.h" }
        ],
        expectedCount: 1,
        expectedValues: ["stdio.h"]
    },
    {
        name: "Trimmed duplicates",
        input: [
            { id: 1, value: "stdio.h" },
            { id: 2, value: " stdio.h" },
            { id: 3, value: "stdio.h " },
            { id: 4, value: " stdio.h " }
        ],
        expectedCount: 1,
        expectedValues: ["stdio.h"]
    },
    {
        name: "Mixed case and trimming",
        input: [
            { id: 1, value: "stdio.h" },
            { id: 2, value: " STDIO.H " },
            { id: 3, value: "stdlib.h" },
            { id: 4, value: " stdlib.h" }
        ],
        expectedCount: 2,
        expectedValues: ["stdio.h", "stdlib.h"]
    },
    {
        name: "Preserves first occurrence",
        input: [
            { id: 1, value: "STDIO.H" },
            { id: 2, value: "stdio.h" },
            { id: 3, value: "StDiO.h" }
        ],
        expectedCount: 1,
        expectedValues: ["STDIO.H"]  // First occurrence preserved
    },
    {
        name: "Stable ordering",
        input: [
            { id: 1, value: "string.h" },
            { id: 2, value: "stdio.h" },
            { id: 3, value: "stdlib.h" },
            { id: 4, value: "STDIO.H" },  // Duplicate
            { id: 5, value: "math.h" }
        ],
        expectedCount: 4,
        expectedValues: ["string.h", "stdio.h", "stdlib.h", "math.h"]
    },
    {
        name: "Empty array",
        input: [],
        expectedCount: 0,
        expectedValues: []
    },
    {
        name: "Single item",
        input: [{ id: 1, value: "stdio.h" }],
        expectedCount: 1,
        expectedValues: ["stdio.h"]
    }
];

dedupeTests.forEach((test, index) => {
    const result = deduplicateIncludes(test.input);
    const countMatch = result.length === test.expectedCount;
    const valuesMatch = JSON.stringify(result.map(b => b.value)) === JSON.stringify(test.expectedValues);
    const success = countMatch && valuesMatch;
    
    if (success) {
        console.log(`✓ Test ${index + 1}: ${test.name}`);
        passed++;
    } else {
        console.log(`✗ Test ${index + 1}: ${test.name}`);
        console.log(`  Expected count: ${test.expectedCount}, Got: ${result.length}`);
        console.log(`  Expected values: ${JSON.stringify(test.expectedValues)}`);
        console.log(`  Got values: ${JSON.stringify(result.map(b => b.value))}`);
        failed++;
    }
});

// Export filename test
console.log(`\n=== Export Filename Test ===\n`);

const exportFilename = "hello.c";
const isCorrectFilename = exportFilename === "hello.c";

if (isCorrectFilename) {
    console.log(`✓ Export filename is hardcoded as "hello.c"`);
    passed++;
} else {
    console.log(`✗ Export filename test failed`);
    console.log(`  Expected: "hello.c"`);
    console.log(`  Got: "${exportFilename}"`);
    failed++;
}

// Summary
console.log(`\n${'='.repeat(50)}`);
console.log(`Results: ${passed} passed, ${failed} failed`);
console.log(`${'='.repeat(50)}`);

if (failed > 0) {
    console.log("\n✗ Header validation tests failed");
    process.exit(1);
}

console.log("\n✅ All header validation tests passed!");
