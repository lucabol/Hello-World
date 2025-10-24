# Hello World
Test repo for JediMaster

## Visual C Code Editor

Try the [Visual Block-Based C Code Editor](tools/editor/editor.html) to create C programs using drag-and-drop blocks!

Open `tools/editor/editor.html` in your browser to:
- Build C programs visually with drag-and-drop blocks
- See live code preview as you build
- Export ready-to-compile `hello.c` files

See [tools/editor/EDITOR_GUIDE.md](tools/editor/EDITOR_GUIDE.md) for detailed usage instructions.

## Testing

Run the complete test suite:
```bash
make test          # Run all 75 tests
make test-quick    # Skip GCC compilation test
make help          # Show all test targets
```

See [TESTING.md](TESTING.md) for detailed testing documentation, including:
- Test suite overview and organization
- HTML validation strategy and trade-offs
- CI integration guidelines
- Migration path to parser-based testing