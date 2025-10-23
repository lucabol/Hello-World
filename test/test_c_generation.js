#!/usr/bin/env node
/**
 * Test C code generation safety
 * Verifies that generated C code handles special characters correctly
 * and compiles without errors
 * 
 * Dependencies: None - uses only built-in Node.js APIs
 */

const { execSync } = require('child_process');
const fs = require('fs');
const path = require('path');
const os = require('os');

console.log("🧪 Running C code generation safety tests...\n");

let passed = 0;
let failed = 0;

// Test cases with special characters
const testCases = [
    {
        name: "Simple printf with newline",
        printf: "Hello world!\\n",
        expected: "Hello world!\n"
    },
    {
        name: "Printf with quotes",
        printf: 'Say \\"Hello\\"\\n',
        expected: 'Say "Hello"\n'
    },
    {
        name: "Printf with backslashes",
        printf: "Path: C:\\\\Users\\\\Test\\n",
        expected: "Path: C:\\Users\\Test\n"
    },
    {
        name: "Printf with tabs",
        printf: "Col1\\tCol2\\tCol3\\n",
        expected: "Col1\tCol2\tCol3\n"
    },
    {
        name: "Printf with format specifiers (escaped)",
        printf: "100%% complete\\n",
        expected: "100% complete\n"
    },
    {
        name: "Empty string (no output)",
        printf: "",
        expected: "",
        skipCompile: true  // Empty printf triggers -Werror=format-zero-length
    }
];

// Create temp directory for tests
const tmpDir = fs.mkdtempSync(path.join(os.tmpdir(), 'c-gen-test-'));

testCases.forEach((testCase, index) => {
    console.log(`Test ${index + 1}: ${testCase.name}`);
    
    if (testCase.skipCompile) {
        console.log(`  ⊘ Skipped (empty printf not supported with -Werror)`);
        console.log();
        return;
    }
    
    try {
        // Generate C code
        const code = `#include <stdio.h>

int main(){
    printf("${testCase.printf}");
    return 0;
}`;
        
        // Write to temp file
        const cFile = path.join(tmpDir, `test_${index}.c`);
        const exeFile = path.join(tmpDir, `test_${index}`);
        fs.writeFileSync(cFile, code);
        
        // Try to compile
        try {
            execSync(`gcc -Wall -Wextra -Werror -o ${exeFile} ${cFile}`, {
                stdio: 'pipe',
                timeout: 5000
            });
            console.log(`  ✓ Compilation successful`);
        } catch (compileError) {
            console.log(`  ✗ Compilation failed`);
            console.log(`    ${compileError.message}`);
            failed++;
            return;
        }
        
        // Run and check output
        try {
            const output = execSync(exeFile, {
                encoding: 'utf8',
                timeout: 5000
            });
            
            if (output === testCase.expected) {
                console.log(`  ✓ Output matches expected`);
                passed++;
            } else {
                console.log(`  ✗ Output mismatch`);
                console.log(`    Expected: ${JSON.stringify(testCase.expected)}`);
                console.log(`    Got:      ${JSON.stringify(output)}`);
                failed++;
            }
        } catch (runError) {
            console.log(`  ✗ Execution failed`);
            console.log(`    ${runError.message}`);
            failed++;
        }
    } catch (error) {
        console.log(`  ✗ Test failed: ${error.message}`);
        failed++;
    }
    
    console.log();
});

// Cleanup
try {
    fs.rmSync(tmpDir, { recursive: true, force: true });
} catch (e) {
    console.log(`Warning: Could not clean up temp directory: ${e.message}`);
}

// Summary
console.log(`${'='.repeat(50)}`);
console.log(`Results: ${passed} passed, ${failed} failed`);
console.log(`${'='.repeat(50)}`);

if (failed > 0) {
    console.log("\n✗ C code generation tests failed");
    process.exit(1);
}

console.log("\n✅ All C code generation tests passed!");
