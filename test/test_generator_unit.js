#!/usr/bin/env node
/**
 * Unit tests for the C code generator module
 * Tests the core code generation logic without browser dependencies
 * 
 * Run with: node test/test_generator_unit.js
 * Dependencies: None - uses only built-in Node.js APIs
 */

const generator = require('../tools/editor/generator.js');
const { execSync } = require('child_process');
const fs = require('fs');
const path = require('path');
const os = require('os');

console.log("🧪 Running C Code Generator Unit Tests...\n");

let passed = 0;
let failed = 0;

function assert(condition, testName, expected, actual) {
    if (condition) {
        console.log(`  ✓ ${testName}`);
        passed++;
        return true;
    } else {
        console.log(`  ✗ ${testName}`);
        if (expected !== undefined && actual !== undefined) {
            console.log(`    Expected: ${JSON.stringify(expected)}`);
            console.log(`    Actual:   ${JSON.stringify(actual)}`);
        }
        failed++;
        return false;
    }
}

// Test 1: escapeHtml - Basic HTML escaping
console.log("Test Suite 1: HTML Escaping");
assert(
    generator.escapeHtml('<script>') === '&lt;script&gt;',
    'Escapes angle brackets',
    '&lt;script&gt;',
    generator.escapeHtml('<script>')
);

assert(
    generator.escapeHtml('a & b') === 'a &amp; b',
    'Escapes ampersands',
    'a &amp; b',
    generator.escapeHtml('a & b')
);

assert(
    generator.escapeHtml('"quotes"') === '&quot;quotes&quot;',
    'Escapes double quotes',
    '&quot;quotes&quot;',
    generator.escapeHtml('"quotes"')
);

assert(
    generator.escapeHtml("'single'") === '&#039;single&#039;',
    'Escapes single quotes',
    '&#039;single&#039;',
    generator.escapeHtml("'single'")
);

assert(
    generator.escapeHtml('<img src=x onerror=alert(1)>') === '&lt;img src=x onerror=alert(1)&gt;',
    'Escapes XSS attack vector',
    '&lt;img src=x onerror=alert(1)&gt;',
    generator.escapeHtml('<img src=x onerror=alert(1)>')
);

// Test 2: escapePrintfFormat - Printf format specifier escaping
console.log("\nTest Suite 2: Printf Format Escaping");
assert(
    generator.escapePrintfFormat('50% complete') === '50%% complete',
    'Escapes single percent sign',
    '50%% complete',
    generator.escapePrintfFormat('50% complete')
);

assert(
    generator.escapePrintfFormat('100%') === '100%%',
    'Escapes trailing percent',
    '100%%',
    generator.escapePrintfFormat('100%')
);

assert(
    generator.escapePrintfFormat('%s %d %f') === '%%s %%d %%f',
    'Escapes multiple format specifiers',
    '%%s %%d %%f',
    generator.escapePrintfFormat('%s %d %f')
);

assert(
    generator.escapePrintfFormat('no percents') === 'no percents',
    'Leaves strings without % unchanged',
    'no percents',
    generator.escapePrintfFormat('no percents')
);

// Test 3: getPlaceholder
console.log("\nTest Suite 3: Placeholder Text");
assert(
    generator.getPlaceholder('include') === 'stdio.h',
    'Returns correct placeholder for include',
    'stdio.h',
    generator.getPlaceholder('include')
);

assert(
    generator.getPlaceholder('printf') === 'Hello world!',
    'Returns correct placeholder for printf',
    'Hello world!',
    generator.getPlaceholder('printf')
);

assert(
    generator.getPlaceholder('return') === '0',
    'Returns correct placeholder for return',
    '0',
    generator.getPlaceholder('return')
);

assert(
    generator.getPlaceholder('unknown') === '',
    'Returns empty string for unknown type',
    '',
    generator.getPlaceholder('unknown')
);

// Test 4: generateCode - Basic code generation
console.log("\nTest Suite 4: Basic Code Generation");

const simpleBlocks = [
    { id: 0, type: 'include', value: 'stdio.h' },
    { id: 1, type: 'printf', value: 'Hello world!' }
];
const simpleCode = generator.generateCode(simpleBlocks);
assert(
    simpleCode.includes('#include <stdio.h>'),
    'Includes stdio.h header',
    '#include <stdio.h>',
    simpleCode.split('\n')[0]
);
assert(
    simpleCode.includes('printf("Hello world!");'),
    'Includes printf statement',
    'printf("Hello world!");',
    simpleCode.match(/printf\([^)]+\);/)?.[0]
);
assert(
    simpleCode.includes('int main(){'),
    'Includes main function',
    'int main(){',
    simpleCode.match(/int main\(\)\{/)?.[0]
);

// Test 5: Auto-include feature
console.log("\nTest Suite 5: Auto-Include Feature");
const blocksWithoutInclude = [
    { id: 0, type: 'printf', value: 'Test' }
];
const codeWithAutoInclude = generator.generateCode(blocksWithoutInclude);
assert(
    codeWithAutoInclude.includes('#include <stdio.h>'),
    'Auto-adds stdio.h when printf is used',
    true,
    codeWithAutoInclude.includes('#include <stdio.h>')
);

const blocksWithInclude = [
    { id: 0, type: 'include', value: 'stdio.h' },
    { id: 1, type: 'printf', value: 'Test' }
];
const codeWithManualInclude = generator.generateCode(blocksWithInclude);
const includeCount = (codeWithManualInclude.match(/#include <stdio\.h>/g) || []).length;
assert(
    includeCount === 1,
    'Does not duplicate stdio.h when manually included',
    1,
    includeCount
);

// Test 6: XSS Protection in generated code
console.log("\nTest Suite 6: XSS Protection");
const xssBlocks = [
    { id: 0, type: 'include', value: 'stdio.h' },
    { id: 1, type: 'printf', value: '<script>alert("xss")</script>' }
];
const xssCode = generator.generateCode(xssBlocks);
assert(
    xssCode.includes('&lt;script&gt;'),
    'Escapes HTML tags in printf',
    true,
    xssCode.includes('&lt;script&gt;')
);
assert(
    !xssCode.includes('<script>'),
    'Does not include raw script tags',
    false,
    xssCode.includes('<script>')
);

// Test 7: Printf format specifier safety
console.log("\nTest Suite 7: Printf Format Specifier Safety");
const percentBlocks = [
    { id: 0, type: 'include', value: 'stdio.h' },
    { id: 1, type: 'printf', value: '50% complete' }
];
const percentCode = generator.generateCode(percentBlocks);
assert(
    percentCode.includes('50%% complete'),
    'Escapes percent signs in printf',
    true,
    percentCode.includes('50%% complete')
);

// Test 8: Return statement handling
console.log("\nTest Suite 8: Return Statement Handling");
const returnBlocks = [
    { id: 0, type: 'include', value: 'stdio.h' },
    { id: 1, type: 'printf', value: 'Test' },
    { id: 2, type: 'return', value: '0' }
];
const returnCode = generator.generateCode(returnBlocks);
assert(
    returnCode.includes('return 0;'),
    'Includes return statement',
    true,
    returnCode.includes('return 0;')
);

const multipleReturns = [
    { id: 0, type: 'return', value: '1' },
    { id: 1, type: 'return', value: '0' }
];
const multiReturnCode = generator.generateCode(multipleReturns);
assert(
    multiReturnCode.includes('return 0;'),
    'Uses last return statement',
    true,
    multiReturnCode.includes('return 0;')
);
assert(
    !multiReturnCode.includes('return 1;'),
    'Ignores earlier return statements',
    false,
    multiReturnCode.includes('return 1;')
);

// Test 9: Multiple includes
console.log("\nTest Suite 9: Multiple Includes");
const multiIncludeBlocks = [
    { id: 0, type: 'include', value: 'stdio.h' },
    { id: 1, type: 'include', value: 'stdlib.h' },
    { id: 2, type: 'printf', value: 'Test' }
];
const multiIncludeCode = generator.generateCode(multiIncludeBlocks);
assert(
    multiIncludeCode.includes('#include <stdio.h>') &&
    multiIncludeCode.includes('#include <stdlib.h>'),
    'Includes multiple headers',
    true,
    multiIncludeCode.includes('#include <stdio.h>') && multiIncludeCode.includes('#include <stdlib.h>')
);

// Test 10: Edge cases - Special characters
console.log("\nTest Suite 10: Edge Cases");
const specialCharBlocks = [
    { id: 0, type: 'include', value: 'stdio.h' },
    { id: 1, type: 'printf', value: 'Line1\\nLine2' }
];
const specialCharCode = generator.generateCode(specialCharBlocks);
assert(
    specialCharCode.includes('Line1\\nLine2'),
    'Preserves backslash-n sequences',
    true,
    specialCharCode.includes('Line1\\nLine2')
);

const emptyBlocks = [];
const emptyCode = generator.generateCode(emptyBlocks);
assert(
    emptyCode === 'int main(){\n}\n',
    'Generates valid empty program',
    'int main(){\n}\n',
    emptyCode
);

// Test 11: Compilation test - Generate and compile actual code
console.log("\nTest Suite 11: Compilation Validation");
const compilationBlocks = [
    { id: 0, type: 'include', value: 'stdio.h' },
    { id: 1, type: 'printf', value: 'Hello world!' }
];
const compilationCode = generator.generateCode(compilationBlocks);

const tmpDir = fs.mkdtempSync(path.join(os.tmpdir(), 'cgen-test-'));
const testFile = path.join(tmpDir, 'test.c');
const testExe = path.join(tmpDir, 'test');

try {
    fs.writeFileSync(testFile, compilationCode);
    
    // Try to compile
    try {
        execSync(`gcc -Wall -Wextra -Wpedantic -Werror -o "${testExe}" "${testFile}"`, {
            stdio: 'pipe'
        });
        assert(true, 'Generated code compiles successfully');
        
        // Try to run
        try {
            const output = execSync(`"${testExe}"`, { encoding: 'utf-8' });
            assert(
                output === 'Hello world!',
                'Compiled program produces correct output',
                'Hello world!',
                output
            );
        } catch (runError) {
            assert(false, 'Compiled program runs successfully');
        }
    } catch (compileError) {
        assert(false, 'Generated code compiles successfully');
        console.log(`    Compilation error: ${compileError.message}`);
    }
} finally {
    // Cleanup
    try {
        fs.unlinkSync(testFile);
        if (fs.existsSync(testExe)) fs.unlinkSync(testExe);
        fs.rmdirSync(tmpDir);
    } catch (e) {
        // Ignore cleanup errors
    }
}

// Test 12: Complex edge case - All features combined
console.log("\nTest Suite 12: Complex Integration");
const complexBlocks = [
    { id: 0, type: 'include', value: 'stdio.h' },
    { id: 1, type: 'include', value: 'stdlib.h' },
    { id: 2, type: 'printf', value: 'Progress: 50% <done>' },
    { id: 3, type: 'printf', value: 'Line\\n' },
    { id: 4, type: 'return', value: '0' }
];
const complexCode = generator.generateCode(complexBlocks);
assert(
    complexCode.includes('50%% &lt;done&gt;'),
    'Combines format escaping and HTML escaping',
    true,
    complexCode.includes('50%% &lt;done&gt;')
);

// Summary
console.log(`\n${'='.repeat(60)}`);
console.log(`Results: ${passed} passed, ${failed} failed`);
console.log(`${'='.repeat(60)}`);

if (failed > 0) {
    console.log("\n✗ Unit tests failed");
    process.exit(1);
}

console.log("\n✅ All unit tests passed!");
process.exit(0);
