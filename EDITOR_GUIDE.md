# Visual C Code Editor Guide

## Overview
The Visual C Code Editor is a web-based, block-oriented editor that allows you to create C programs visually using drag-and-drop blocks, eliminating the need to write code directly.

## Browser Support

### Minimum Versions
- **Chrome/Edge**: 90+ (recommended)
- **Firefox**: 88+
- **Safari**: 14+
- **Mobile browsers**: iOS Safari 14+, Chrome Mobile 90+

### Requirements
- JavaScript must be enabled
- Modern browser with ES6+ support
- No internet connection required (runs entirely locally)

## Getting Started

### Opening the Editor
Simply open `editor.html` in any modern web browser:
```bash
# Option 1: Double-click editor.html in your file browser
# Option 2: Open from command line
open editor.html      # macOS
xdg-open editor.html  # Linux
start editor.html     # Windows
```

No installation, build process, or dependencies required!

## Using the Editor

### 1. Block Palette (Left Panel)
The Block Palette contains three types of draggable blocks:

- **#include**: Include header files (e.g., stdio.h, stdlib.h, string.h)
- **printf()**: Print text output statements
- **return**: Return a value from main()

#### Adding Blocks
- **Mouse**: Drag blocks from the palette into the workspace
- **Keyboard**: Tab to select a block, press Enter or Space to add it
- **Touch**: Tap on a block to add it to the workspace

### 2. Workspace (Center Panel)
The Workspace is where you build your program:

- **Add blocks**: Drag from palette or use keyboard navigation
- **Edit values**: Click into input fields and type
- **Remove blocks**: Click the "✕ Remove" button on any block
- **Reorder**: Blocks are automatically organized (includes first, then statements)

### 3. Generated Code (Right Panel)
Watch your C code generate in real-time as you:
- Add blocks to the workspace
- Remove blocks from the workspace
- Edit block values

The code display updates automatically as you work.

### 4. Export
Click the **"📥 Export to hello.c"** button to download your generated code.

**Security Note**: The export filename is hardcoded as `hello.c` to prevent security issues like path traversal attacks. This cannot be changed.

## Features

### Input Validation
- **Header names**: Must contain only alphanumeric characters, dots, hyphens, and underscores
- **Printf text**: Automatically escaped for C string safety
- **Return values**: Accepts any numeric value or expression

### Automatic Code Organization
- Includes are automatically placed at the top of the file
- Duplicate includes are removed (case-insensitive)
- Includes maintain stable ordering for deterministic output
- Statements appear inside `main()` in the order added

### String Escaping
The editor automatically handles special characters in printf statements:
- Backslashes (`\`) are properly escaped
- Quotes (`"`) are escaped for C strings
- Newlines (`\n`), tabs (`\t`), and carriage returns (`\r`) are preserved
- Format specifiers (like `%d`, `%s`) work as expected

Example:
```
Input:  Say "Hello"\n
Output: printf("Say \"Hello\"\n");
```

### Keyboard Accessibility
The editor is fully keyboard accessible:
- **Tab**: Navigate between elements
- **Enter/Space**: Activate buttons or add blocks
- **Focus indicators**: Visible yellow outlines show current focus
- **Screen reader**: All elements have proper ARIA labels

### Touch Support
The editor works on touch devices:
- **Tap** a block in the palette to add it to the workspace
- **Tap** input fields to edit values
- **Tap** remove buttons to delete blocks

Note: Traditional drag-and-drop may not work on all touch devices; use tap-to-add instead.

## Examples

### Hello World
1. The editor starts with a default "Ciao, Mondo!" program
2. Edit the printf block to change the message
3. Click Export

### Custom Program with Multiple Includes
1. Drag an `#include` block for `string.h`
2. Drag a `printf` block
3. Enter your message
4. Drag a `return` block with value `0`
5. Export the code

### Format Specifiers and Special Characters
The editor generates safe printf calls using `printf("%s", userString)` format:
1. Add a printf block
2. Enter any text, including special characters like `%`, `\n`, `100%`, etc.
3. The generated code safely prints your text as-is
4. No manual escaping needed - `100%` will output `100%` correctly

**✅ Safe behavior:** The editor always uses `printf("%s", string)` to prevent format string vulnerabilities. User input is treated as data, not format strings, so characters like `%d`, `%s` are printed literally.

**⚠️ For advanced users:** If you need actual format specifiers with variables, you must manually edit the exported code:
- Generated: `printf("%s", "Value: %d\n");` outputs `"Value: %d\n"` literally
- Manual edit: `printf("Value: %d\n", myVar);` outputs `"Value: 42\n"` with variable substitution

## Known Limitations

### What's Supported
✅ Basic C program structure (includes + main function)  
✅ Header includes  
✅ Printf statements with string literals (safe from format string vulnerabilities)
✅ Return statements  
✅ String escaping for C safety  
✅ Duplicate include detection  
✅ Special characters (%, \n, etc.) handled safely

### What's NOT Supported
❌ Variable declarations  
❌ Function definitions (beyond main)  
❌ Control structures (if, while, for, switch)  
❌ Complex expressions  
❌ Pointer operations
❌ Printf with format specifiers and variables (can be added manually after export)  
❌ Struct/union definitions  
❌ Preprocessor directives (beyond #include)  
❌ Multi-file projects  
❌ Import existing C code  

This editor is designed for simple, educational C programs. For complex programs, use a full IDE or text editor.

### Performance
- **Small programs** (< 20 blocks): Instant updates
- **Medium programs** (20-50 blocks): Sub-second updates with debouncing
- **Large programs** (50+ blocks): May experience slight delays

Input is debounced with a 150ms delay to prevent excessive re-renders during typing.

## Security

### XSS Protection
All user input is sanitized using multiple layers of protection:
- **escapeHtml()**: Escapes HTML entities before DOM insertion
- **textContent**: Used for all code display (prevents XSS)
- **Input validation**: Header names are validated against a whitelist pattern
- **No eval()**: No dynamic code execution

### File Download Safety
- **Hardcoded filename**: Export filename is always `hello.c` (cannot be changed)
- **Secure Blob API**: Uses standard browser file download mechanisms
- **No server interaction**: All processing happens locally in your browser

### Code Quality
- **No global pollution**: All code wrapped in IIFE (Immediately Invoked Function Expression)
- **Strict mode**: JavaScript runs in strict mode
- **CSS scoping**: All CSS classes prefixed with `vce-` to avoid conflicts
- **Tested**: Comprehensive security test suite validates all protections

## Troubleshooting

### Editor Won't Load
- Ensure JavaScript is enabled in your browser
- Try a different browser (Chrome, Firefox, or Safari recommended)
- Check browser console for errors (F12 → Console tab)

### Blocks Won't Add
- For keyboard: Ensure the block is focused (yellow outline) before pressing Enter
- For touch: Try tapping directly on the block text
- For mouse: Ensure you're dragging to the workspace area

### Export Not Working
- Check that your browser allows file downloads
- Disable popup blockers if necessary
- Try a different browser

### Code Looks Wrong
- Check for duplicate includes (automatically deduplicated)
- Verify input validation messages on header names
- Ensure proper escaping of special characters in printf statements

## Testing

The editor includes comprehensive test coverage:

```bash
# Run all tests
make test

# Individual test suites
node test/test_escape_html.js        # HTML escaping tests
node test/test_xss_integration.js    # XSS security tests
node test/test_innerHTML_scan.js     # innerHTML security scan
node test/test_c_generation.js       # C code generation tests
bash test/test_editor_output.sh      # Editor output smoke tests
```

## Advanced Usage

### Modifying the Editor
The editor source code is contained in a single HTML file (`editor.html`) for simplicity. Advanced users can:
- Customize CSS styles (prefixed with `.vce-`)
- Modify default block values
- Add new block types
- Change code generation templates

All JavaScript code is wrapped in an IIFE to prevent global scope pollution.

### Integration
The editor can be embedded in other web pages:
```html
<iframe src="editor.html" width="100%" height="800px"></iframe>
```

## Support

For issues, questions, or contributions:
1. Check this guide for known limitations
2. Review the test suite for expected behavior
3. Open an issue on the project repository

## Version History

See git commit history for detailed changes.

## License

See repository LICENSE file for licensing information.
