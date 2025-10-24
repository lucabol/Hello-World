/**
 * Core code generation module for the Visual C Code Editor
 * This module contains pure logic that can be tested in Node.js without a browser
 * 
 * Usage in Node.js tests:
 *   const generator = require('./generator.js');
 *   const code = generator.generateCode(blocks);
 * 
 * Usage in browser (editor.html):
 *   <script src="generator.js"></script>
 *   const code = CCodeGenerator.generateCode(blocks);
 */

(function(global) {
    'use strict';

    /**
     * Security: HTML Escaping Function
     * 
     * This function prevents XSS (Cross-Site Scripting) attacks by escaping user input
     * before inserting it into the DOM or generated code. It converts potentially dangerous
     * characters (<, >, &, ", ') into their HTML entity equivalents.
     * 
     * Approach:
     * - In browser: Uses textContent to safely set text on a temporary div element
     * - In Node.js: Uses string replacement for the same escaping
     * - The browser automatically escapes special characters
     * - Returns the escaped string
     * 
     * Validation: See test/test_escape_html.js for comprehensive test coverage
     * Integration Tests: See test/test_xss_integration.js
     */
    function escapeHtml(text) {
        if (typeof document !== 'undefined') {
            // Browser environment
            const div = document.createElement('div');
            div.textContent = text;
            return div.innerHTML;
        } else {
            // Node.js environment
            return text
                .replace(/&/g, '&amp;')
                .replace(/</g, '&lt;')
                .replace(/>/g, '&gt;')
                .replace(/"/g, '&quot;')
                .replace(/'/g, '&#039;');
        }
    }

    /**
     * Security: Escape printf format specifiers
     * 
     * Escapes percent signs (%) in user-provided strings to prevent printf format
     * string injection vulnerabilities. When users enter text like "50% complete",
     * the % should be treated as literal text, not as a format specifier.
     * 
     * This ensures generated code uses safe, literal string output.
     */
    function escapePrintfFormat(text) {
        return text.replace(/%/g, '%%');
    }

    /**
     * Get placeholder text for block type
     */
    function getPlaceholder(type) {
        const placeholders = {
            'include': 'stdio.h',
            'printf': 'Hello world!',
            'return': '0'
        };
        return placeholders[type] || '';
    }

    /**
     * Generate C code from blocks
     * 
     * Automatically adds required includes if they're missing.
     * Ensures printf strings are safe by escaping format specifiers.
     * Uses escapeHtml to prevent code injection via user inputs.
     * 
     * @param {Array} blocks - Array of block objects with {id, type, value}
     * @returns {string} - Generated C code
     */
    function generateCode(blocks) {
        let code = '';
        const printfs = blocks.filter(b => b.type === 'printf');
        const includes = blocks.filter(b => b.type === 'include');
        
        // Auto-add stdio.h if printf is used but stdio.h is not included
        const hasStdio = includes.some(b => b.value.trim() === 'stdio.h');
        if (printfs.length > 0 && !hasStdio) {
            code += '#include <stdio.h>\n';
        }

        // Add user-specified includes
        includes.forEach(block => {
            const header = block.value.trim();
            code += `#include <${escapeHtml(header)}>\n`;
        });

        if (printfs.length > 0 || includes.length > 0) {
            code += '\n';
        }

        // Start main function
        code += 'int main(){\n';

        // Add printf statements with format specifier escaping
        printfs.forEach(block => {
            const text = block.value.trim();
            // Escape HTML entities first, then escape printf format specifiers
            const escapedText = escapePrintfFormat(escapeHtml(text));
            code += `    printf("${escapedText}");\n`;
        });

        // Add return statement or default to return 0
        const returns = blocks.filter(b => b.type === 'return');
        if (returns.length > 0) {
            const returnValue = returns[returns.length - 1].value.trim();
            code += `    return ${escapeHtml(returnValue)};\n`;
        }

        code += '}\n';

        return code;
    }

    // Export for Node.js and browser
    /**
     * Module Exports - Universal Module Definition (UMD) Pattern
     * 
     * This module is designed to work in both Node.js and browser environments:
     * 
     * Node.js Usage (CommonJS):
     *   const generator = require('./generator.js');
     *   const code = generator.generateCode(blocks);
     *   const escaped = generator.escapeHtml(userInput);
     * 
     * Browser Usage (global object):
     *   <script src="generator.js"></script>
     *   const code = CCodeGenerator.generateCode(blocks);
     *   const escaped = CCodeGenerator.escapeHtml(userInput);
     * 
     * Exported API:
     *   - escapeHtml(text): Escapes HTML entities for XSS protection
     *   - escapePrintfFormat(text): Escapes % signs for printf safety
     *   - getPlaceholder(type): Returns default placeholder for block type
     *   - generateCode(blocks): Generates C code from block array
     */
    const CCodeGenerator = {
        escapeHtml: escapeHtml,
        escapePrintfFormat: escapePrintfFormat,
        getPlaceholder: getPlaceholder,
        generateCode: generateCode
    };

    // Node.js export (CommonJS)
    if (typeof module !== 'undefined' && module.exports) {
        module.exports = CCodeGenerator;
    }

    // Browser global export
    if (typeof global !== 'undefined') {
        global.CCodeGenerator = CCodeGenerator;
    }

})(typeof window !== 'undefined' ? window : global);
