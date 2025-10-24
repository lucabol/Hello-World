# Visual C Code Editor - User Guide

## Overview
The Visual C Code Editor is a web-based block programming interface that allows you to create C programs using drag-and-drop blocks. It's perfect for beginners learning C programming or for quickly prototyping simple C programs.

## Getting Started

### Opening the Editor
Open `tools/editor/editor.html` in any modern web browser:
- **Chrome/Edge**: Double-click the file or drag it into the browser
- **Firefox**: Double-click the file or use File → Open File
- **Safari**: Double-click the file
- **Via Web Server**: `python3 -m http.server 8080` then navigate to `http://localhost:8080/tools/editor/editor.html`

## Accessibility Features

### Keyboard Navigation
The editor is fully accessible via keyboard:
- **Tab** - Navigate between palette blocks, workspace blocks, and buttons
- **Enter** or **Space** - Add a block from the palette to workspace
- **Delete** or **Backspace** - Remove focused block from workspace
- **Arrow keys** - Navigate within text inputs

### Screen Reader Support
- All interactive elements have ARIA labels
- Live regions announce changes to the workspace and code preview
- Semantic HTML roles for better navigation
- Descriptive button labels and alt text

### Keyboard-Only Workflow
1. Tab to a palette block
2. Press Enter or Space to add it to workspace
3. Tab to the new block's input field
4. Type your value
5. Tab to delete button or next block
6. Press Enter on Export button to download

## Features

### 📦 Block Palette (Left Panel)
Contains three types of blocks you can drag into your workspace:

1. **📚 Include Library** - Add C header files (e.g., stdio.h, stdlib.h)
2. **🖨️ Print Text** - Add printf statements to output text
3. **↩️ Return Value** - Add a return statement to the main function

### 🎨 Workspace (Center Panel)
The main area where you build your program:
- **Drag blocks** from the palette into this area
- **Edit values** in each block by typing in the text boxes
- **Delete blocks** using the 🗑️ button on each block
- Blocks are executed in order from top to bottom

### 👁️ Code Preview (Right Panel)
Shows the generated C code in real-time:
- Updates automatically as you add, modify, or remove blocks
- Displays properly formatted C code with syntax highlighting
- Shows exactly what will be exported

## How to Use

### Creating a "Hello World" Program
The editor starts with a default "Hello World" program already set up:
1. One Include block with `stdio.h`
2. One Printf block with `Hello world!`

### Building a Custom Program
1. **Clear the workspace** (optional): Click "Clear All Blocks" to start fresh
2. **Add an Include block**: Drag from palette, enter header name (e.g., `stdio.h`)
3. **Add Printf blocks**: Drag from palette, enter the text you want to print
4. **Add a Return block** (optional): Drag from palette, enter return value (usually `0`)
5. **Watch the preview**: See your C code update in real-time

### Exporting Your Code
1. Click the **"💾 Export to hello.c"** button
2. Your browser will download a `hello.c` file
3. Compile it with: `gcc hello.c -o hello`
4. Run it with: `./hello`

## Example Workflows

### Simple Print Program
1. Add Include block: `stdio.h`
2. Add Printf block: `Hello, World!`
3. Export and compile

### Multiple Prints
1. Add Include block: `stdio.h`
2. Add Printf block: `Line 1\n` (use `\n` for new line)
3. Add Printf block: `Line 2\n`
4. Add Printf block: `Line 3\n`
5. Export and compile

### Program with Return Value
1. Add Include block: `stdio.h`
2. Add Printf block: `Program starting...\n`
3. Add Return block: `0` (indicates success)
4. Export and compile

## Tips & Tricks

### Formatting Output
- Use `\n` for newline: `Hello\nWorld!` prints on two lines
- Use `\t` for tab: `Name:\tJohn` adds spacing
- The default blocks produce output without newline: `Hello world!`

### Multiple Includes
You can add multiple Include blocks for different libraries:
- `stdio.h` - Standard input/output
- `stdlib.h` - Standard library functions
- `string.h` - String manipulation
- `math.h` - Math functions

### Block Order Matters
Blocks execute in order from top to bottom, just like lines of C code:
```c
printf("First");   // First Printf block
printf("Second");  // Second Printf block
printf("Third");   // Third Printf block
```

### Deleting Blocks
- Click the 🗑️ button on any block to remove it
- Click "Clear All Blocks" to remove everything and start over
- A confirmation dialog prevents accidental deletion

## Security Features
The editor includes comprehensive security measures:
- **XSS Protection**: All user input is HTML-escaped to prevent script injection
- **Format String Safety**: Percent signs (%) are escaped to prevent printf vulnerabilities
- **Safe DOM Manipulation**: Uses textContent instead of innerHTML for user data
- **Fixed Filename**: Export filename is hardcoded as `hello.c` (cannot be manipulated)
- **Content Security Policy**: CSP headers prevent external script execution
- **No External Dependencies**: Runs entirely in your browser - no data sent to servers
- **Validated by Tests**: Comprehensive test suite validates security features

## Browser Compatibility
Works in all modern browsers:
- ✅ Chrome 90+
- ✅ Firefox 88+
- ✅ Safari 14+
- ✅ Edge 90+
- ✅ Opera 76+

## Troubleshooting

### Editor not loading?
- Make sure JavaScript is enabled in your browser
- Try a different browser
- Check browser console for errors (F12)

### Drag and drop not working?
- Make sure you're dragging from the Block Palette
- Drop the block in the gray dashed area
- **Touch devices**: Drag-and-drop has limited support on touchscreens
  - **Workaround**: Use keyboard navigation instead (see Accessibility section)
  - Press Enter/Space on palette blocks to add them
  - This provides a better experience on touch devices

### Export button not working?
- Check if pop-up blocker is preventing download
- Make sure browser has permission to download files
- Try right-click → Save As on the export button

### Generated code won't compile?
- Make sure you have GCC installed: `gcc --version`
- Check the code preview for syntax errors
- Verify all required includes are present
- Make sure string values are properly formatted

## Advanced Usage

### Working with Strings
When adding text to Printf blocks:
- Regular text: `Hello world!`
- With newline: `Hello world!\n`
- Multiple lines: `Line 1\nLine 2\nLine 3\n`
- With tabs: `Name:\tJohn` (for spacing)
- **Percent signs**: Use `%%` for literal percent (e.g., `50%% complete`)
  - The editor automatically escapes single `%` to `%%` for safety

### Auto-Include Feature
The editor automatically adds `#include <stdio.h>` if:
- You add a Printf block
- No Include block with `stdio.h` exists
This ensures your code always compiles successfully!

### Program Structure
The editor generates code with this structure:
```c
#include <stdio.h>    // Include blocks appear here

int main(){           // Main function starts
    printf("...");    // Printf blocks appear here
    return 0;         // Return block appears here
}                     // Main function ends
```

### Limitations
- Currently supports simple printf and include statements
- No support for variables, loops, or conditionals yet
- Best suited for simple "Hello World" style programs
- For complex programs, use a traditional code editor

## Educational Use
This editor is perfect for:
- **Learning C basics**: Understand program structure
- **Teaching programming**: Visual approach for beginners
- **Quick prototyping**: Test simple C output quickly
- **Code generation**: Generate starter templates

## Contributing
Found a bug or want to suggest a feature? The editor is open source and contributions are welcome!

## Technical Details
- **File size**: ~17KB (no dependencies)
- **Languages**: HTML, CSS, JavaScript
- **APIs used**: DOM, Blob, URL, File Download
- **Security**: XSS protection, input sanitization
- **Testing**: Comprehensive test suite included

---

**Enjoy building C programs visually! 🧱**
