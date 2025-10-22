# Visual Block-Based C Code Editor

## Overview

This repository includes a web-based visual block editor that allows you to create and modify C code using drag-and-drop blocks. The editor generates valid C code that can be exported to `hello.c`.

## Getting Started

### Opening the Editor

1. Open `block-editor.html` in any modern web browser
2. Or serve it via a local web server:
   ```bash
   python3 -m http.server 8080
   # Then open http://localhost:8080/block-editor.html
   ```

### Using the Editor

The editor consists of three main panels:

#### 1. Block Toolbox (Left Panel)
Contains draggable block templates:
- **📚 Include Library**: Add C library includes (e.g., stdio.h, stdlib.h)
- **📝 Print Text**: Add printf statements
- **💬 Comment**: Add code comments
- **🔢 Variable**: Declare integer variables

#### 2. Workspace (Center Panel)
Your code construction area:
- Drag blocks from the toolbox into this area
- Edit field values in each block
- Remove blocks using the "Remove" button
- The editor starts with a default "Hello world!" program

#### 3. Generated C Code (Right Panel)
Shows the real-time C code output:
- Code updates automatically as you modify blocks
- View the exact C code that will be generated
- Dark theme for better code readability

## Features

### Drag and Drop
- Click and hold any block in the toolbox
- Drag it to the workspace or drop zone
- Release to add the block to your program

### Edit Block Fields
- Each block has editable text fields
- Click any field to modify its value
- Changes are reflected immediately in the generated code

### Code Generation
The editor automatically generates valid C code with:
- Proper include statements at the top
- Main function structure
- Correct C syntax and formatting
- Real-time updates as you edit

### Export Code
1. Click the **"💾 Download hello.c"** button
2. Save the file to your computer
3. Replace the existing `hello.c` in the repository with your download
4. Compile and run as usual: `gcc -o hello hello.c && ./hello`

### Reset Workspace
- Click **"🔄 Reset"** to restore the default "Hello world!" program
- Useful for starting over or reverting changes

## Example Workflows

### Modifying the Hello World Message

1. Open the editor
2. Click on the "Text to print" field in the Print Text block
3. Change "Hello world!" to your desired message
4. Click "Download hello.c" to save
5. Replace the original hello.c file

### Adding Comments

1. Drag the "💬 Comment" block from the toolbox
2. Drop it in the workspace (it will be added before main)
3. Edit the comment text
4. Download the updated code

### Adding Variables

1. Drag the "🔢 Variable" block from the toolbox
2. Edit the variable name and initial value
3. The variable will be declared inside main()
4. Download and test the code

### Multiple Includes

1. Drag multiple "📚 Include Library" blocks
2. Change the library name in each (e.g., stdlib.h, string.h)
3. All includes will appear at the top of the generated code

## Generated Code Format

The editor generates C code in the following format:

```c
# include <stdio.h>
# include <stdlib.h>

int main(){
    // Comments appear here
    int count = 0;
    printf("Hello world!");
}
```

**Note**: The generated code uses `# include` (with a space) to match the repository's existing style.

## Browser Compatibility

The editor works in all modern browsers:
- Chrome/Edge (recommended)
- Firefox
- Safari
- Opera

**Requirements**:
- JavaScript enabled
- HTML5 drag-and-drop support
- No external dependencies or internet connection needed (after initial load)

## Tips and Best Practices

1. **Start Simple**: Begin with the default program and modify it incrementally
2. **Test Frequently**: Download and compile your code regularly to catch errors early
3. **Use Comments**: Add comments to document your code structure
4. **Order Matters**: Includes should be at the top, followed by main function code
5. **Valid C Syntax**: The editor generates syntactically correct C, but you're responsible for logical correctness

## Limitations

- The editor focuses on simple C programs suitable for this repository
- Advanced C features (functions, loops, conditionals) are not included in the basic toolbox
- You can only edit one file at a time (hello.c)
- The editor is designed for educational purposes and simple programs

## Troubleshooting

### Generated Code Won't Compile

1. Verify all required includes are present (at minimum stdio.h for printf)
2. Check that text fields don't contain invalid C syntax
3. Ensure quotes and special characters are properly escaped

### Drag and Drop Not Working

1. Ensure JavaScript is enabled in your browser
2. Try refreshing the page
3. Use a supported modern browser

### Download Button Not Working

1. Check that your browser allows downloads
2. Ensure popup blockers aren't interfering
3. Try right-click and "Save As" if needed

## Integration with Repository

After generating code with the block editor:

1. Download the `hello.c` file
2. Replace the existing `/home/runner/work/Hello-World/Hello-World/hello.c`
3. Test compilation: `gcc -Wall -Wextra -o hello hello.c`
4. Run the program: `./hello`
5. Verify output matches expectations
6. Run validation: `./test/validate.sh`

The generated code is compatible with the repository's build process and validation scripts.

## Future Enhancements

Potential features for future versions:
- More block types (loops, conditionals, functions)
- Block reordering within workspace
- Save/load workspace state
- Code templates library
- Syntax validation in real-time
- Multiple file support
- Advanced C language features
