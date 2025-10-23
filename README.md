# Hello World
Test repo for JediMaster

## Features

### C Program
A simple "Ciao, Mondo!" C program (`hello.c`) that demonstrates basic output.

### Visual Block Editor
A web-based visual editor (`editor.html`) that allows you to create C programs using drag-and-drop blocks:
- 🎨 Intuitive drag-and-drop interface
- ⌨️ Full keyboard accessibility
- 📱 Touch device support
- 🔒 Security hardened with XSS protection
- 📥 Export generated code as C files

[See EDITOR_GUIDE.md for detailed documentation](EDITOR_GUIDE.md)

## Quick Start

### Compile and Run
```bash
make hello
./hello
```

### Use the Visual Editor
1. Open `editor.html` in any modern web browser
2. Drag blocks to build your program
3. Click "Export" to download as `hello.c`

### Run Tests
```bash
make test
```

## Browser Support
- Chrome/Edge 90+
- Firefox 88+
- Safari 14+

## Files
- `hello.c` - Main C program source
- `hello.h` - Header file with function declarations
- `editor.html` - Visual block editor
- `EDITOR_GUIDE.md` - Comprehensive editor documentation
- `Makefile` - Build and test automation
- `test/` - Test suite

## Testing
The project includes comprehensive testing:
- C unit tests
- Editor security tests (XSS, innerHTML scanning)
- C code generation tests
- Integration tests

All tests run automatically in CI/CD.
