# Visual C Code Block Editor

This is a web-based visual editor that allows users to create C code using drag-and-drop blocks, and then export the result to `hello.c`.

## Features

- **Drag and Drop Interface**: Drag blocks from the sidebar to the workspace
- **Visual Block Programming**: Create C code without typing
- **Real-time Code Generation**: See the generated C code as you build
- **Export Functionality**: Save generated code directly to `hello.c`
- **Multiple Block Types**: Support for includes, functions, statements, and variables

## Block Types

### Program Structure
- **#include blocks**: Add header files (default: `#include <stdio.h>`)
- **main() function**: Creates the main function structure

### Statements
- **printf statement**: Print text to console
- **return statement**: Return values from functions

### Variables
- **int variable**: Declare integer variables
- **char variable**: Declare character variables

## How to Use

### Starting the Editor

1. Start the server:
   ```bash
   python3 server.py
   ```

2. Open your browser and navigate to:
   ```
   http://localhost:8081/block-editor.html
   ```

### Creating Code

1. **Drag blocks** from the sidebar to the workspace
2. **Edit values** in the input fields within blocks
3. **Click "Generate Code"** to see the C code
4. **Click "Export to hello.c"** to save the code

### Example Workflow

1. Drag an `#include <stdio.h>` block to the workspace
2. Drag a `main() function` block
3. Drag a `printf statement` block
4. Edit the printf message if needed
5. Generate the code
6. Export to hello.c

## Generated Code Format

The editor generates properly formatted C code that matches the original hello.c format:

```c
#include <stdio.h>

int main(){
    printf("Hello world!");
}
```

## Technical Details

- **Frontend**: HTML5, CSS3, JavaScript (vanilla)
- **Backend**: Python 3 HTTP server
- **File I/O**: Direct file writing to hello.c
- **Browser Compatibility**: Modern browsers with HTML5 drag-and-drop support

## Files

- `block-editor.html` - The main visual editor interface
- `server.py` - Python server with export functionality
- `README-visual-editor.md` - This documentation

## Development

The editor uses a modular block system where each block type has:
- HTML template for visual representation
- Code generation function
- Input field handling

To add new block types, extend the `blockTemplates` object in the JavaScript code.