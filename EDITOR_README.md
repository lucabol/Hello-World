# Visual C Code Editor

A web-based visual block editor for creating and modifying C code using drag-and-drop blocks.

## Features

- **Visual Block Programming**: Create C code using intuitive drag-and-drop blocks
- **Real-time Code Generation**: See the generated C code update as you build
- **Export Functionality**: Download your code as `hello.c`
- **Pre-built Example**: Load the classic "Hello World" program with one click

## How to Use

### Opening the Editor

1. Open `editor.html` in any modern web browser (Chrome, Firefox, Safari, Edge)
   ```bash
   # On Linux/Mac
   open editor.html
   
   # Or simply double-click the file in your file explorer
   ```

2. The editor will open with an example "Hello World" program already loaded

### Building Your Program

1. **Using the Toolbox**: On the left side, you'll find categories of blocks:
   - **Program Structure**: Main function, include statements
   - **Output**: Print statements
   - **Variables**: Variable declarations and assignments
   - **Control Flow**: If statements, loops
   - **Functions**: Custom function declarations
   - **Text & Math**: Text strings and numbers

2. **Creating Code**:
   - Drag blocks from the toolbox to the workspace
   - Connect blocks together by dragging them near each other
   - Fill in text fields within blocks (like variable names, conditions, etc.)
   - The generated C code appears on the right panel in real-time

3. **Example Program Structure**:
   ```
   [#include <stdio.h>]
        ↓
   [main function]
        ↓
   [print "Hello world!"]
   ```

### Available Actions

- **Generate Code**: Click to refresh the generated C code (automatic by default)
- **Export to hello.c**: Downloads the generated code as `hello.c`
- **Load Example**: Loads the "Hello World" example program

### Tips

- You can zoom in/out using the controls in the bottom-right corner
- Use the trash icon to delete blocks
- Right-click on blocks for additional options (duplicate, delete, etc.)
- Blocks automatically snap together when they're compatible

## Block Types

### Program Structure
- **main function**: The entry point of your C program
- **#include**: Include header files like `stdio.h`

### Output
- **print**: Print text without a newline (uses `printf`)
- **print with newline**: Print text with a newline

### Variables
- **Variable declaration**: Declare and initialize variables (int, char, float, double)
- **Variable assignment**: Assign values to existing variables

### Control Flow
- **if**: Conditional statement
- **for loop**: For loop with initialization, condition, and increment
- **while loop**: While loop with a condition

### Functions
- **Function declaration**: Create custom functions
- **return**: Return a value from a function

## Examples

### Hello World
The simplest program that prints "Hello world!":
1. Load the example using the "Load Example" button
2. Click "Export to hello.c" to download

### Custom Message
To change the message:
1. Click on the text block inside the print block
2. Change "Hello world!" to your custom message
3. Export the code

### Adding Variables
1. Drag a "Variable declaration" block into the main function
2. Set the type (int, char, float, double)
3. Set the variable name and initial value
4. Use the variable in your code

## Generated Code Format

The editor generates clean, readable C code that:
- Includes necessary headers
- Follows standard C formatting conventions
- Can be compiled with GCC: `gcc -o hello hello.c`
- Maintains proper indentation

## Browser Compatibility

Works with modern browsers:
- Chrome/Edge (recommended)
- Firefox
- Safari
- Opera

## Troubleshooting

**Problem**: Editor doesn't load
- Solution: Ensure you have an internet connection (Blockly library loads from CDN)

**Problem**: Export doesn't work
- Solution: Check browser permissions for downloads

**Problem**: Blocks don't connect
- Solution: Make sure blocks are compatible (check their shapes and colors)

## Technical Details

- Built with [Google Blockly](https://developers.google.com/blockly)
- Pure client-side HTML/JavaScript (no server required)
- Custom C code generator
- Responsive design for different screen sizes
