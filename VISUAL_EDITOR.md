# Visual Block-Based C Code Editor

## Overview

This project includes a web-based visual block editor that allows you to create and modify C code using drag-and-drop blocks. The editor provides an intuitive interface for building C programs without writing code manually.

## Getting Started

### Opening the Editor

1. Open `block-editor.html` in your web browser
2. The editor will load with a default "Hello World" example

### Using the Editor

#### Block Toolbox

The toolbox on the left contains the following blocks:

- **📚 Include Header**: Add C header files (stdio.h, stdlib.h, string.h, math.h)
- **⚙️ Main Function**: Define the main() function (automatically included in code generation)
- **📝 Printf Statement**: Print text to console
- **🔢 Variable Declaration**: Declare and initialize variables (int, float, double, char)

#### Creating Your Program

1. **Drag blocks** from the toolbox to the canvas area
2. **Edit block fields** by clicking on dropdown menus or text inputs within each block
3. **Arrange blocks** by dragging them to different positions
4. **Delete blocks** by clicking the × button on any block
5. **Generate code** by clicking the "Generate Code" button
6. **Export code** by clicking the "💾 Export to hello.c" button

### Features

- **Drag-and-Drop Interface**: Intuitive block-based programming
- **Real-time Code Generation**: See your C code as you build it
- **Export Functionality**: Download generated code as hello.c
- **Visual Feedback**: Color-coded blocks for different types of operations
- **No External Dependencies**: Works offline, no internet connection required

## Block Types

### Include Header Block (Purple)
- Select from common C header files
- Automatically formatted with `# include <header.h>`

### Main Function Block (Red)
- Represents the main() function
- Other statement blocks will be placed inside main()

### Printf Statement Block (Green)
- Add printf statements to output text
- Edit the text to display in the input field
- Automatically adds proper formatting with quotes

### Variable Declaration Block (Orange)
- Declare variables with initialization
- Choose type: int, float, double, char
- Set variable name and initial value

## Code Generation

The editor generates properly formatted C code:

```c
# include <stdio.h>

int main(){
    int x = 0;
    printf("Hello world!");
}
```

### Code Structure

1. **Include directives** are placed at the top (sorted by Y position)
2. **Main function** wraps all statement blocks
3. **Statements** are ordered by their Y position on the canvas

## Exporting Code

1. Click "Generate Code" to preview the generated code
2. Click "💾 Export to hello.c" to download the file
3. The exported file can be compiled with: `gcc -o hello hello.c`

## Tips

- Blocks are ordered top-to-bottom based on their Y position
- You can have multiple blocks of the same type
- The "Clear All" button removes all blocks (with confirmation)
- Edit block values before generating code for best results

## Technical Details

- Pure HTML/CSS/JavaScript implementation
- No external libraries or dependencies
- Works in all modern web browsers
- Generates standard C code compatible with GCC and Clang compilers

## Screenshots

### Initial Editor View
![Block Editor Initial View](https://github.com/user-attachments/assets/f6f16993-14f7-4bc7-842a-4385d3a295e9)

### Editor with Multiple Blocks
![Block Editor with Variable](https://github.com/user-attachments/assets/73ccd1b3-c77c-489c-8259-b485b20d58b3)

## Compatibility

The visual editor works with:
- Chrome/Chromium (latest)
- Firefox (latest)
- Safari (latest)
- Edge (latest)

Generated code is compatible with:
- GCC (all versions)
- Clang (all versions)
- Any standard C compiler
