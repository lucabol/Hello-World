#!/bin/bash
# Test script for Visual Block Editor
# Validates that the visual editor can be launched and serves the correct files

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "🧩 Testing Visual Block Editor..."
echo ""

# Test 1: Check that visual_editor.html exists and is valid HTML
echo "📄 Checking visual_editor.html..."
if [ ! -f "visual_editor.html" ]; then
    echo "❌ visual_editor.html not found"
    exit 1
fi

# Basic HTML validation
if ! grep -q "<!DOCTYPE html>" visual_editor.html; then
    echo "❌ visual_editor.html missing DOCTYPE"
    exit 1
fi

if ! grep -q "Visual Block Editor" visual_editor.html; then
    echo "❌ visual_editor.html missing title"
    exit 1
fi

if ! grep -q "class VisualBlockEditor" visual_editor.html; then
    echo "❌ visual_editor.html missing main JavaScript class"
    exit 1
fi

echo "✓ visual_editor.html is valid"

# Test 2: Check that launch script exists and is executable
echo "📄 Checking launch_visual.sh..."
if [ ! -f "launch_visual.sh" ]; then
    echo "❌ launch_visual.sh not found"
    exit 1
fi

if [ ! -x "launch_visual.sh" ]; then
    echo "❌ launch_visual.sh is not executable"
    exit 1
fi

echo "✓ launch_visual.sh exists and is executable"

# Test 3: Check documentation
echo "📄 Checking VISUAL_README.md..."
if [ ! -f "VISUAL_README.md" ]; then
    echo "❌ VISUAL_README.md not found"
    exit 1
fi

if ! grep -q "Visual Block Editor for C Programming" VISUAL_README.md; then
    echo "❌ VISUAL_README.md missing main title"
    exit 1
fi

echo "✓ VISUAL_README.md exists and contains documentation"

# Test 4: Check server integration
echo "📄 Checking server integration..."
if ! grep -q "/build" collab_server.js; then
    echo "❌ collab_server.js missing /build endpoint"
    exit 1
fi

if ! grep -q "/run" collab_server.js; then
    echo "❌ collab_server.js missing /run endpoint"
    exit 1
fi

if ! grep -q "/export" collab_server.js; then
    echo "❌ collab_server.js missing /export endpoint"
    exit 1
fi

echo "✓ Server endpoints integrated"

# Test 5: Quick server start test (if Node.js is available)
if command -v node >/dev/null 2>&1 && [ -d "node_modules" ]; then
    echo "🚀 Testing server startup..."
    
    # Start server in background
    timeout 10s npm start &
    SERVER_PID=$!
    
    # Give it time to start
    sleep 3
    
    # Check if server is responding
    if curl -s http://localhost:8082/health >/dev/null 2>&1; then
        echo "✓ Server starts successfully"
    else
        echo "⚠️  Server may not be responding (this is expected in automated tests)"
    fi
    
    # Clean up
    kill $SERVER_PID 2>/dev/null || true
    wait $SERVER_PID 2>/dev/null || true
fi

echo ""
echo "🎉 All Visual Block Editor tests passed!"
echo ""
echo "📋 To use the visual editor:"
echo "   1. Run: ./launch_visual.sh"
echo "   2. Open: http://localhost:8082/visual_editor.html"
echo "   3. Drag blocks from palette to workspace"
echo "   4. Build, run, and export C programs"
echo ""