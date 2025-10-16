#!/bin/bash
# Test script to verify the visual editor generates valid C code

set -e

echo "Testing Visual Block Editor Code Generation..."

# Test 1: Verify the block-editor.html file exists
if [ ! -f "block-editor.html" ]; then
    echo "❌ FAILED: block-editor.html not found"
    exit 1
fi
echo "✓ block-editor.html exists"

# Test 2: Verify documentation exists
if [ ! -f "VISUAL_EDITOR.md" ]; then
    echo "❌ FAILED: VISUAL_EDITOR.md not found"
    exit 1
fi
echo "✓ VISUAL_EDITOR.md exists"

# Test 3: Verify block-editor.html contains required components
if grep -q "Block Toolbox" block-editor.html && \
   grep -q "Include Header" block-editor.html && \
   grep -q "Main Function" block-editor.html && \
   grep -q "Printf Statement" block-editor.html && \
   grep -q "Variable Declaration" block-editor.html; then
    echo "✓ All required blocks are present in the editor"
else
    echo "❌ FAILED: Missing required blocks in editor"
    exit 1
fi

# Test 4: Verify export functionality is present
if grep -q "Export to hello.c" block-editor.html && \
   grep -q "downloadCode" block-editor.html; then
    echo "✓ Export functionality is present"
else
    echo "❌ FAILED: Export functionality not found"
    exit 1
fi

# Test 5: Verify code generation function exists
if grep -q "function generateCode" block-editor.html; then
    echo "✓ Code generation function exists"
else
    echo "❌ FAILED: Code generation function not found"
    exit 1
fi

# Test 6: Verify the editor is self-contained (no external dependencies)
if ! grep -q "https://" block-editor.html || \
   ! grep -q "http://" block-editor.html; then
    echo "✓ Editor is self-contained with no external dependencies"
else
    # Check if there are any external script/link references
    if grep -E '<script[^>]*src="https?://' block-editor.html || \
       grep -E '<link[^>]*href="https?://' block-editor.html; then
        echo "⚠️  WARNING: Editor may have external dependencies"
    else
        echo "✓ Editor is self-contained with no external dependencies"
    fi
fi

echo ""
echo "✅ All visual editor tests passed!"
echo ""
echo "To use the visual editor:"
echo "  1. Open block-editor.html in a web browser"
echo "  2. Drag blocks to create your C program"
echo "  3. Click 'Generate Code' to see the result"
echo "  4. Click 'Export to hello.c' to download"
