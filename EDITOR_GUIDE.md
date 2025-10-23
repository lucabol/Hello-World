# Visual C Code Editor Guide

## Overview
The Visual C Code Editor is a web-based, drag-and-drop block editor that allows you to create C programs visually without writing code directly.

## Getting Started

### Opening the Editor
Simply open `editor.html` in any modern web browser (Chrome, Firefox, Safari, Edge).

No installation or server required - it runs entirely in the browser!

## Using the Editor

### 1. Block Palette (Left Panel)
The Block Palette contains three types of blocks you can use:

- **#include**: Include header files (e.g., stdio.h, stdlib.h)
- **printf()**: Print text output
- **return**: Return a value from main()

### 2. Workspace (Center Panel)
The Workspace is where you build your program:

- **Drag blocks** from the palette into the workspace
- **Edit values** in each block's input fields
- **Remove blocks** using the "✕ Remove" button
- Blocks are automatically organized into proper C code structure

### 3. Generated Code (Right Panel)
Watch your C code generate in real-time as you:
- Add blocks
- Remove blocks
- Edit block values

### 4. Export
Click the **"📥 Export to hello.c"** button to download your generated code as a C file.

## Example Usage

### Creating "Hello World"
1. The editor starts with a default program
2. Edit the printf block to change the message
3. Click Export to download

### Creating a Custom Program
1. Drag an **#include** block to add a header
2. Drag a **printf** block to print something
3. Drag a **return** block to return 0
4. Edit each block's value as needed
5. Export your code!

## Tips

### Printf Formatting
- Use `\n` for newlines
- Use `\t` for tabs
- Example: `Hello, World!\n` prints with a newline

### Include Headers
- Just enter the header name (e.g., `stdio.h`)
- The angle brackets are added automatically

### Program Structure
The editor automatically creates:
- Includes at the top
- `int main(){` wrapper
- Proper indentation
- Closing brace `}`

## Security
The editor includes multiple security features:
- XSS protection with HTML escaping
- Safe file downloads
- No code execution from user input
- Validated and tested for security vulnerabilities

## Browser Compatibility
Works with:
- Chrome/Chromium (recommended)
- Firefox
- Safari
- Edge
- Any modern browser with JavaScript enabled

## No Dependencies
- No installation required
- No internet connection needed
- No npm packages
- Just open and use!
