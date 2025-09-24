#!/bin/bash
# Collaborative Editor Launcher
# Starts a web server and opens the collaborative editing interface

set -e

PORT=${1:-8081}
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "🚀 Starting Collaborative Editor for hello.c..."
echo "📁 Serving from: ${SCRIPT_DIR}"
echo "🌐 URL: http://localhost:${PORT}/collab_editor.html"
echo ""
echo "👥 Features:"
echo "   - Real-time collaborative editing simulation"
echo "   - Multi-user cursor tracking"
echo "   - Live build and run integration"
echo "   - Automatic conflict resolution"
echo "   - Activity logging"
echo ""
echo "💡 Press Ctrl+C to stop the server"
echo "───────────────────────────────────────"

# Start Python web server in the background
cd "${SCRIPT_DIR}"
python3 -m http.server "${PORT}"