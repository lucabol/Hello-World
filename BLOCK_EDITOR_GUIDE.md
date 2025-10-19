# Visual Block Editor Guide

## Overview

The Visual Block Editor is a web-based tool that allows you to create and modify C code using drag-and-drop blocks. This makes C programming more accessible and visual, especially for beginners or for rapid prototyping.

## Getting Started

### Opening the Editor

1. Open the `block-editor.html` file in your web browser:
   ```bash
   # On Linux/Mac
   open block-editor.html
   
   # Or directly in your browser
   firefox block-editor.html
   # or
   chrome block-editor.html
   ```

2. You can also serve it with a simple HTTP server:
   ```bash
   # Using Python 3
   python3 -m http.server 8000
   
   # Then open http://localhost:8000/block-editor.html in your browser
   ```

### Interface Overview

The editor consists of three main sections:

1. **Block Workspace (Left Panel)**: Drag blocks from the toolbox and arrange them here
2. **Generated C Code (Right Panel)**: Shows the C code generated from your blocks in real-time
3. **Control Panel (Bottom)**: Contains action buttons for managing your code

## Using the Block Editor

### Available Block Categories

#### Program Structure
- **main function**: The entry point of your C program
- **#include**: Include standard C libraries (stdio.h, stdlib.h, string.h, math.h)

#### Output
- **print**: Print text without a newline (uses printf)
- **print with newline**: Print text with a newline at the end

#### Variables
- **int variable**: Declare and initialize an integer variable
- **char array**: Declare a character array (string)
- **assign**: Assign a value to an existing variable

#### Control Flow
- **if**: Conditional statement
- **while**: While loop
- **for**: For loop with counter

#### Values
- **string**: Text value in quotes
- **number**: Numeric value

### Creating a Simple Program

1. **Start with includes**: Drag an `#include` block and select `stdio.h`
2. **Add main function**: Drag the `main function` block below the include
3. **Add statements**: Inside the main function, add blocks for your program logic
4. **Add output**: Drag a `print` block and connect a `string` block to it

Example: Hello World Program
```
#include <stdio.h> block
    ↓
main function block
    └─ print block
         └─ "Hello world!" string block
```

### Control Buttons

- **🔄 Update Code Preview**: Manually refresh the code preview (updates automatically when blocks change)
- **💾 Export to hello.c**: Download the generated C code as `hello.c`
- **📄 Load Example**: Load a pre-made Hello World example
- **🗑️ Clear Workspace**: Remove all blocks from the workspace

## Creating the Classic Hello World

The editor loads with an example by default, but you can create it manually:

1. Drag `#include` block, keep it as `<stdio.h>`
2. Drag `main function` block and connect it below the include
3. Inside the main function, drag a `print` block
4. Drag a `string` block and connect it to the print block's input
5. Edit the string to say "Hello world!"
6. Click **Export to hello.c** to download

This will generate:
```c
# include <stdio.h>

int main(){
    printf("Hello world!");
}
```

## Exporting Your Code

1. Once you're satisfied with your block arrangement, click **💾 Export to hello.c**
2. The browser will download a file named `hello.c` with your generated code
3. You can then compile and run it:
   ```bash
   gcc -o hello hello.c
   ./hello
   ```

## Advanced Features

### Auto-Preview
The code preview updates automatically as you modify blocks, so you can see your changes in real-time.

### Drag and Drop
- Drag blocks from the toolbox to the workspace
- Drag blocks within the workspace to rearrange them
- Drop blocks into the trash can to delete them

### Block Connection
Blocks can be connected in different ways:
- **Vertical connection**: Stack blocks sequentially (for statements)
- **Input connection**: Insert value blocks into input slots (for parameters)
- **Nested connection**: Place blocks inside container blocks (for control flow)

### Zoom and Pan
- Use the zoom controls (+/-) at the bottom right of the workspace
- Scroll to zoom in/out
- Click and drag the background to pan around the workspace

## Tips and Best Practices

1. **Always start with includes**: Make sure to include necessary libraries like `<stdio.h>` for printf
2. **Use the main function**: All C programs need a main function as the entry point
3. **Check the preview**: Always review the generated code before exporting
4. **Save your work**: The editor doesn't auto-save, so export your code regularly
5. **Build incrementally**: Add blocks one at a time and verify the code looks correct

## Troubleshooting

### Code Doesn't Look Right
- Make sure blocks are properly connected (you'll see connection shapes match)
- Check that value blocks are inserted into input slots
- Verify the main function contains your statements

### Export Doesn't Work
- Ensure your browser allows downloads
- Check that you have blocks in the workspace
- Try a different browser if issues persist

### Blocks Won't Connect
- Make sure you're connecting compatible block types
- Statement blocks connect vertically
- Value blocks connect to input slots
- Check the block shapes - they should match

## Examples

### Hello World (Default)
```c
# include <stdio.h>

int main(){
    printf("Hello world!");
}
```

### Hello World with Newline
```c
# include <stdio.h>

int main(){
    printf("Hello world!\n");
}
```

### Program with Variable
```c
# include <stdio.h>

int main(){
    int count = 5;
    printf("Count: ");
    printf("%d", count);
}
```

## Technical Details

The block editor uses:
- **Blockly**: Google's library for building visual programming editors
- **Pure HTML/CSS/JavaScript**: No server-side code required
- **Browser-based**: Works entirely in your web browser
- **No compilation**: Generates C code as text for you to compile separately

## Limitations

- The editor generates code but doesn't compile or run it
- Advanced C features (pointers, structs, unions) are not included
- Generated code follows a specific formatting style
- You'll need a C compiler (like GCC) to actually run the code

## Future Enhancements

Potential improvements could include:
- More block types (arrays, functions, pointers)
- Better syntax support for complex expressions
- Workspace save/load functionality
- Direct compilation in the browser (using WebAssembly)
- Syntax error checking and validation

## Support

For issues or questions:
- Check the console for error messages (F12 in most browsers)
- Verify you're using a modern browser (Chrome, Firefox, Safari, Edge)
- Ensure JavaScript is enabled in your browser
