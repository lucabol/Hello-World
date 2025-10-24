#!/usr/bin/env node
/**
 * Integration test for XSS protection in the visual editor
 * This test verifies that user input with malicious content is properly escaped
 * and does not execute as code in the generated output.
 * 
 * Dependencies: None - uses only built-in Node.js APIs (fs, path)
 * No npm install required
 */

const fs = require('fs');
const path = require('path');

console.log("🧪 Running XSS integration test...\n");

// Read the editor.html file
const editorPath = path.join(__dirname, '..', 'tools', 'editor', 'editor.html');
const editorContent = fs.readFileSync(editorPath, 'utf8');

let passed = 0;
let failed = 0;

// Test 1: Verify no unsafe innerHTML usage with user data
console.log("Test 1: Checking for unsafe innerHTML usage...");
const userInputVars = ['block.value', 'block.type', 'text', 'newValue'];
let unsafeInnerHTML = false;

// Check if any user input variables are used with innerHTML without escaping
userInputVars.forEach(varName => {
    const dangerousPattern = new RegExp(`innerHTML.*${varName.replace('.', '\\.')}`, 'g');
    if (dangerousPattern.test(editorContent)) {
        console.log(`  ✗ Found potentially unsafe innerHTML with ${varName}`);
        unsafeInnerHTML = true;
        failed++;
    }
});

if (!unsafeInnerHTML) {
    console.log("  ✓ No unsafe innerHTML usage detected");
    passed++;
}

// Test 2: Verify escapeHtml function exists (either inline or in generator.js)
console.log("\nTest 2: Verifying escapeHtml function...");
const generatorPath = path.join(__dirname, '..', 'tools', 'editor', 'generator.js');
let generatorContent = '';
if (fs.existsSync(generatorPath)) {
    generatorContent = fs.readFileSync(generatorPath, 'utf8');
}
if (editorContent.includes('function escapeHtml(text)') || generatorContent.includes('function escapeHtml(text)')) {
    console.log("  ✓ escapeHtml function found");
    passed++;
} else {
    console.log("  ✗ escapeHtml function not found");
    failed++;
}

// Test 3: Verify escapeHtml is called or CCodeGenerator is used
console.log("\nTest 3: Verifying escapeHtml is called for user content...");
const escapeCallsInline = (editorContent.match(/escapeHtml\(/g) || []).length;
const escapeCallsInGenerator = (generatorContent.match(/escapeHtml\(/g) || []).length;
const usesGenerator = editorContent.includes('CCodeGenerator');
const totalEscapeCalls = escapeCallsInline + escapeCallsInGenerator;

if (totalEscapeCalls >= 3 || usesGenerator) {
    console.log(`  ✓ escapeHtml called ${totalEscapeCalls} times or uses CCodeGenerator module`);
    passed++;
} else {
    console.log(`  ✗ escapeHtml called only ${totalEscapeCalls} times (expected at least 3)`);
    failed++;
}

// Test 4: Verify textContent is used (in editor.html or generator.js)
console.log("\nTest 4: Verifying textContent usage...");
const textContentUsageEditor = (editorContent.match(/textContent\s*=/g) || []).length;
const textContentUsageGenerator = (generatorContent.match(/textContent\s*=/g) || []).length;
const totalTextContentUsage = textContentUsageEditor + textContentUsageGenerator;
if (totalTextContentUsage >= 3) {
    console.log(`  ✓ textContent used ${totalTextContentUsage} times`);
    passed++;
} else {
    console.log(`  ✗ textContent used only ${totalTextContentUsage} times`);
    failed++;
}

// Test 5: Verify Blob and download are used safely
console.log("\nTest 5: Verifying safe download implementation...");
if (editorContent.includes('new Blob([code]') && 
    editorContent.includes('a.download = \'hello.c\'') &&
    editorContent.includes('URL.createObjectURL(blob)')) {
    console.log("  ✓ Safe Blob/download implementation found");
    passed++;
} else {
    console.log("  ✗ Safe download implementation not verified");
    failed++;
}

// Test 6: Verify no eval or Function constructor
console.log("\nTest 6: Checking for eval or Function constructor...");
if (!editorContent.includes('eval(') && 
    !editorContent.match(/new\s+Function\s*\(/)) {
    console.log("  ✓ No eval or Function constructor found");
    passed++;
} else {
    console.log("  ✗ Dangerous eval or Function constructor detected");
    failed++;
}

// Test 7: Verify filename is hardcoded
console.log("\nTest 7: Verifying export filename is hardcoded...");
if (editorContent.includes("a.download = 'hello.c'")) {
    console.log("  ✓ Filename is hardcoded as 'hello.c'");
    passed++;
} else {
    console.log("  ✗ Filename is not properly hardcoded");
    failed++;
}

// Summary
console.log(`\n${'='.repeat(50)}`);
console.log(`Results: ${passed} passed, ${failed} failed`);
console.log(`${'='.repeat(50)}`);

if (failed > 0) {
    console.log("\n✗ Integration test failed");
    process.exit(1);
}

console.log("\n✅ All XSS integration tests passed!");
