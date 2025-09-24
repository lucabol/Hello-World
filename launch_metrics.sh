#!/bin/bash
# Code Metrics Spreadsheet Launcher
# Starts a web server and opens the code metrics interface

set -e

PORT=${1:-8080}
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "🚀 Starting Code Metrics Spreadsheet..."
echo "📁 Serving from: ${SCRIPT_DIR}"
echo "🌐 URL: http://localhost:${PORT}/code_metrics.html"
echo ""
echo "💡 Press Ctrl+C to stop the server"
echo "───────────────────────────────────────"

# Start Python web server in the background
cd "${SCRIPT_DIR}"
python3 -m http.server "${PORT}"