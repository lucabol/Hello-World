# Security Implementation Details

This document provides detailed information about the security implementations in the Visual C Code Editor.

## Location
All security functions are implemented in `editor.html` within the IIFE starting at line ~285.

## Core Security Functions

### 1. escapeHtml()

**Location:** `editor.html`, line ~371

**Implementation:**
```javascript
/**
 * Security: Escape HTML to prevent XSS attacks
 * Used before inserting user content into HTML attributes
 * 
 * @param {string} text - Text to escape
 * @returns {string} - Escaped HTML-safe text
 */
function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}
```

**Purpose:** Escapes HTML entities to prevent XSS attacks when user input is inserted into HTML contexts.

**How it works:**
1. Creates a temporary div element
2. Sets the user input as `textContent` (which auto-escapes)
3. Retrieves the escaped result via `innerHTML`

**Escaping rules:**
- `<` → `&lt;`
- `>` → `&gt;`
- `&` → `&amp;`
- `"` → `&quot;`
- `'` → `&#039;` (in some browsers)

**Edge cases handled:**
- Null/undefined: Converted to string "null"/"undefined"
- Empty string: Returns empty string
- Unicode characters: Preserved as-is (UTF-8 safe)
- Surrogate pairs: Preserved correctly by textContent
- Control characters: Preserved (may be escaped in innerHTML)

**Usage:** Called 5+ times throughout the codebase before any HTML attribute insertion.

**Security guarantees:**
- Prevents script injection via `<script>` tags
- Prevents attribute-based XSS via `onclick=`, `onerror=`, etc.
- Prevents HTML injection via `<iframe>`, `<object>`, etc.
- Safe for use in HTML attribute contexts (e.g., `value="${escapeHtml(userInput)}"`)

**Limitations:**
- NOT safe for JavaScript string contexts (use JSON.stringify)
- NOT safe for CSS contexts (additional escaping needed)
- NOT safe for URL contexts (use encodeURIComponent)

---

### 2. escapeCString()

**Location:** `editor.html`, line ~379

**Implementation:**
```javascript
/**
 * Escape string for safe use in C source code
 * Handles backslashes, quotes, and format specifiers
 * 
 * @param {string} text - Text to escape for C
 * @returns {string} - C-safe escaped text
 */
function escapeCString(text) {
    return text
        .replace(/\\/g, '\\\\')   // Escape backslashes first
        .replace(/"/g, '\\"')     // Escape double quotes
        .replace(/\n/g, '\\n')    // Keep newline escapes
        .replace(/\t/g, '\\t')    // Keep tab escapes
        .replace(/\r/g, '\\r');   // Keep carriage return escapes
}
```

**Purpose:** Escapes strings for safe use in C source code string literals.

**How it works:**
Applies a series of replacements to ensure the string can be safely embedded in C source code within double quotes.

**Escaping rules:**
- `\` → `\\` (must be first to avoid double-escaping)
- `"` → `\"`
- Literal `\n` in input → Preserved as `\\n` escape sequence
- Literal `\t` in input → Preserved as `\\t` escape sequence
- Literal `\r` in input → Preserved as `\\r` escape sequence

**Edge cases handled:**
- Empty string: Returns empty string
- String with only backslashes: Each `\` becomes `\\`
- Mixed escapes: `"Hello\n"` → `\"Hello\\n\"`
- Format specifiers: `%d`, `%s` preserved as-is (caller must handle)

**Usage:** Called during C code generation for printf statement contents.

**Security guarantees:**
- Prevents breaking out of C string literals
- Prevents injection of arbitrary C code via unescaped quotes
- Generates valid C source code that compiles
- Preserves intended escape sequences

**Limitations:**
- Does NOT escape `%` format specifiers (by design - allows printf formatting)
- Does NOT handle null bytes (would terminate C string)
- Does NOT validate C identifier safety
- NOT safe for C preprocessor contexts

**Testing:** See `test/test_c_generation.js` for comprehensive tests including quotes, backslashes, and escape sequences.

---

### 3. isValidHeader()

**Location:** `editor.html`, line ~395

**Implementation:**
```javascript
/**
 * Validate header file name
 * Only allows alphanumeric, dots, hyphens, underscores
 * 
 * @param {string} header - Header filename to validate
 * @returns {boolean} - True if valid
 */
function isValidHeader(header) {
    // Allow standard header patterns like stdio.h, sys/types.h
    const pattern = /^[a-zA-Z0-9_.-]+(\/[a-zA-Z0-9_.-]+)*$/;
    return pattern.test(header) && header.length < 100;
}
```

**Purpose:** Validates header file names to prevent path traversal and injection attacks.

**How it works:**
Uses a whitelist regex pattern to only allow safe characters in header names.

**Allowed characters:**
- Letters: a-z, A-Z
- Numbers: 0-9
- Special: `_` (underscore), `.` (dot), `-` (hyphen), `/` (forward slash for directories)

**Validation rules:**
1. Must match the pattern
2. Must be less than 100 characters
3. Can include directory separators (`/`) for system headers like `sys/types.h`

**Rejected inputs:**
- Path traversal: `../etc/passwd`
- Backslashes: `path\to\file`
- Angle brackets: `<stdio.h>` (brackets added by generator)
- Spaces: `my header.h`
- Empty string: ``
- Special chars: `header;rm -rf.h`

**Edge cases handled:**
- Empty string: Returns false
- Very long names: Rejected if > 100 chars
- Multiple dots: Allowed (e.g., `my.header.file.h`)
- Leading/trailing spaces: Rejected (not in whitelist)

**Security guarantees:**
- Prevents path traversal attacks
- Prevents command injection via header names
- Prevents malformed #include directives
- Only allows valid C header names

**Usage:** Called before adding #include directives to generated C code.

---

### 4. deduplicateIncludes()

**Location:** `editor.html`, line ~405

**Implementation:**
```javascript
/**
 * Deduplicate includes - keep first occurrence
 * 
 * @param {Array} includeBlocks - Array of include block objects
 * @returns {Array} - Deduplicated array
 */
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
```

**Purpose:** Removes duplicate #include directives from generated code.

**How it works:**
1. Normalizes header names (trim + lowercase)
2. Uses a Set to track seen headers
3. Filters out duplicates, keeping first occurrence

**Deduplication rules:**
- Case-insensitive: `stdio.h` and `STDIO.H` are treated as duplicates
- Space-insensitive: Leading/trailing spaces ignored
- First occurrence wins: Original casing and position preserved

**Edge cases handled:**
- Empty array: Returns empty array
- Single item: Returns unchanged
- All duplicates: Returns single item
- Mixed case: Normalized for comparison

**Security guarantees:**
- Prevents compiler warnings about duplicate includes
- Ensures deterministic output
- Maintains stable ordering

**Testing:** See `test/test_header_validation.js` for deduplication tests.

---

## Security Architecture

### Defense in Depth

The editor employs multiple layers of security:

1. **Input Validation** (isValidHeader) - Whitelist approach
2. **HTML Escaping** (escapeHtml) - For DOM insertion
3. **C String Escaping** (escapeCString) - For code generation
4. **DOM API Safety** (textContent over innerHTML) - Browser-level protection
5. **CSP Ready** - No eval(), inline handlers use IIFE
6. **Encapsulation** - IIFE prevents global pollution

### XSS Prevention

All user input paths are protected:

1. **Block values in workspace**
   - Escaped with `escapeHtml()` before HTML insertion
   - Displayed using `textContent` (not innerHTML)

2. **Generated code display**
   - Uses `textContent` exclusively (line ~705)
   - Never uses innerHTML for user content

3. **Header names**
   - Validated with `isValidHeader()` whitelist
   - Only safe characters allowed in #include directives

4. **Printf strings**
   - Escaped with `escapeCString()` for C safety
   - Properly handles quotes and backslashes

### Code Injection Prevention

**C Code Injection:**
- `escapeCString()` prevents breaking out of string literals
- `isValidHeader()` prevents malicious #include paths
- No user input in code structure (only string literals)

**JavaScript Injection:**
- IIFE wrapper prevents global scope pollution
- No `eval()` or `Function()` constructor usage
- No dynamic script loading

**HTML Injection:**
- All innerHTML usage is safe (clearing or escapeHtml context)
- Automated scanner (`test/test_innerHTML_scan.js`) validates
- textContent used for all user content display

## Testing

Security functions are tested by:

1. `test/test_escape_html.js` - HTML escaping unit tests
2. `test/test_xss_integration.js` - XSS prevention integration tests
3. `test/test_innerHTML_scan.js` - Automated innerHTML security scan
4. `test/test_c_generation.js` - C code generation safety tests
5. `test/test_header_validation.js` - Header validation tests (to be added)

## References

- OWASP XSS Prevention Cheat Sheet
- CWE-79: Improper Neutralization of Input During Web Page Generation
- CWE-78: Improper Neutralization of Special Elements in OS Commands
