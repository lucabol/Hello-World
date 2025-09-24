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

4. **Save your code:**
   Choose from two save options:
   - **Download hello.c** - Downloads the generated code to your local machine
   - **Save to Server** - Saves the code directly to the server's hello.c file

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
- **Dual Save Options:** 
  - Client-side download for local development
  - Server-side save for direct file modification
- **Block Management:** Remove individual blocks or clear all
- **Error Handling:** Comprehensive validation and error reporting
- **Responsive Design:** Works on desktop and mobile browsers

## Save Options Explained

### Download hello.c (Client-side)
- Downloads the generated C code as a file to your computer
- Works offline once the page is loaded
- Useful for local development workflows
- No server interaction required for the save operation

### Save to Server (Server-side)
- Saves the generated code directly to the server's hello.c file
- Useful for development environments where the server has the build tools
- Includes validation and error handling
- Provides immediate feedback on save success/failure

## Workflow

1. Start with the `#include <stdio.h>` block
2. Add `int main() {` to begin the main function
3. Add `printf("Hello world!");` for the output
4. Close with `}` to complete the function
5. Choose your preferred save method:
   - **Download** for local development
   - **Save to Server** for server-based development

## Generated Code

The visual editor generates clean, properly formatted C code that:
- Compiles with GCC without warnings
- Produces the exact output: "Hello world!" (no trailing newline)
- Exits with code 0
- Maintains compatibility with the original hello.c behavior

## Technical Details

- **Frontend:** HTML5, CSS3, JavaScript (no external dependencies)
- **Backend:** Python 3 HTTP server with enhanced error handling
- **Port:** Configurable (default: 8080)
- **Export Formats:** Standard C source file (.c extension)
- **Error Handling:** Comprehensive validation for both client and server operations

## Server API

The server provides a REST API endpoint:

- **POST /save-code** - Saves C code to hello.c
  - Content-Type: application/json
  - Body: `{"code": "C source code here"}`
  - Returns: JSON response with success/error status

## Customization

The block templates can be modified in the `visual-editor.html` file by editing the `blockTemplates` object in the JavaScript section.