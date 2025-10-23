#!/usr/bin/env node
/**
 * CodeQL-style security scanner for innerHTML usage
 * Scans editor.html for potentially dangerous innerHTML patterns
 * 
 * Dependencies: None - uses only built-in Node.js APIs
 */

const fs = require('fs');
const path = require('path');

console.log("🔍 Running innerHTML security scan...\n");

// Read editor.html
const editorPath = path.join(__dirname, '..', 'editor.html');
const content = fs.readFileSync(editorPath, 'utf8');

let issues = 0;

// Test 1: Check for innerHTML with concatenation
console.log("Test 1: Checking for innerHTML with string concatenation...");
const concatPattern = /innerHTML\s*[+]=|innerHTML\s*=\s*[^'"`]*\+/g;
const concatMatches = content.match(concatPattern);
if (concatMatches && concatMatches.length > 0) {
    console.log(`  ✗ Found ${concatMatches.length} potentially unsafe innerHTML concatenation(s)`);
    concatMatches.forEach(match => console.log(`    - ${match}`));
    issues++;
} else {
    console.log("  ✓ No innerHTML concatenation detected");
}

// Test 2: Check for insertAdjacentHTML usage
console.log("\nTest 2: Checking for insertAdjacentHTML...");
if (content.includes('insertAdjacentHTML')) {
    console.log("  ✗ Found insertAdjacentHTML usage (potentially unsafe)");
    issues++;
} else {
    console.log("  ✓ No insertAdjacentHTML usage detected");
}

// Test 3: Check for document.write usage
console.log("\nTest 3: Checking for document.write...");
if (content.includes('document.write')) {
    console.log("  ✗ Found document.write usage (potentially unsafe)");
    issues++;
} else {
    console.log("  ✓ No document.write usage detected");
}

// Test 4: Verify textContent is preferred over innerHTML
console.log("\nTest 4: Verifying textContent usage...");
const textContentCount = (content.match(/textContent\s*=/g) || []).length;
const innerHTMLCount = (content.match(/innerHTML\s*=/g) || []).length;
console.log(`  - textContent assignments: ${textContentCount}`);
console.log(`  - innerHTML assignments: ${innerHTMLCount}`);
if (textContentCount > innerHTMLCount) {
    console.log("  ✓ textContent is preferred over innerHTML");
} else {
    console.log("  ⚠ Consider using textContent more often for XSS protection");
}

// Test 5: Check that innerHTML is only in clearning or escapeHtml contexts
console.log("\nTest 5: Verifying innerHTML is only in safe contexts...");
const innerHTMLLines = [];
const lines = content.split('\n');
lines.forEach((line, index) => {
    if (line.includes('innerHTML') && !line.includes('//')) {
        innerHTMLLines.push({ num: index + 1, content: line.trim() });
    }
});

let unsafeInnerHTML = 0;
innerHTMLLines.forEach(item => {
    const line = item.content;
    // Safe patterns: clearing (= '') or inside escapeHtml function
    const isSafe = line.includes("innerHTML = ''") || 
                   line.includes("innerHTML = \"\"") ||
                   line.includes('return div.innerHTML') ||
                   line.includes('Element innerHTML') || // Comment
                   line.includes('* - '); // Comment
    
    if (!isSafe) {
        console.log(`  ⚠ Line ${item.num}: ${line}`);
        unsafeInnerHTML++;
    }
});

if (unsafeInnerHTML === 0) {
    console.log("  ✓ All innerHTML usage appears safe");
} else {
    console.log(`  ⚠ Found ${unsafeInnerHTML} potentially unsafe innerHTML usage(s)`);
}

// Summary
console.log(`\n${'='.repeat(50)}`);
console.log(`Security scan complete: ${issues} critical issues found`);
console.log(`${'='.repeat(50)}`);

if (issues > 0) {
    console.log("\n✗ Security scan failed");
    process.exit(1);
}

console.log("\n✅ Security scan passed!");
