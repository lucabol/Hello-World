# Visual C Code Editor

This repository includes a web-based visual block editor that allows you to create and modify C code using drag-and-drop blocks.

## Quick Start

1. **Start the server:**
   ```bash
   python3 server.py
   ```
   The server will start at http://localhost:8080

2. **Open the editor:**
   Navigate to http://localhost:8080 in your web browser

3. **Build your program:**
   - Drag blocks from the "Available Blocks" panel on the left
   - Drop them in the "Program Workspace" in the center
   - Watch the "Generated C Code" update on the right

4. **Export your code:**
   Click "Export to hello.c" to download the generated C code

## Available Blocks

The visual editor provides the following code blocks:

- **#include <stdio.h>** - Standard I/O header inclusion
- **int main() {** - Main function declaration
- **printf("Hello world!");** - Print statement
- **return 0;** - Return statement
- **}** - Closing brace

## Features

- **Drag & Drop Interface:** Intuitive block-based programming
- **Live Preview:** See generated C code in real-time
- **Export Functionality:** Download your code as hello.c
- **Block Management:** Remove individual blocks or clear all
- **Responsive Design:** Works on desktop and mobile browsers

## Workflow

1. Start with the `#include <stdio.h>` block
2. Add `int main() {` to begin the main function
3. Add `printf("Hello world!");` for the output
4. Close with `}` to complete the function
5. Export the generated code

## Generated Code

The visual editor generates clean, properly formatted C code that:
- Compiles with GCC without warnings
- Produces the exact output: "Hello world!" (no trailing newline)
- Exits with code 0
- Maintains compatibility with the original hello.c behavior

## Technical Details

- **Frontend:** HTML5, CSS3, JavaScript (no external dependencies)
- **Backend:** Python 3 HTTP server
- **Port:** Configurable (default: 8080)
- **Export Format:** Standard C source file (.c extension)

## Customization

The block templates can be modified in the `visual-editor.html` file by editing the `blockTemplates` object in the JavaScript section.