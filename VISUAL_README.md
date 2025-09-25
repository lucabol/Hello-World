# Visual Block Editor for C Programming

This implementation adds a web-based visual block editor to the Hello World C project, enabling users to create and modify C code using intuitive drag-and-drop blocks, and export the result to hello.c.

## Features

### 🧩 Visual Programming Interface
- **Drag & Drop Blocks**: Create C programs by dragging blocks from a palette
- **Real-time Code Generation**: See C code generated as you build your program
- **Interactive Parameters**: Edit block parameters (variable names, strings, values) inline
- **Visual Block Organization**: Blocks are visually connected and organized

### 🔧 Integrated Development Environment
- **Live Compilation**: Build generated C code directly from the editor
- **Program Execution**: Run compiled programs and see output in real-time
- **Export Functionality**: Export generated C code to hello.c or download as file
- **Build Status**: Real-time feedback on compilation and execution

### 📝 Supported C Constructs
- **Headers**: `#include <stdio.h>`, `#include <stdlib.h>`
- **Variables**: `int variable = value`, `char text[] = "string"`
- **Output**: `printf("format", args)`, `puts("text")`
- **Control Flow**: `if (condition) { }`, `return value`
- **Functions**: `int main(void) { }`

## Quick Start

### Option 1: Full Server Mode (Recommended)
```bash
# Launch the visual editor with full capabilities
./launch_visual.sh

# Opens at: http://localhost:8082/visual_editor.html
```

### Option 2: Standalone Mode
```bash
# Start with Node.js dependencies installed
npm install
npm start

# Open: http://localhost:8082/visual_editor.html
```

### Option 3: Simple Web Server
```bash
# Basic mode (build/run/export features limited)
python3 -m http.server 8081

# Open: http://localhost:8081/visual_editor.html
```

## Usage

### Creating a Program
1. **Drag blocks** from the palette on the left to the workspace
2. **Configure parameters** by editing the input fields in blocks
3. **View generated code** in the right panel in real-time
4. **Arrange blocks** in logical order (includes → main → statements → return)

### Building and Running
1. **Generate Code**: Click "🔄 Generate Code" to refresh the C code
2. **Build**: Click "🔨 Build" to compile the generated code
3. **Run**: Click "▶️ Run" to execute the program
4. **View Output**: See compilation and execution results in the output panel

### Exporting Code
1. **Export to hello.c**: Click "💾 Export to hello.c" to save to the project file
2. **Download**: If server export fails, code is automatically downloaded as a file

### Workspace Management
- **Clear All**: Click "🗑️ Clear" to remove all blocks
- **Delete Block**: Hover over a block and click the "×" button
- **Select Block**: Click on a block to select it (highlighted in blue)

## Block Types

### Header Blocks
- **`#include <stdio.h>`**: Standard I/O library
- **`#include <stdlib.h>`**: Standard library

### Variable Blocks
- **`int variable = 0`**: Integer variable with editable name and value
- **`char text[] = "text"`**: Character array with editable name and value

### Output Blocks
- **`printf("Hello world!")`**: Formatted output with editable format string
- **`puts("Hello world!")`**: Simple string output with editable text

### Control Flow Blocks
- **`if (condition) { }`**: Conditional statement with editable condition
- **`return 0`**: Return statement with editable return value

### Function Blocks
- **`int main(void) { }`**: Main function container

## Architecture

### Client-Side (visual_editor.html)
- **HTML5/CSS3**: Modern responsive interface
- **Vanilla JavaScript**: No external dependencies for core functionality
- **Drag & Drop API**: Native HTML5 drag and drop
- **Real-time Updates**: Immediate feedback on all changes

### Server Integration (collab_server.js)
- **Express.js Endpoints**: `/build`, `/run`, `/export` for full functionality
- **Security**: Sandboxed execution with timeouts and resource limits
- **Compatibility**: Extends existing collaborative editing server

### File Structure
```
visual_editor.html          # Main visual block editor interface
launch_visual.sh           # Quick launcher script
collab_server.js           # Extended server with visual editor endpoints
VISUAL_README.md           # This documentation
```

## Integration with Existing System

### Backward Compatibility
- **Preserves hello.c**: Original file structure maintained
- **Build System**: Compatible with existing Makefile and validation
- **Output Format**: Generated code follows project conventions
- **Validation**: Can generate code that passes all existing tests

### Code Generation
The visual editor generates clean, properly formatted C code:
```c
#include <stdio.h>

int main(void) {
    puts("Hello world!");
    return 0;
}
```

## Security & Safety

### Sandboxed Execution
- **Timeout Limits**: Build and run operations have strict time limits
- **Resource Limits**: Memory and output size restrictions
- **Restricted Environment**: Limited PATH and environment variables
- **No Shell Access**: Direct executable calls without shell interpretation

### Input Validation
- **Parameter Validation**: All block parameters are sanitized
- **Code Injection Prevention**: No string interpolation in system calls
- **File System Limits**: Restricted file access and temporary files

## Troubleshooting

### Connection Issues
- **Build/Run Failed**: Ensure the server is running with `npm start`
- **Export Failed**: Server may not be accessible, code downloads as file instead
- **Drag & Drop**: Ensure browser supports HTML5 drag and drop API

### Code Generation
- **Incomplete Program**: Add a main function block for complete programs
- **Compilation Errors**: Check that all required includes are present
- **Missing Returns**: Add return statement to main function

### Browser Compatibility
- **Modern Browsers**: Chrome, Firefox, Safari, Edge (recent versions)
- **Mobile Devices**: Responsive design works on tablets and phones
- **JavaScript Required**: Core functionality requires JavaScript enabled

## Validation

The visual block editor maintains compatibility with existing validation:

```bash
# Create a program in visual editor
# Export to hello.c
# Test with existing validation
make test
```

## Development

### Adding New Block Types
1. **Update Palette**: Add new block in visual_editor.html palette section
2. **Block Rendering**: Add case in `getBlockHTML()` method
3. **Code Generation**: Add case in `generateCode()` method
4. **Default Parameters**: Add case in `getDefaultParams()` method

### Customizing Appearance
- **CSS Styling**: All styles embedded in visual_editor.html `<style>` section
- **VS Code Theme**: Dark theme inspired by popular code editor
- **Color Coding**: Different colors for different block types
- **Responsive Design**: Works across desktop and mobile devices

### Testing Features
```bash
# Start server
npm start

# Open visual editor
# http://localhost:8082/visual_editor.html

# Test drag & drop, build, run, export functionality
```

## Future Enhancements

Potential improvements for the visual block editor:

- **More Block Types**: Support for loops, functions, arrays, pointers
- **Code Import**: Load existing C code and convert to visual blocks
- **Syntax Highlighting**: Enhanced code display with syntax coloring
- **Block Connections**: Visual connectors showing program flow
- **Collaborative Editing**: Multi-user visual programming
- **Templates**: Pre-built program templates and examples
- **Advanced C Features**: Structures, unions, preprocessor directives
- **Debugging Support**: Visual debugging with block highlighting
- **Code Optimization**: Suggestions for code improvements
- **Export Options**: Multiple output formats and coding standards