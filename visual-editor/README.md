# Visual Block-Based C Code Editor

A web-based visual programming editor for creating and modifying C code using drag-and-drop blocks.

## Features

- **Drag-and-drop interface**: Build C programs visually without typing code
- **Real-time code generation**: See the generated C code as you work
- **Custom C blocks**: Includes blocks for main function, printf, and more
- **Export functionality**: Download your visual program as `hello.c`
- **Beginner-friendly**: Perfect for learning C programming concepts

## How to Use

### Opening the Editor

Simply open `index.html` in any modern web browser:

```bash
# From the visual-editor directory
open index.html
# or
firefox index.html
# or
google-chrome index.html
```

No server or installation required! The editor runs entirely in your browser.

### Creating Your First Program

1. **Start with the Main Function**: The main function block is the entry point of your C program
2. **Add Print Blocks**: Drag a "print" block from the toolbox into the main function
3. **Customize Text**: Click on the text field to edit what you want to print
4. **Generate Code**: The C code is generated automatically as you work
5. **Download**: Click "Download hello.c" to save your program

### Available Blocks

#### Program Structure
- **main function**: The entry point of every C program

#### Output
- **print**: Prints text without a newline (uses `printf`)
- **print with newline**: Prints text followed by a newline

#### Values
- **text**: A text string value
- **number**: A numeric value

## Example Programs

### Hello World (Default)
The editor starts with a simple Hello World program:
```c
# include <stdio.h>

int main(){
    printf("Hello world!");
}
```

### Multiple Prints
You can stack multiple print blocks:
```c
# include <stdio.h>

int main(){
    printf("Hello ");
    printf("world!");
}
```

## Technical Details

- **Built with**: [Blockly](https://developers.google.com/blockly) - Google's visual programming library
- **No dependencies**: Uses Blockly via CDN
- **Browser compatibility**: Works in all modern browsers (Chrome, Firefox, Safari, Edge)
- **No server required**: Pure client-side application

## Integration with the Repository

The generated C code follows the same format as the repository's `hello.c`:
- Uses `# include <stdio.h>` (with space after #, matching the repository style)
- Implements `int main()` function
- Compatible with the existing build and test infrastructure

### Workflow

1. Create your program in the visual editor
2. Download `hello.c`
3. Replace the repository's `hello.c` with your downloaded file
4. Build and test as usual:
   ```bash
   gcc -o hello hello.c
   ./hello
   ```

## Extending the Editor

To add new blocks, edit `index.html` and add:

1. **Block definition** (JavaScript):
   ```javascript
   Blockly.Blocks['your_block_name'] = {
       init: function() {
           // Define block appearance and behavior
       }
   };
   ```

2. **Code generator** (JavaScript):
   ```javascript
   Blockly.JavaScript['your_block_name'] = function(block) {
       // Generate C code
       return 'your C code here';
   };
   ```

3. **Toolbox entry** (XML):
   ```xml
   <block type="your_block_name"></block>
   ```

## Browser Developer Tools

You can open the browser console (F12) to see any errors or debug the editor.

## License

This visual editor is part of the Hello-World repository and follows the same license terms.
