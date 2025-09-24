# Visual C Code Editor

## Overview

The Visual C Code Editor is a web-based drag-and-drop interface that allows users to create C programs using visual blocks instead of typing code manually. This tool is perfect for:

- Learning C programming concepts
- Rapid prototyping
- Educational purposes
- Visual programming enthusiasts

**Privacy Notice**: This editor is completely self-contained and does not call any external services, analytics, or tracking. All processing happens locally in your browser.

## Features

### Visual Block System
- **Basic Statements**: Print statements and variable declarations
- **Control Flow**: If statements and for loops
- **Functions**: Main function and include directives
- **Drag and Drop**: Intuitive interface for building programs

### Security & Input Sanitization

This visual editor implements enterprise-grade security through a multi-layered approach:

#### Content Security Policy (CSP)
- **Strict CSP**: The editor includes a restrictive Content-Security-Policy that blocks all external resources
- **No External Connections**: Zero network requests - completely self-contained and offline-capable
- **No Analytics**: No tracking, telemetry, or data collection of any kind

#### Conservative Input Validation
- **escapeForC()**: Sanitizes all user input before code generation
  - Escapes quotes, backslashes, newlines, tabs, and printf format specifiers
  - Safe handling of null/undefined inputs
  - Type coercion for non-string inputs
- **validateCIdentifier()**: Ensures C variable names are safe
  - Validates against C naming conventions (alphanumeric + underscore, starts with letter/underscore)
  - 63-character length limit (C99 internal identifier limit)
  - Safe fallback to 'defaultVar' for invalid inputs
- **validateCExpression()**: Conservative whitelist approach for expressions
  - **Educational Focus**: Designed for learning, not production use
  - **Explicit Rejections**: Blocks dangerous constructs like function calls, casts, pointers
  - **Whitelist Only**: Only allows basic operators (+, -, *, /, <, >, ==, etc.)
  - **Length Limits**: Maximum 200 characters to prevent abuse
  - **Token Blocking**: Rejects semicolons, preprocessor directives, comments, string literals

#### Expression Validation Grammar
**Supported (Safe for Educational Use):**
- Variable names: `variable`, `count`, `total`
- Numeric literals: `42`, `3.14` 
- Basic operators: `x + 1`, `count > 10`, `a == b`
- Grouping: `(x + y) * 2`
- Array indexing: `arr[i]`

**Rejected (Too Advanced/Dangerous for Educational Context):**
- Function calls: `strlen(str)`, `malloc(size)` - could enable arbitrary execution
- Pointer operations: `*ptr`, `&variable` - advanced concepts
- Cast operations: `(int)value` - complex and potentially unsafe
- String/char literals: `"hello"`, `'c'` - complicates escaping
- Comments: `/* comment */` - could hide malicious content
- Statements: `x = 1; y = 2` - could allow code injection

#### Safe File Downloads
- **Browser Blob API**: Uses secure native browser APIs for file generation
- **UTF-8 Encoding**: Proper character encoding for international compatibility
- **No Server Involvement**: Files are generated entirely client-side

### Real-time Code Generation
- Automatic C code generation as you build your program
- Live preview of the generated code
- Syntax highlighting in the output panel
- Input validation and error handling

### Export Functionality
- Export your visual program to a `hello.c` file
- Compatible with the existing build system
- Generates standard C code that compiles with GCC
- Secure file download using browser APIs

### Accessibility Features
- **Keyboard Navigation**: Full keyboard support with Tab navigation
- **Screen Reader Support**: ARIA labels and live regions for assistive technologies
- **Keyboard Shortcuts**: 
  - `Tab`/`Shift+Tab`: Navigate between elements
  - `Enter`/`Space`: Select blocks from palette
  - `Delete`/`Backspace`: Remove placed blocks
  - `Ctrl+↑`/`Ctrl+↓`: Move blocks up/down
  - `Ctrl+S`: Export file
  - `Ctrl+R`: Generate code
- **Focus Indicators**: Clear visual focus indicators for keyboard users
- **High Contrast**: Accessible color scheme with sufficient contrast ratios

## How to Use

1. **Open the Editor**: Open `visual-editor.html` in your web browser
2. **Drag Blocks**: Drag programming blocks from the left palette to the center canvas
3. **Customize**: Edit the input fields in blocks to customize your program
4. **Generate Code**: The C code is generated automatically and displayed on the right
5. **Export**: Click "Export to hello.c" to download your program
6. **Compile**: Use `gcc -o hello hello.c` to compile your exported program

### Keyboard-Only Usage
1. Use `Tab` to navigate to the block palette
2. Use `Enter` or `Space` to select a block and add it to the canvas
3. Use `Tab` to navigate to input fields and modify block parameters
4. Use `Delete` on placed blocks to remove them
5. Use `Ctrl+↑`/`Ctrl+↓` to reorder blocks
6. Use `Ctrl+S` to export your program

## Block Types

### Basic Statements
- **Print Block**: Creates `printf()` statements with customizable text
  - Automatically escapes quotes and format specifiers
  - Adds newline character for proper output
- **Variable Block**: Declares and initializes variables of different types
  - Validates variable names for C compliance
  - Supports int, char, float, double types

### Control Flow
- **If Statement**: Creates conditional logic blocks
  - Validates expressions for basic syntax
  - Generates proper C if statement structure
- **For Loop**: Creates iteration loops with customizable parameters
  - Validates loop variables and expressions
  - Generates standard C for loop syntax

### Functions
- **Main Function**: Defines the main program entry point
- **Include**: Adds header file includes with validation

## Security Features

### Input Sanitization Functions

The editor implements three comprehensive security functions to prevent code injection:

#### 1. `escapeForC(input)` - String Literal Escaping
- **Purpose**: Safely escapes user input for inclusion in C string literals
- **Handles**: Quotes, backslashes, newlines, tabs, printf format specifiers
- **Null safety**: Converts null/undefined to empty string, coerces other types to strings
- **Example**:
  ```javascript
  escapeForC('Hello "world" %s') → 'Hello \"world\" %%s'
  escapeForC(null) → ''
  escapeForC(123) → '123'
  ```

#### 2. `validateCIdentifier(input)` - C Identifier Validation
- **Purpose**: Ensures variable/function names follow C identifier rules
- **Validation**: Must start with letter/underscore, contain only alphanumeric/underscore
- **Length limit**: Maximum 63 characters (C99 standard)
- **Fallback**: Returns 'defaultVar' for invalid input
- **Example**:
  ```javascript
  validateCIdentifier('valid_name') → 'valid_name'
  validateCIdentifier('invalid-name') → 'defaultVar'
  validateCIdentifier(null) → 'defaultVar'
  ```

#### 3. `validateCExpression(input)` - Expression Sanitization
- **Purpose**: Validates C expressions using conservative whitelist approach
- **Security**: Rejects dangerous tokens (semicolons, preprocessor directives, strings, comments)
- **Whitelist**: Only allows alphanumeric chars, operators, parentheses, brackets
- **Length limit**: Maximum 200 characters
- **Fallback**: Returns '1' (safe numeric literal) for invalid input
- **Example**:
  ```javascript
  validateCExpression('x > 0') → 'x > 0'
  validateCExpression('x; system("rm")') → '1'  // Rejected - contains semicolon
  validateCExpression('#include <evil>') → '1'  // Rejected - preprocessor
  ```

### Security Limitations & Scope

**Educational Use Only**: This editor is designed for learning C programming concepts, not production code generation.

**Expression Validation Constraints**:
- Does not support string literals in expressions (for safety)
- Does not support character literals 
- Does not support function calls in expressions
- Does not support pointer dereferencing or casting
- Conservative approach may reject some valid C expressions

**Not a C Compiler**: The editor performs basic syntax validation but does not:
- Check for undefined variables
- Validate function signatures
- Perform type checking
- Detect runtime errors

### Safe File Generation
- Uses browser Blob API for secure file creation
- No server-side processing required  
- Generated files contain only validated C code
- UTF-8 encoding prevents encoding-based attacks

## Browser Compatibility

### Supported Browsers
- **Chrome/Chromium**: Version 60+ (Full support)
- **Firefox**: Version 60+ (Full support)
- **Safari**: Version 12+ (Full support)
- **Edge**: Version 79+ (Full support)
- **Mobile browsers**: iOS Safari 12+, Chrome Mobile 60+

### Required Features
- **Drag and Drop API**: For visual block manipulation
- **ES6+ JavaScript**: Modern JavaScript features (const, let, arrow functions)
- **CSS Grid/Flexbox**: For responsive layout
- **Blob API**: For file downloads

### Touch Device Support
- Touch-friendly drag and drop on mobile devices
- Responsive design adapts to mobile screens
- Alternative keyboard navigation for accessibility

## Educational Scope and Limitations

### Intended Use
**This visual editor is designed specifically for educational purposes and C programming learning.** It is NOT intended for production code development or as a replacement for professional development tools.

### Educational Focus
- **Learning Tool**: Helps students understand C program structure visually
- **Concept Introduction**: Introduces programming concepts through drag-and-drop interface  
- **Safe Environment**: Conservative validation prevents students from encountering complex errors
- **Simplified Syntax**: Focuses on fundamental C constructs without advanced features

### Explicit Limitations
**The editor intentionally restricts advanced C features to maintain educational focus:**

#### Not Supported (By Design)
- **Complex Expressions**: Function calls, pointer operations, type casting
- **Advanced Data Types**: Structures, unions, enums, complex pointer types
- **Memory Management**: malloc/free, dynamic allocation
- **String Manipulation**: String literals in expressions, string functions
- **Preprocessor**: Advanced macros, conditional compilation  
- **Error Handling**: Complex error checking, exception handling patterns

#### Why These Limitations Exist
1. **Cognitive Load**: Reduces complexity for beginners learning core concepts
2. **Security**: Prevents potentially dangerous operations in educational context
3. **Focus**: Concentrates on fundamental programming logic rather than language intricacies
4. **Safety**: Ensures generated code compiles successfully with minimal errors

### Production Development Recommendation
For professional C development, use:
- **IDEs**: Visual Studio, CLion, Code::Blocks, Dev-C++
- **Text Editors**: VS Code, Vim, Emacs with C extensions
- **Command Line Tools**: GCC/Clang directly with makefiles
- **Professional Linters**: Static analysis tools, memory checkers

This visual editor serves as a stepping stone toward these professional tools.

## Technical Details

### Generated Code Structure
The visual editor generates standard C code with the following structure:
```c
#include <stdio.h>

int main() {
    // Variable declarations (sanitized)
    int validVar = 42;
    
    // Program logic (escaped and validated)
    printf("Hello \"quoted\" world!\n");
    
    return 0;
}
```

### Compatibility & Standards
- Generated code is compatible with GCC and Clang
- Output follows C99 standards
- Passes strict compiler warnings (-Wall -Wextra -Werror)
- Compatible with existing validation tests

### Performance Considerations
- Lightweight: Single HTML file (~34KB)
- No external dependencies or network requests
- Efficient DOM manipulation for large programs
- Responsive design for various screen sizes

## Testing & Validation

### Automated Testing
Run the test suite to validate the editor:
```bash
./test/test-visual-editor.sh
```

The test suite includes:
- **Security Function Tests**: Validates input sanitization
- **Code Generation Tests**: Ensures generated C code compiles
- **Accessibility Tests**: Checks for proper ARIA attributes
- **Browser Compatibility Tests**: Validates modern JavaScript usage

### Example Configurations
See the `examples/` directory for sample generated C files:
- `hello_basic.c`: Simple hello world program
- `hello_variables.c`: Program with variables and escaped strings

### CI Integration
Add to your CI pipeline:
```bash
# Test visual editor
./test/test-visual-editor.sh

# Test generated code compilation
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -o hello examples/hello_basic.c
./hello
```

## Integration with Existing Workflow

The visual editor integrates seamlessly with the existing Hello World repository:

1. Create your program using the visual editor
2. Export to `hello.c`
3. Use existing build commands: `gcc -o hello hello.c`
4. Run validation: `./test/validate.sh`
5. All existing tooling continues to work

## Development & Extension

### Adding New Block Types
1. Add HTML structure in the palette section
2. Implement block handling in `addBlockToCanvas()`
3. Add code generation logic in `generateCode()`
4. Include input sanitization for new block parameters
5. Add test cases for the new block type

### Extending for Non-Browser Environments
The code generation functions are exposed via `window.VisualEditor`:
```javascript
// Access in Node.js or testing environment
const editor = window.VisualEditor;
const safeString = editor.escapeForC(userInput);
const validId = editor.validateCIdentifier(userInput);
```

### Linting & Code Quality
The editor follows these standards:
- **JavaScript**: ES6+ with strict mode
- **HTML**: Valid HTML5 with semantic markup
- **CSS**: Modern CSS3 with browser prefixes where needed
- **Accessibility**: WCAG 2.1 AA compliance

## Troubleshooting

### Common Issues
- **Blocks not dragging**: Ensure JavaScript is enabled and browser supports Drag and Drop API
- **Export not working**: Check if browser blocks downloads; allow pop-ups for the page
- **Generated code won't compile**: Verify input sanitization is working; check for special characters
- **Keyboard navigation broken**: Ensure focus is on the editor area; try Tab to navigate

### Browser-Specific Notes
- **Safari**: May require explicit permission for downloads
- **Firefox**: Drag and drop may require enabling in privacy settings
- **Mobile**: Use touch gestures; long-press to initiate drag operations

## Examples

### Simple Hello World
1. Drag a "Print" block to the canvas
2. The default text "Hello world!" is already set
3. Export to create a working hello.c program

### Program with Variables
1. Start with an "Include" block for stdio.h
2. Add a "Main Function" block  
3. Add a "Variable" block, set type to "int", name to "count", value to "5"
4. Add a "Print" block with text "Count: %d", but be aware this requires manual printf formatting
5. Export and compile

### More Complex Programs
1. Use "If Statement" blocks for conditional logic
2. Use "For Loop" blocks for iteration
3. Combine multiple blocks to create structured programs
4. All inputs are automatically sanitized for security

The visual editor provides a safe, accessible, and educational way to create C programs while maintaining compatibility with professional development workflows.