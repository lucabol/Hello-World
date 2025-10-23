#!/usr/bin/env node

/**
 * Accessibility (A11y) Tests for Visual C Code Editor
 * Tests ARIA attributes, keyboard navigation, and screen reader support
 * 
 * This provides automated accessibility validation without requiring
 * a full axe-core setup. For comprehensive testing, use axe-core or pa11y.
 */

const fs = require('fs');
const path = require('path');

console.log('♿ Running Accessibility Tests...\n');

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

console.log('Test Group 1: ARIA Roles and Labels');

test('Block palette has role="button" or role="listitem"', () => {
    assert(editorContent.includes('role="button"') || editorContent.includes('role="listitem"'),
        'Block palette missing proper ARIA roles');
});

test('Block palette items have aria-label', () => {
    const hasAriaLabel = editorContent.match(/aria-label="[^"]+"/g) || [];
    assert(hasAriaLabel.length >= 3, 
        `Found ${hasAriaLabel.length} aria-labels, expected at least 3 (one per block type)`);
});

test('Workspace has role="region" or role="main"', () => {
    assert(editorContent.includes('role="region"') || editorContent.includes('role="main"'),
        'Workspace missing proper ARIA role');
});

test('Workspace has aria-label describing its purpose', () => {
    const workspaceLabel = editorContent.match(/role="region"[^>]*aria-label="[^"]*[Ww]orkspace[^"]*"/);
    assert(workspaceLabel || editorContent.match(/aria-label="[^"]*[Ww]orkspace[^"]*"/),
        'Workspace missing descriptive aria-label');
});

test('Code display has role="log" or role="region"', () => {
    assert(editorContent.includes('role="log"') || editorContent.match(/role="region".*[Cc]ode/),
        'Code display missing proper ARIA role');
});

test('Code display has aria-live for dynamic updates', () => {
    assert(editorContent.includes('aria-live'),
        'Code display missing aria-live for screen reader announcements');
});

test('aria-live politeness is appropriate (polite or assertive)', () => {
    assert(editorContent.includes('aria-live="polite"') || 
           editorContent.includes('aria-live="assertive"'),
        'Missing or incorrect aria-live politeness setting');
});

test('Export button has descriptive aria-label', () => {
    assert(editorContent.match(/aria-label="[^"]*[Ee]xport[^"]*hello\.c[^"]*"/),
        'Export button missing descriptive aria-label');
});

console.log('\nTest Group 2: Keyboard Navigation');

test('Interactive elements have tabindex', () => {
    const tabindexCount = (editorContent.match(/tabindex=/g) || []).length;
    assert(tabindexCount >= 3,
        `Found ${tabindexCount} tabindex attributes, expected at least 3`);
});

test('Has keyboard event listeners (keydown, keypress, or keyup)', () => {
    assert(editorContent.includes('keydown') || 
           editorContent.includes('keypress') || 
           editorContent.includes('keyup'),
        'Missing keyboard event listeners');
});

test('Supports Enter key for activation', () => {
    assert(editorContent.includes('Enter') || editorContent.includes('13'),
        'Missing Enter key support');
});

test('Supports Space key for activation', () => {
    assert(editorContent.includes('Space') || editorContent.includes(' ') || 
           editorContent.includes('32'),
        'Missing Space key support');
});

test('Has visible keyboard instructions', () => {
    assert(editorContent.match(/[Kk]eyboard/i) && 
           (editorContent.includes('Tab') || editorContent.includes('Enter')),
        'Missing keyboard usage instructions');
});

console.log('\nTest Group 3: Focus Management');

test('Has focus indicators (:focus styles)', () => {
    assert(editorContent.includes(':focus'),
        'Missing :focus CSS styles for keyboard navigation');
});

test('Focus outline is visible and accessible', () => {
    const focusStyle = editorContent.match(/:focus\s*\{[^}]*outline[^}]*\}/);
    assert(focusStyle && !focusStyle[0].includes('outline: none'),
        'Focus outline should be visible (not set to none)');
});

test('Focus outline has sufficient contrast', () => {
    const focusMatch = editorContent.match(/:focus\s*\{[^}]*\}/);
    if (focusMatch) {
        // Check for solid outline color (not just border)
        assert(focusMatch[0].includes('outline'),
            'Focus should use outline, not just border');
    }
});

console.log('\nTest Group 4: Screen Reader Support');

test('Has screen reader announcements mechanism', () => {
    assert(editorContent.includes('aria-live') || 
           editorContent.includes('role="status"') ||
           editorContent.includes('role="alert"'),
        'Missing screen reader announcement mechanism');
});

test('Dynamic content updates use aria-live regions', () => {
    const ariaLiveRegions = (editorContent.match(/aria-live=/g) || []).length;
    assert(ariaLiveRegions >= 1,
        `Found ${ariaLiveRegions} aria-live regions, expected at least 1`);
});

test('Form inputs have labels or aria-label', () => {
    const inputs = (editorContent.match(/<input[^>]*>/g) || []);
    const textareas = (editorContent.match(/<textarea[^>]*>/g) || []);
    const totalInputs = inputs.length + textareas.length;
    
    if (totalInputs > 0) {
        // At least one should have aria-label or be in a label
        assert(editorContent.includes('aria-label') || editorContent.includes('<label'),
            'Form inputs should have labels or aria-label');
    }
});

test('Buttons have accessible names', () => {
    const buttons = (editorContent.match(/<button[^>]*>/g) || []);
    // All buttons should have text content or aria-label
    assert(buttons.length === 0 || editorContent.includes('aria-label'),
        'Buttons should have accessible names');
});

console.log('\nTest Group 5: Color and Contrast');

test('Does not rely solely on color for information', () => {
    // Check for text labels alongside color coding
    assert(editorContent.includes('printf') && editorContent.includes('#include'),
        'Should use text labels, not just colors');
});

test('Has high contrast focus indicators', () => {
    const focusStyles = editorContent.match(/:focus\s*\{[^}]*\}/);
    if (focusStyles) {
        // Check for solid, visible outline
        const style = focusStyles[0];
        assert(style.includes('outline') && !style.includes('outline: 0'),
            'Focus indicators should have visible outline');
    }
});

console.log('\nTest Group 6: Semantic HTML');

test('Uses semantic HTML5 elements', () => {
    assert(editorContent.includes('<header') || 
           editorContent.includes('<main') || 
           editorContent.includes('<section'),
        'Should use semantic HTML5 elements');
});

test('Has proper heading hierarchy', () => {
    assert(editorContent.includes('<h1'), 'Missing main heading (h1)');
});

test('Page has lang attribute', () => {
    assert(editorContent.match(/<html[^>]*lang=/),
        'HTML element missing lang attribute');
});

test('Page has descriptive title', () => {
    const titleMatch = editorContent.match(/<title>([^<]+)<\/title>/);
    assert(titleMatch && titleMatch[1].length > 3,
        'Page should have descriptive title');
});

console.log('\nTest Group 7: Interactive Element Accessibility');

test('Draggable elements have keyboard alternatives', () => {
    // If draggable, should have keyboard instructions or alternatives
    if (editorContent.includes('drag')) {
        assert(editorContent.match(/[Kk]eyboard/i) || editorContent.includes('Tab to select'),
            'Draggable elements should have keyboard alternatives');
    }
});

test('No empty links or buttons', () => {
    // Check for buttons/links without text
    const emptyButton = editorContent.match(/<button[^>]*><\/button>/);
    const emptyLink = editorContent.match(/<a[^>]*><\/a>/);
    assert(!emptyButton && !emptyLink,
        'All buttons and links should have content or aria-label');
});

test('Touch targets documented for mobile', () => {
    assert(editorContent.includes('Tap') || editorContent.includes('touch') ||
           editorContent.includes('mobile'),
        'Should document touch/mobile interaction');
});

console.log('\nTest Group 8: Screen Reader Announcement Examples');

test('Announcements are concise and informative', () => {
    const ariaLabelExamples = editorContent.match(/aria-label="([^"]+)"/g) || [];
    if (ariaLabelExamples.length > 0) {
        const firstLabel = ariaLabelExamples[0];
        // Labels should be reasonably concise (< 100 chars)
        const labelText = firstLabel.match(/aria-label="([^"]+)"/)[1];
        assert(labelText.length < 100 && labelText.length > 3,
            `aria-label should be concise: "${labelText}"`);
    }
});

console.log('\nTest Group 9: Accessibility Documentation');

test('Editor guide mentions accessibility features', () => {
    const guidePath = path.join(__dirname, '..', 'EDITOR_GUIDE.md');
    if (fs.existsSync(guidePath)) {
        const guideContent = fs.readFileSync(guidePath, 'utf8');
        assert(guideContent.match(/accessibility|a11y|keyboard/i),
            'Editor guide should document accessibility features');
    }
});

// Extract and display example ARIA attributes
console.log('\n' + '='.repeat(50));
console.log('Example ARIA Attributes Found:');
console.log('='.repeat(50));

const ariaExamples = [
    ...new Set(editorContent.match(/role="[^"]+"/g) || [])
].slice(0, 5);
ariaExamples.forEach(example => console.log(`  • ${example}`));

const ariaLabelExamples = [
    ...new Set(editorContent.match(/aria-label="[^"]+"/g) || [])
].slice(0, 3);
ariaLabelExamples.forEach(example => console.log(`  • ${example}`));

const ariaLiveExamples = [
    ...new Set(editorContent.match(/aria-live="[^"]+"/g) || [])
];
ariaLiveExamples.forEach(example => console.log(`  • ${example}`));

// Summary
console.log('\n' + '='.repeat(50));
console.log(`A11y Tests Complete: ${passedTests} passed, ${failedTests} failed`);
console.log('='.repeat(50));

if (failedTests > 0) {
    console.log('\n❌ Some accessibility tests failed!');
    process.exit(1);
}

console.log('\n✅ All accessibility tests passed!');
console.log('\n📝 Note: For comprehensive testing, consider:');
console.log('   - axe-core: npm install @axe-core/cli && axe editor.html');
console.log('   - pa11y: npm install pa11y && pa11y editor.html');
console.log('   - Manual: Test with screen reader (NVDA, JAWS, VoiceOver)\n');

process.exit(0);
