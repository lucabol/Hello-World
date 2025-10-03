# Visual Editor Screenshot Guide

This document describes what you'll see when you open the visual editor.

## Interface Layout

```
╔══════════════════════════════════════════════════════════════════════════════╗
║                    🧩 Visual C Code Editor                                    ║
║          Create C code using visual blocks - perfect for learning!           ║
╚══════════════════════════════════════════════════════════════════════════════╝

┌─────────────────────────────────────┬────────────────────────────────────────┐
│                                     │                                        │
│  📦 Block Palette - Click to Add   │     📄 Generated C Code                │
│  ┌────────────────────────────────┐│  ┌──────────────────────────────────┐ │
│  │ ➕ Print (no newline)          ││  │ # include <stdio.h>              │ │
│  │ ➕ Print (with newline)         ││  │                                  │ │
│  │ ➕ Comment                      ││  │ int main(){                      │ │
│  └────────────────────────────────┘│  │     printf("Hello world!");      │ │
│                                     │  │ }                                │ │
│  ┌────────────────────────────────┐│  └──────────────────────────────────┘ │
│  │ 🎯 Main Function                ││                                        │
│  │ (Program Entry Point)           ││     📋 Quick Start                     │
│  │                                 ││  • Click blocks in the palette         │
│  │  ┌──────────────────────────┐  ││  • Edit text in the input fields       │
│  │  │ 🖨️ Print Statement       │  ││  • Code updates automatically          │
│  │  │ ✖ Delete                 │  ││  • Click "Download hello.c" to save    │
│  │  ├──────────────────────────┤  ││  • Use the generated file              │
│  │  │ Text to print:           │  ││                                        │
│  │  │ [Hello world!]           │  ││  ┌──────────┐  ┌──────────────────┐  │
│  │  └──────────────────────────┘  ││  │ 🔄 Reset │  │ 💾 Download     │  │
│  │                                 ││  │          │  │    hello.c      │  │
│  └────────────────────────────────┘│  └──────────┘  └──────────────────┘  │
│                                     │                                        │
└─────────────────────────────────────┴────────────────────────────────────────┘
```

## Block Palette (Top Left)

The Block Palette contains buttons to add different types of blocks:

- **➕ Print (no newline)**: Adds a printf statement without \n
- **➕ Print (with newline)**: Adds a printf statement with \n at the end
- **➕ Comment**: Adds a C-style // comment

Click any button to add that block type to your program.

## Workspace (Left Side)

The main workspace shows your program structure:

### Main Function Block (Red border)
- This is the entry point of your C program
- Cannot be deleted (every C program needs a main function)
- All other blocks go inside this block

### Print Statement Blocks (Blue border)
- Each block has a text input field
- Edit the text to change what gets printed
- Click "✖ Delete" to remove the block
- Blocks are executed in order from top to bottom

### Comment Blocks
- Add documentation to your code
- Helpful for explaining what your program does
- Don't affect the program's behavior

## Generated Code (Right Side, Top)

Shows the C code that will be generated from your blocks:
- Updates automatically as you edit
- Shows exact code that will be downloaded
- Includes proper C syntax (headers, main function, etc.)

## Quick Start Guide (Right Side, Middle)

Helpful tips and instructions for using the editor.

## Action Buttons (Right Side, Bottom)

- **🔄 Reset**: Clears all blocks and resets to default Hello World
- **💾 Download hello.c**: Downloads the generated C code as a file

## Workflow

1. Start with the default "Hello world!" program
2. Click buttons in the palette to add more blocks
3. Edit text in the input fields
4. Watch the code update in real-time
5. Click "Download hello.c" when satisfied
6. Use the downloaded file in your C project

## Color Coding

- **Red blocks**: Main function (program structure)
- **Blue blocks**: Print statements (output)
- **White blocks**: Comments (documentation)

## Tips

- Blocks execute from top to bottom
- You can have as many blocks as you want
- Delete blocks you don't need
- Reset anytime to start fresh
- No internet connection needed after page loads
