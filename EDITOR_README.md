# Visual C Code Block Editor

This project includes a web-based visual block editor that allows you to create and modify C code using an intuitive drag-and-drop interface.

## Getting Started

### Opening the Editor

**Recommended approach:** Serve the file via HTTP to avoid browser restrictions

1. Using Python's built-in HTTP server:
   ```bash
   python3 -m http.server 8080
   ```
   Then navigate to `http://localhost:8080/editor.html`

2. Or open `editor.html` directly in your browser:
   - Most modern browsers support opening local HTML files via `file://` protocol
   - If you encounter issues with file downloads, use the HTTP server method above

### Using the Editor

The editor interface has two main sections:

#### Left Panel - Block Toolbox and Workspace
- **Available Blocks**: Click on any block to add it to your program
  - 📚 **Include Header**: Add `#include` directives for C libraries
  - 🏁 **Main Function**: The entry point of your C program
  - 💬 **Print Statement**: Print text to the console using `printf()`
  - ↩️ **Return Statement**: Return a value from a function

- **Your Program Blocks**: The workspace where your blocks appear
  - Each block has controls to edit, reorder, indent, or remove it

#### Right Panel - Generated Code and Controls
- **Generated C Code**: Shows the C code created from your blocks
- **🔄 Generate Code**: Manually refresh the generated code (auto-updates as you work)
- **💾 Export to hello.c**: Download the generated code as a file

## Block Controls

Each block in the workspace has several controls:

- **✏️ Edit**: Modify the block's content (e.g., change text in a printf statement)
- **⬆️**: Move the block up in the program
- **⬇️**: Move the block down in the program
- **⬅️**: Decrease indentation (move left)
- **➡️**: Increase indentation (move right)
- **🗑️ Remove**: Delete the block from your program

## Creating a Basic C Program

A typical C program structure:

1. Click **Include Header** to add `#include <stdio.h>`
2. Click **Main Function** to add the main function
3. Click **Print Statement** to add a printf inside main
4. Click **Return Statement** to add `return 0;` inside main
5. Use the **⬅️** and **➡️** buttons to adjust indentation so printf and return are inside main

The generated code will look like:
```c
#include <stdio.h>

int main(){
    printf("Hello world!");
    return 0;
}
```

## Exporting Your Code

1. Review the generated C code in the right panel
2. Click **💾 Export to hello.c**
3. The file will be downloaded to your computer
4. Compile and run it:
   ```bash
   gcc -o hello hello.c
   ./hello
   ```

## Tips

- The code auto-generates as you add and modify blocks
- Use indentation controls to organize code inside functions
- You can have multiple printf statements by adding multiple Print blocks
- Edit blocks to customize the text, header files, or return values

## Technical Details

- The editor is built with vanilla JavaScript (no external dependencies)
- All code generation happens in the browser
- The exported file is a standard C source file compatible with GCC and other C compilers
- No internet connection required after the page loads
- **Important:** The editor uses `file://` protocol which works in most browsers, but serving via HTTP is recommended for best compatibility
- User input is properly escaped to prevent XSS vulnerabilities
- Keyboard navigation is fully supported with Tab and Enter keys
- ARIA labels are provided for screen reader accessibility

## Troubleshooting

**Q: My code doesn't indent properly**
- Use the ⬅️ and ➡️ buttons to adjust indentation manually
- Statements like printf and return should typically be indented inside the main function

**Q: The exported file won't compile**
- Make sure you have a valid C program structure (include header, main function)
- Check that printf statements are inside the main function
- Verify that the main function has a return statement

**Q: The export produces an empty file**
- Make sure you have added blocks to your program before exporting
- The editor will alert you if there are no blocks to export
- Try adding at least an include header and main function

**Q: How do I start over?**
- Use the 🗑️ Remove button on each block to clear the workspace
- Refresh the page to reload the default "Hello world!" program

**Q: Does the editor save my work?**
- Currently, the editor does not persist blocks to localStorage
- Your work is lost when you refresh the page or close the browser
- Always export your code before closing the editor

## Example Workflows

### Creating a Custom Greeting

1. Start with the default program
2. Click ✏️ Edit on the printf block
3. Change the text to your custom message
4. Click **Export to hello.c** to download

### Building from Scratch

1. Remove all default blocks using 🗑️
2. Add blocks in this order:
   - Include Header (stdio.h)
   - Main Function
   - Print Statement (adjust indentation with ➡️)
   - Return Statement (adjust indentation with ➡️)
3. Edit the printf text to your liking
4. Export when ready

## Future Enhancements

Potential features for future versions:
- Support for variables and data types
- Conditional statements (if/else)
- Loops (for, while)
- Functions beyond main()
- Multiple file support
- Syntax highlighting
- Block categories and search
