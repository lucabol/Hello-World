#!/bin/bash
# Launch script for Visual Block Editor
# Provides both standalone and server modes

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "🧩 Starting Visual Block Editor for C Programming..."
echo ""

# Check if Node.js is available for full server mode
if command -v node >/dev/null 2>&1 && [ -f "package.json" ]; then
    echo "🚀 Starting with full server capabilities..."
    
    # Install dependencies if needed
    if [ ! -d "node_modules" ]; then
        echo "📦 Installing Node.js dependencies..."
        npm install
    fi
    
    # Start the collaborative server (includes visual editor endpoints)
    echo "🌐 Server will be available at:"
    echo "   Visual Block Editor: http://localhost:8082/visual_editor.html"
    echo "   Collaborative Editor: http://localhost:8082/collab_editor.html"
    echo ""
    echo "   Ctrl+C to stop the server"
    echo ""
    
    npm start
    
else
    echo "🔧 Node.js not available - starting in simple mode..."
    echo ""
    
    # Start a simple HTTP server for the visual editor
    PORT=8081
    
    if command -v python3 >/dev/null 2>&1; then
        echo "🌐 Visual Block Editor available at: http://localhost:$PORT/visual_editor.html"
        echo "   (Note: Build/Run/Export features require the full server mode)"
        echo ""
        echo "   Ctrl+C to stop"
        echo ""
        python3 -m http.server $PORT
    elif command -v python >/dev/null 2>&1; then
        echo "🌐 Visual Block Editor available at: http://localhost:$PORT/visual_editor.html"
        echo "   (Note: Build/Run/Export features require the full server mode)"
        echo ""
        echo "   Ctrl+C to stop"
        echo ""
        python -m SimpleHTTPServer $PORT
    else
        echo "❌ Neither Node.js nor Python available for web server"
        echo "📋 To use the visual editor:"
        echo "   1. Install Node.js and run: npm install && npm start"
        echo "   2. Open: http://localhost:8082/visual_editor.html"
        echo "   Or:"
        echo "   1. Start any web server in this directory"
        echo "   2. Open: visual_editor.html"
        exit 1
    fi
fi