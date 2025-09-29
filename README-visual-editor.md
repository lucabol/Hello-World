# Visual C Code Block Editor

This is a web-based visual editor that allows users to create C code using drag-and-drop blocks, and then export the result to `hello.c`.

## ⚠️ Security Notice

**This editor is designed for local development only.** The server binds to `localhost` and should not be exposed to external networks. Input validation is implemented but this tool is intended for educational and development purposes.

## Features

- **Drag and Drop Interface**: Drag blocks from the sidebar to the workspace
- **Visual Block Programming**: Create C code without typing
- **Real-time Code Generation**: See the generated C code as you build
- **Export Functionality**: Save generated code directly to `hello.c`
- **Input Validation**: Sanitizes user input to prevent code injection
- **Accessibility Support**: Keyboard navigation and screen reader friendly
- **Multiple Block Types**: Support for includes, functions, statements, and variables

## Block Types

### Program Structure
- **#include blocks**: Add header files (default: `#include <stdio.h>`)
  - Validates header names against safe patterns
  - Only allows alphanumeric characters, dots, slashes, and underscores
- **main() function**: Creates the main function structure

### Statements
- **printf statement**: Print text to console
  - Automatically escapes quotes, backslashes, and special characters
  - Limits string length to prevent overly long output
- **return statement**: Return values from functions
  - Validates return expressions for basic safety

### Variables
- **int variable**: Declare integer variables
  - Validates variable names as proper C identifiers
- **char variable**: Declare character variables
  - Ensures single character literals with proper escaping

## How to Use

### Starting the Editor

1. Start the server:
   ```bash
   python3 server.py
   ```

2. Open your browser and navigate to:
   ```
   http://localhost:8081/block-editor.html
   ```

### Creating Code

#### Using Mouse/Touch:
1. **Drag blocks** from the sidebar to the workspace
2. **Edit values** in the input fields within blocks
3. **Click "Generate Code"** to see the C code
4. **Click "Export to hello.c"** to save the code

#### Using Keyboard (Accessibility):
1. **Tab** to navigate between blocks in the sidebar
2. **Enter or Space** to add a block to the workspace
3. **Tab** to move between workspace blocks and input fields
4. **Delete/Backspace** to remove focused blocks
5. **Click or Enter** on Generate Code and Export buttons

### Example Workflow

1. Drag an `#include <stdio.h>` block to the workspace
2. Drag a `main() function` block
3. Drag a `printf statement` block
4. Edit the printf message if needed (e.g., "Hello, Visual Editor!")
5. Generate the code
6. Export to hello.c

## Generated Code Format

The editor generates properly formatted C code that matches the original hello.c format:

```c
#include <stdio.h>

int main(){
    printf("Hello world!");
}
```

### Input Sanitization Examples

| Input Type | Example Input | Sanitized Output | Reason |
|------------|---------------|------------------|---------|
| Printf message | `Say "Hello"` | `Say \"Hello\"` | Escapes quotes |
| Header name | `../../../etc/passwd` | `stdio.h` | Invalid path, fallback |
| Variable name | `123invalid` | `variable` | Invalid identifier, fallback |
| String with newlines | `Line1\nLine2` | `Line1\\nLine2` | Escapes newlines |

## Security Features

### Input Validation
- **Header names**: Only allow safe header file patterns
- **C identifiers**: Validate variable/function names against C standards
- **String literals**: Escape special characters and limit length
- **Code structure**: Verify basic C syntax before export

### Server Security
- **Localhost binding**: Server only accepts connections from localhost
- **Path validation**: Only allows writing to `hello.c` in current directory
- **Request size limits**: Prevents oversized requests
- **File backup**: Creates backup before overwriting hello.c

### Error Handling
- **HTTP status codes**: Proper 200/201/400/500 responses
- **JSON error messages**: Clear error descriptions
- **Graceful degradation**: Fallback values for invalid input

## Testing

### Running Tests

```bash
# Run comprehensive test suite
python3 test_visual_editor.py

# Test just the server validation
python3 -m unittest test_visual_editor.TestCodeValidator

# Test code compilation
python3 -m unittest test_visual_editor.TestCodeGeneration
```

### Manual Testing Scenarios

1. **Basic functionality**:
   ```bash
   python3 server.py
   # Open http://localhost:8081/block-editor.html
   # Create blocks, generate code, export
   gcc -o hello hello.c && ./hello
   ```

2. **Input sanitization**:
   - Try entering quotes in printf messages
   - Test invalid variable names
   - Attempt path traversal in include headers

3. **Accessibility**:
   - Navigate using only keyboard
   - Test with screen reader if available

## Browser Compatibility

Tested and verified on:
- ✅ **Chrome 90+**: Full functionality including drag-and-drop
- ✅ **Firefox 88+**: Complete support
- ✅ **Safari 14+**: Works with minor styling differences
- ✅ **Edge 90+**: Full compatibility

**Requirements**: Modern browser with HTML5 drag-and-drop API support

## Technical Details

- **Frontend**: HTML5, CSS3, JavaScript (vanilla, no dependencies)
- **Backend**: Python 3.6+ HTTP server with JSON API
- **File I/O**: Direct file writing to hello.c with backup
- **Validation**: Client-side and server-side input sanitization
- **Security**: Localhost-only binding, path validation, input limits

## Files

- `block-editor.html` - The main visual editor interface (self-contained)
- `server.py` - Python server with export functionality and validation
- `test_visual_editor.py` - Comprehensive test suite
- `README-visual-editor.md` - This documentation

## Development

### Adding New Block Types

To add new block types, extend the `blockTemplates` object in the JavaScript:

```javascript
blockTemplates.newType = {
    html: 'Block HTML with <input> fields',
    generateCode: (block) => {
        // Extract and validate inputs
        // Return generated C code
    }
};
```

### Security Considerations

When modifying the code:
1. Always validate user input on both client and server
2. Use the `InputValidator` utilities for consistency
3. Test with malicious inputs (quotes, backslashes, path traversal)
4. Ensure generated C code compiles successfully

### Server API

**POST /export**
```json
Request: {"code": "C source code"}
Response: {
  "success": true,
  "message": "Code successfully exported to hello.c",
  "file_path": "/path/to/hello.c",
  "backup_created": true
}
```

Error responses include appropriate HTTP status codes (400, 500) and descriptive messages.

## License

This code follows the repository's existing license. See the main README.md for license details.