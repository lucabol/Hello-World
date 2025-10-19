# Visual Block-Based C Code Editor

## Overview

This project includes a web-based visual editor that allows you to create and modify C code using drag-and-drop blocks. The editor uses Google's Blockly library to provide an intuitive interface for building C programs.

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
- **Delete Blocks:** Drag unwanted blocks to the trash can
- **Zoom:** Use mouse wheel or zoom controls
- **Undo/Redo:** Use Ctrl+Z / Ctrl+Shift+Z

## Troubleshooting

### The editor doesn't open
- Make sure you're opening `editor.html` in a modern web browser (Chrome, Firefox, Edge, Safari)
- Check that you have internet connection (the editor loads Blockly from CDN)

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

The editor requires internet connection to load the Blockly library from CDN. For offline use, you would need to:
1. Download Blockly library locally
2. Update the script src in `editor.html` to point to local file

## Further Customization

To add more blocks or features, edit `editor.html`:
- Define new blocks in the `Blockly.defineBlocksWithJsonArray()` section
- Add code generators in the `cGenerator.forBlock` sections
- Update the toolbox configuration to include new categories
