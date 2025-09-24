# Visual C Code Editor

## Overview

The Visual C Code Editor is a web-based drag-and-drop interface that allows users to create C programs using visual blocks instead of typing code manually. This tool is perfect for:

- Learning C programming concepts
- Rapid prototyping
- Educational purposes
- Visual programming enthusiasts

## Features

### Visual Block System
- **Basic Statements**: Print statements and variable declarations
- **Control Flow**: If statements and for loops
- **Functions**: Main function and include directives
- **Drag and Drop**: Intuitive interface for building programs

### Real-time Code Generation
- Automatic C code generation as you build your program
- Live preview of the generated code
- Syntax highlighting in the output panel

### Export Functionality
- Export your visual program to a `hello.c` file
- Compatible with the existing build system
- Generates standard C code that compiles with GCC

## How to Use

1. **Open the Editor**: Open `visual-editor.html` in your web browser
2. **Drag Blocks**: Drag programming blocks from the left palette to the center canvas
3. **Customize**: Edit the input fields in blocks to customize your program
4. **Generate Code**: The C code is generated automatically and displayed on the right
5. **Export**: Click "Export to hello.c" to download your program
6. **Compile**: Use `gcc -o hello hello.c` to compile your exported program

## Block Types

### Basic Statements
- **Print Block**: Creates `printf()` statements with customizable text
- **Variable Block**: Declares and initializes variables of different types

### Control Flow
- **If Statement**: Creates conditional logic blocks
- **For Loop**: Creates iteration loops with customizable parameters

### Functions
- **Main Function**: Defines the main program entry point
- **Include**: Adds header file includes

## Technical Details

### Generated Code Structure
The visual editor generates standard C code with the following structure:
```c
#include <stdio.h>

int main() {
    // Your blocks generate code here
    printf("Hello world!\n");
    return 0;
}
```

### Compatibility
- Works with any modern web browser
- Generated code is compatible with GCC and Clang
- Output follows C99 standards
- Passes all existing validation tests

### File Structure
- `visual-editor.html`: Complete standalone visual editor
- Self-contained: No external dependencies
- Responsive design: Works on desktop and mobile

## Integration with Existing Workflow

The visual editor integrates seamlessly with the existing Hello World repository:

1. Create your program using the visual editor
2. Export to `hello.c`
3. Use existing build commands: `gcc -o hello hello.c`
4. Run validation: `./test/validate.sh`
5. All existing tooling continues to work

## Examples

### Simple Hello World
1. Drag a "Print" block to the canvas
2. The default text "Hello world!" is already set
3. Export to create a working hello.c program

### More Complex Programs
1. Start with an "Include" block for stdio.h
2. Add a "Main Function" block
3. Add "Variable" blocks for data
4. Use "If Statement" or "For Loop" blocks for logic
5. Add "Print" blocks to show output

## Browser Compatibility

- Chrome/Chromium: Full support
- Firefox: Full support  
- Safari: Full support
- Edge: Full support
- Mobile browsers: Responsive design with touch support