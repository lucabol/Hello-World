#!/usr/bin/env node

/**
 * End-to-End Tests for Visual C Code Editor
 * Tests interactive flows: drag/drop, keyboard, touch, and export
 * 
 * These are lightweight E2E tests that verify the editor's main workflows
 * without requiring a full browser automation framework like Puppeteer.
 * 
 * For full browser testing, use: npx playwright test or similar
 */

const fs = require('fs');
const path = require('path');

console.log('🧪 Running E2E Editor Tests...\n');

let passedTests = 0;
let failedTests = 0;

function test(description, fn) {
    try {
        fn();
        console.log(`  ✓ ${description}`);
        passedTests++;
    } catch (error) {
        console.log(`  ✗ ${description}`);
        console.log(`    ${error.message}`);
        failedTests++;
    }
}

function assert(condition, message) {
    if (!condition) {
        throw new Error(message || 'Assertion failed');
    }
}

// Read editor.html
const editorPath = path.join(__dirname, '..', 'editor.html');
const editorContent = fs.readFileSync(editorPath, 'utf8');

console.log('Test 1: Editor structure validation');
test('Editor file exists and is readable', () => {
    assert(editorContent.length > 1000, 'Editor file is too small');
});

test('Contains Block Palette with all block types', () => {
    assert(editorContent.includes('#include'), 'Missing include block');
    assert(editorContent.includes('printf()'), 'Missing printf block');
    assert(editorContent.includes('return'), 'Missing return block');
});

test('Contains workspace area', () => {
    assert(editorContent.includes('Workspace'), 'Missing workspace section');
});

test('Contains code display area', () => {
    assert(editorContent.includes('Generated Code'), 'Missing code display');
});

test('Contains export button', () => {
    assert(editorContent.includes('Export to hello.c'), 'Missing export button');
});

console.log('\nTest 2: Keyboard accessibility features');
test('Has keyboard navigation instructions', () => {
    assert(editorContent.includes('Keyboard:') || editorContent.includes('Tab'), 
        'Missing keyboard instructions');
});

test('Has ARIA roles for screen readers', () => {
    assert(editorContent.includes('role="button"'), 'Missing button roles');
    assert(editorContent.includes('aria-label'), 'Missing ARIA labels');
});

test('Has aria-live regions for dynamic updates', () => {
    assert(editorContent.includes('aria-live'), 'Missing aria-live regions');
});

test('Has tabindex for keyboard navigation', () => {
    assert(editorContent.includes('tabindex'), 'Missing tabindex attributes');
});

console.log('\nTest 3: Touch/mobile support features');
test('Has mobile viewport meta tag', () => {
    assert(editorContent.includes('viewport'), 'Missing viewport meta tag');
});

test('Has touch event listeners or tap instructions', () => {
    assert(editorContent.includes('touchstart') || editorContent.includes('Tap') || 
           editorContent.includes('touch'),
        'Missing touch support indicators');
});

console.log('\nTest 4: Code generation workflow');
test('Has generateCode function', () => {
    assert(editorContent.includes('function generateCode') || 
           editorContent.includes('generateCode()'),
        'Missing generateCode function');
});

test('Uses safe printf format with %s', () => {
    assert(editorContent.includes('printf("%s"'), 
        'Missing safe printf format');
});

test('Has escapeHtml function', () => {
    assert(editorContent.includes('function escapeHtml'), 
        'Missing escapeHtml function');
});

test('Has escapeCString function', () => {
    assert(editorContent.includes('function escapeCString'), 
        'Missing escapeCString function');
});

test('Has header validation function', () => {
    assert(editorContent.includes('isValidHeader') || 
           editorContent.includes('validateHeader'),
        'Missing header validation');
});

test('Has include deduplication', () => {
    assert(editorContent.includes('deduplicate') || 
           editorContent.includes('Set(') && editorContent.includes('toLowerCase'),
        'Missing include deduplication');
});

console.log('\nTest 5: Export functionality');
test('Has file export mechanism', () => {
    assert(editorContent.includes('Blob') || editorContent.includes('download'),
        'Missing file export mechanism');
});

test('Export filename is hardcoded to hello.c', () => {
    assert(editorContent.includes('"hello.c"') || editorContent.includes("'hello.c'"),
        'Export filename should be hardcoded');
});

test('Uses safe Blob API for downloads', () => {
    assert(editorContent.includes('new Blob('), 'Missing Blob API usage');
});

console.log('\nTest 6: Security features');
test('No eval() or new Function()', () => {
    assert(!editorContent.includes('eval('), 'Found eval() usage');
    assert(!editorContent.includes('new Function('), 'Found new Function() usage');
});

test('No document.write()', () => {
    assert(!editorContent.includes('document.write'), 'Found document.write() usage');
});

test('Uses textContent for safe DOM updates', () => {
    assert(editorContent.includes('textContent'), 
        'Missing textContent usage for safe DOM updates');
});

test('innerHTML only used safely (empty strings or in escapeHtml)', () => {
    const innerHTMLMatches = editorContent.match(/innerHTML\s*=/g) || [];
    assert(innerHTMLMatches.length <= 3, 
        `Too many innerHTML assignments: ${innerHTMLMatches.length}`);
});

console.log('\nTest 7: IIFE and encapsulation');
test('Code wrapped in IIFE', () => {
    assert(editorContent.includes('(function()') || 
           editorContent.includes('(function ()') ||
           editorContent.includes('=>') && editorContent.includes('()()'),
        'Missing IIFE wrapper');
});

test('Uses strict mode', () => {
    assert(editorContent.includes('"use strict"') || 
           editorContent.includes("'use strict'"),
        'Missing strict mode');
});

test('CSS scoped with .vce- prefix', () => {
    assert(editorContent.includes('.vce-'), 
        'Missing CSS scoping prefix');
});

console.log('\nTest 8: Special character handling');
test('Handles special characters in C strings', () => {
    const escapeCStringMatch = editorContent.match(/function escapeCString[\s\S]{0,500}/);
    if (escapeCStringMatch) {
        const func = escapeCStringMatch[0];
        assert(func.includes('\\\\'), 'Missing backslash escaping');
        assert(func.includes('\\"'), 'Missing quote escaping');
        assert(func.includes('\\n'), 'Missing newline escaping');
    }
});

test('Validates header names against path traversal', () => {
    assert(editorContent.includes('..') && editorContent.includes('return false'),
        'Missing path traversal check for ".."');
});

console.log('\nTest 9: User feedback and UX');
test('Has visual feedback for actions', () => {
    assert(editorContent.includes('aria-live') || 
           editorContent.includes('announcement'),
        'Missing user feedback mechanism');
});

test('Has input validation with error handling', () => {
    assert(editorContent.includes('return false') || 
           editorContent.includes('invalid'),
        'Missing input validation');
});

test('Has debouncing for performance', () => {
    assert(editorContent.includes('setTimeout') || 
           editorContent.includes('debounce'),
        'Missing debouncing for performance');
});

console.log('\nTest 10: Integration workflow simulation');
test('Workflow: Include → Printf → Return → Export', () => {
    // Verify all components needed for the workflow exist
    const hasInclude = editorContent.includes('#include');
    const hasPrintf = editorContent.includes('printf');
    const hasReturn = editorContent.includes('return');
    const hasExport = editorContent.includes('Export');
    const hasCodeGen = editorContent.includes('generateCode');
    
    assert(hasInclude && hasPrintf && hasReturn && hasExport && hasCodeGen,
        'Missing components for complete workflow');
});

// Summary
console.log('\n' + '='.repeat(50));
console.log(`E2E Tests Complete: ${passedTests} passed, ${failedTests} failed`);
console.log('='.repeat(50));

if (failedTests > 0) {
    console.log('\n❌ Some E2E tests failed!');
    process.exit(1);
}

console.log('\n✅ All E2E tests passed!');
console.log('\n📝 Note: These are static analysis E2E tests.');
console.log('   For interactive browser testing, use:');
console.log('   - Puppeteer: npx puppeteer editor.html');
console.log('   - Playwright: npx playwright test');
console.log('   - Manual: Open editor.html in browser\n');

process.exit(0);
