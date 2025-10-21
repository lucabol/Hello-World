# Visual Block-Based C Code Editor

<!--
Copyright (c) 2024 lucabol/Hello-World Contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
-->

## Overview

This project includes a web-based visual editor that allows you to create and modify C code using drag-and-drop blocks. The editor is a custom, self-contained implementation built with pure HTML, CSS, and JavaScript—no external libraries or frameworks required.

## Getting Started

### Opening the Editor

1. **Option 1 - Direct File Access:**
   - Simply open `editor.html` in your web browser
   - On Linux/Mac: `open editor.html` or `xdg-open editor.html`
   - On Windows: Double-click `editor.html`

2. **Option 2 - Local Web Server:**
   ```bash
   # Using Python 3
   python3 -m http.server 8000
   
   # Using Python 2
   python -m SimpleHTTPServer 8000
   
   # Using Node.js (if you have http-server installed)
   npx http-server
   ```
   Then navigate to `http://localhost:8000/editor.html`

## Using the Editor

### Interface Layout

The editor interface is divided into three main areas:

1. **Left Panel (Toolbox):** Contains categories of blocks you can drag into the workspace
2. **Center Panel (Workspace):** Where you build your program by connecting blocks
3. **Right Panel (Code Preview):** Shows the generated C code in real-time

### Available Blocks

#### Includes Category (Purple)
- **#include block:** Add C library includes (stdio.h, stdlib.h, string.h, math.h)

#### Program Structure Category (Green)
- **int main() block:** The main function container
- **return block:** Return statement with a value (default: 0)

#### Output Category (Blue)
- **printf block:** Print text to console
- **string block:** String literal (e.g., "Hello world!")

### Building Your First Program

1. Click on **"Includes"** category in the toolbox
2. Drag the **#include** block to the workspace
3. Select **"stdio.h"** from the dropdown

4. Click on **"Program Structure"** category
5. Drag the **int main()** block and connect it below the include block

6. Click on **"Output"** category
7. Drag the **printf** block inside the main function
8. Drag the **string** block and connect it to the printf block
9. Edit the string text as needed (default is "Hello world!")

### Buttons

- **Load Example:** Loads a pre-built "Hello World" program
- **Clear:** Clears all blocks from the workspace
- **Export to hello.c:** Downloads the generated C code as `hello.c`

### Exporting Your Code

1. Build your program using the blocks
2. Review the generated code in the right panel
3. Click **"Export to hello.c"** button
4. Save the file to replace your project's `hello.c`
5. Compile and run as usual: `gcc -o hello hello.c && ./hello`

## Example: Creating "Hello World"

The editor loads with a default "Hello World" example. Here's what it looks like:

```
[#include <stdio.h>]
    ↓
[int main() {]
    └─→ [printf("Hello world!")]
[}]
```

This generates:
```c
# include <stdio.h>

int main(){
    printf("Hello world!");
}
```

## Tips

- **Drag and Drop:** Click and hold a block to drag it
- **Connect Blocks:** Blocks snap together when they're compatible
- **Delete Blocks:** Click the × button on any block to remove it
- **Keyboard Navigation:** Use Tab to move between input fields, Enter to activate buttons
- **Text Editing:** Click on text fields within blocks to edit them directly

## Accessibility and Keyboard Support

The editor provides keyboard accessibility:
- **Tab/Shift+Tab**: Navigate between buttons and input fields
- **Enter/Space**: Activate buttons (Load Example, Clear, Export)
- **Arrow keys**: Navigate within dropdown menus (include selection)
- **Text input**: All editable fields (printf text, return values) support standard keyboard input

**Current Limitations**:
- Drag-and-drop is primarily mouse-based. Keyboard users can use the "Load Example" button to get started, then modify block parameters using Tab and input fields
- Screen reader support is basic; we recommend using the code preview panel to verify generated code

## Export Behavior

When you click "Export to hello.c":
1. The editor generates C code from your current blocks
2. The browser downloads a file named `hello.c` to your default downloads folder
3. **No automatic file replacement occurs** - the export only downloads a file to your local system
4. You manually replace the repository's `hello.c` with the downloaded file if desired
5. If a file named `hello.c` already exists in your downloads folder, your browser's standard behavior applies (rename, replace, or prompt)

The editor does not and cannot directly modify files in the repository—it only downloads generated code.

## Troubleshooting

### The editor doesn't open
- Make sure you're opening `editor.html` in a modern web browser (Chrome, Firefox, Edge, Safari)
- The editor is fully self-contained and works offline (no internet connection required)

### Code doesn't export
- Make sure you've added blocks to the workspace
- Check that your browser allows file downloads

### Generated code has errors
- Ensure all required blocks are connected properly
- Every program needs at least an include and a main function
- Check that string blocks are properly connected to printf blocks

## Browser Compatibility

The editor works best in modern browsers:
- ✅ Chrome/Chromium 90+
- ✅ Firefox 88+
- ✅ Edge 90+
- ✅ Safari 14+

## Offline Use

The editor is fully self-contained and works completely offline. No internet connection is required after downloading the editor.html file.

## Further Customization

To add more blocks or features, edit `editor.html`:

### Adding New Block Types
1. Add a new block template in the toolbox HTML (around line 200)
2. Add rendering logic in the `renderBlock()` function (around line 350)
3. Add code generation logic in the `generateCode()` function (around line 450)

### Block Definition Structure
Each block type has three parts:
- **Template**: Visual representation in the toolbox
- **Renderer**: How the block appears in the workspace with input fields
- **Generator**: How the block translates to C code

Example for adding a new block type:
```javascript
// In renderBlock() function:
else if (block.type === 'myblock') {
    content = `
        <div class="block-instance myblock" data-id="${block.id}">
            <button class="delete-btn" data-id="${block.id}">×</button>
            // Your block HTML here
        </div>
    `;
}

// In generateCode() function:
const myblocks = blocks.filter(b => b.type === 'myblock');
myblocks.forEach(block => {
    code += `// Your generated C code here\n`;
});
```

### Security Considerations
The editor sanitizes user input to prevent code injection:
- Include library names are restricted to a dropdown (stdio.h, stdlib.h, string.h, math.h)
- Printf text and return values are treated as plain text and properly quoted/escaped in generated code
- No eval() or dynamic code execution is used
- All user input is validated before code generation

**Note**: Users can edit the downloaded hello.c file after export. The editor only controls what it generates, not what users do with the exported file.
