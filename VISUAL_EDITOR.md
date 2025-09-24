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
- **Escape Handling**: Automatically escapes quotes, backslashes, newlines, and format specifiers in printf strings
- **Identifier Validation**: Ensures C variable names follow proper naming conventions
- **Expression Sanitization**: Basic validation of C expressions to prevent code injection
- **Safe Downloads**: Uses secure Blob API for file downloads

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

### Input Sanitization
The editor implements comprehensive input sanitization to prevent code injection and ensure valid C output:

```javascript
// Example of escaping dangerous input
Input: Hello "world" %s
Output: Hello \"world\" %%s

// Example of identifier validation  
Input: invalid-name
Output: defaultVar (fallback)

// Example of expression sanitization
Input: dangerous$()
Output: dangerous (cleaned)
```

### Safe File Generation
- Uses browser Blob API for secure file creation
- No server-side processing required
- Generated files contain only validated C code

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