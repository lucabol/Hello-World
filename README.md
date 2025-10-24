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
make test          # Run all 75 tests (requires GCC for 1 compilation test)
make test-quick    # Run 74 tests (skips GCC compilation test)
make help          # Show all test targets
```

**Requirements:**
- Node.js 18+ (required for all tests)
- GCC (optional, only for compilation smoke test)

**CI Integration:**
- Fast PR validation: Use `make test-quick` (no GCC needed)
- Full validation: Use `make test` (requires GCC)
- See `.github/workflows/test-visual-editor.yml` for workflow configuration

See [TESTING.md](TESTING.md) for detailed testing documentation, including:
- Test suite overview and organization
- GCC behavior and requirements
- HTML validation strategy and trade-offs
- CI integration guidelines
- Migration path to parser-based testing

See [ACCESSIBILITY.md](ACCESSIBILITY.md) for accessibility testing:
- WCAG 2.1 compliance checklist
- ARIA attribute mapping
- Keyboard navigation testing procedures
- Screen reader testing guide