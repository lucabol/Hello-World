# Hello World
Test repo for JediMaster

## Visual C Code Editor

This repository includes a web-based visual block editor that allows you to create and modify C code using a simple drag-and-drop interface.

### How to Use the Visual Editor

1. Open `editor.html` in your web browser
2. Click on blocks from the left panel to add them to your program workspace
3. Edit the values in each block as needed
4. Click "Generate Code" to see the C code generated from your blocks
5. Click "Download hello.c" to save the generated code to a file
6. Compile the downloaded file with: `gcc -o hello hello.c`

### Available Blocks

- **#include <header.h>** - Include C header files (default: stdio.h)
- **int main() {}** - Main function (edit the function body directly)
- **printf("text")** - Print text to console
- **int var = value** - Declare and initialize variables
- **return 0** - Return from function
- **// comment** - Add comments to your code

### Example Workflow

1. Start with the default program (includes stdio.h and a Hello World main function)
2. Edit the text in the printf statement by clicking on the textbox in the main function body
3. Click "Generate Code" to see your changes
4. Click "Download hello.c" to save your program
5. Compile and run: `gcc -o hello hello.c && ./hello`