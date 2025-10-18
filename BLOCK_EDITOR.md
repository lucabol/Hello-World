# Visual Block-Based C Code Editor

This project includes a web-based visual block editor that allows you to create and modify C code using drag-and-drop blocks.

## Features

- **Drag-and-drop interface**: Build C programs visually without typing code
- **Real-time code generation**: See your C code update as you build with blocks
- **Export functionality**: Download generated code as `hello.c`
- **Pre-built blocks** for common C structures:
  - Library includes (`#include <stdio.h>`, etc.)
  - Main function (`int main()`)
  - Printf statements (with and without newline)
  - Return statements
  - Comments

## How to Use

### Opening the Editor

1. Open the `block-editor.html` file in any modern web browser:
   ```bash
   # On Linux/Mac
   open block-editor.html
   
   # Or with a web browser directly
   firefox block-editor.html
   google-chrome block-editor.html
   ```

2. Alternatively, you can serve it with a simple HTTP server:
   ```bash
   # Python 3
   python3 -m http.server 8000
   
   # Then open http://localhost:8000/block-editor.html in your browser
   ```

### Building Code with Blocks

1. **Load Example**: Click the "Load Example" button to see a sample "Hello World" program
2. **Add Blocks**: Drag blocks from the left sidebar into the workspace
   - Start with library includes (typically `#include <stdio.h>`)
   - Add a main function block
   - Drag statement blocks (like printf) inside the main function
3. **Configure Blocks**: Click on dropdown fields and text fields in blocks to customize them
4. **View Code**: The generated C code appears in the right panel automatically

### Block Categories

- **Libraries**: Include header files
  - Choose from: stdio.h, stdlib.h, string.h, math.h
  
- **Functions**: Function definitions
  - `int main()` - Program entry point
  
- **Output**: Print statements
  - `printf("text")` - Print without newline
  - `printf("text\n")` - Print with newline
  
- **Control**: Control flow statements
  - `return 0;` - Return from function
  
- **Comments**: Documentation
  - `// comment` - Single-line comments

### Exporting Code

1. **Copy to Clipboard**: Click "Copy Code" to copy the generated C code
2. **Download File**: Click "Export to hello.c" to download the code as `hello.c`
3. **Manual Save**: You can also copy the code from the right panel and paste it into `hello.c` manually

### Example: Creating "Hello World"

1. Click "Load Example" to load the default Hello World program, or:
2. Drag an `#include <stdio.h>` block to the workspace
3. Drag an `int main()` block below it
4. Drag a `printf` block into the main function body
5. Type "Hello world!" in the text field
6. Click "Export to hello.c" to save

## Building and Running the Generated Code

After exporting the code to `hello.c`:

```bash
# Compile
gcc -o hello hello.c

# Run
./hello
```

Or use the test validation:
```bash
./test/validate.sh
```

## Browser Compatibility

The block editor works in all modern browsers:
- Chrome/Chromium (recommended)
- Firefox
- Safari
- Edge

**Note**: An internet connection is required for the first load as it uses the Blockly library from a CDN.

## Technical Details

- **Framework**: Google Blockly (loaded from CDN)
- **File Type**: Single HTML file (no installation required)
- **Code Generation**: Real-time JavaScript-based C code generation
- **Offline Use**: After first load, browsers may cache the Blockly library

## Troubleshooting

**Q: The blocks aren't showing up**
- Make sure you're opening the file in a web browser
- Check your internet connection (Blockly loads from CDN)
- Try a different browser

**Q: The export button doesn't work**
- Make sure you have blocks in the workspace
- Check that your browser allows downloads
- Try the "Copy Code" button instead

**Q: Generated code won't compile**
- Ensure you have an `#include` block for any library functions you use
- Make sure all blocks are properly connected (no gaps)
- Verify the printf text doesn't contain unescaped quotes

## Advanced Usage

### Creating Custom Blocks

You can modify `block-editor.html` to add custom blocks for your specific needs. Look for the `Blockly.defineBlocksWithJsonArray` section in the JavaScript code.

### Workspace Persistence

The workspace is reset when you reload the page. To save your work:
1. Export the code regularly
2. Or modify the HTML to add localStorage persistence

## Contributing

Feel free to extend the block editor with additional C language features:
- Variables and declarations
- Control structures (if/else, loops)
- Functions beyond main()
- More complex expressions
- Additional library functions

---

**Need help?** Check the [main README](README.md) for information about building and testing C code in this project.
