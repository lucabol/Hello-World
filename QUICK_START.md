# Visual C Code Editor - Quick Reference

## Opening the Editor

Simply open `editor.html` in any modern web browser. No installation or server required!

```bash
# On Linux/Mac
open editor.html

# On Windows
start editor.html

# Or just double-click the file
```

## Creating Your First Program

### Method 1: Use the Example (Fastest)
1. Click "Load Example" button
2. The classic Hello World program will appear
3. Click "Export to hello.c" to download

### Method 2: Build from Scratch
1. Drag "#include <header>" block to workspace
2. Edit the header to "stdio.h"
3. Drag "printf("text")" block to workspace
4. Edit the text to your message
5. Click "Export to hello.c"

## Block Types Quick Guide

| Block Type | Purpose | Example |
|------------|---------|---------|
| **#include <header>** | Include header files | `#include <stdio.h>` |
| **printf("text")** | Print text | `printf("Hello!");` |
| **int variable = 0** | Declare variable | `int count = 42;` |

## Tips & Tricks

### Editing Text
- Click directly on any text field in a block to edit it
- Text changes update the code in real-time

### Removing Blocks
- Click the "×" button on any block to remove it

### Code Generation
- Code updates automatically as you add/edit blocks
- Click "Generate Code" to manually refresh

### Exporting
- "Export to hello.c" downloads the file to your Downloads folder
- The file is ready to compile with: `gcc -o hello hello.c`

## Common Use Cases

### 1. Simple Hello World
```
[#include <stdio.h>]
[printf("Hello world!")]
```

### 2. Program with Variables
```
[#include <stdio.h>]
[int count = 42]
[printf("Hello world!")]
```

### 3. Different Headers
```
[#include <stdio.h>]
[#include <stdlib.h>]
[printf("Hello!")]
```

## Compiling Your Code

After exporting, compile with:

```bash
# Basic compilation
gcc -o hello hello.c

# With warnings (recommended)
gcc -Wall -Wextra -o hello hello.c

# Run the program
./hello
```

## Troubleshooting

**Q: The editor doesn't load**
- Ensure you're using a modern browser (Chrome, Firefox, Safari, Edge)
- Try clearing your browser cache

**Q: Export doesn't download the file**
- Check your browser's download permissions
- Look in your Downloads folder

**Q: Blocks won't drag**
- Make sure you're dragging the colored blocks (not the workspace)
- Try refreshing the page

**Q: Generated code doesn't compile**
- Verify all blocks have valid text
- Check for typos in variable names or headers

## Advanced Features

### Customizing Output
1. Change "Hello world!" to any message
2. Multiple printf blocks work sequentially
3. Variables can be declared before use

### Code Structure
The editor automatically:
- Wraps statements in `int main(){ }`
- Adds proper indentation
- Includes necessary headers at the top

## Need More Help?

See [EDITOR_README.md](EDITOR_README.md) for detailed documentation.
