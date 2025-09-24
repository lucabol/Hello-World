#!/bin/bash
# Launch Visual Editor for Manual Testing
# Opens the visual-editor.html in a local browser

echo "Starting Visual Editor..."
echo "========================"

if command -v python3 >/dev/null 2>&1; then
    echo "Starting local web server on port 8000..."
    echo "Open http://localhost:8000/visual-editor.html in your browser"
    echo "Press Ctrl+C to stop the server"
    echo
    python3 -m http.server 8000
elif command -v python >/dev/null 2>&1; then
    echo "Starting local web server on port 8000..."
    echo "Open http://localhost:8000/visual-editor.html in your browser"
    echo "Press Ctrl+C to stop the server"
    echo
    python -m SimpleHTTPServer 8000
else
    echo "Python not found. You can open visual-editor.html directly in your browser."
    echo "File location: $(pwd)/visual-editor.html"
fi