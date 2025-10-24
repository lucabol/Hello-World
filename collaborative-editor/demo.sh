#!/bin/bash
# Demo script for collaborative editor
# Starts the server and provides instructions for testing

set -e

# Colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_header() {
    echo ""
    printf '%b' "${BLUE}========================================${NC}\n"
    printf '%b' "${BLUE}$1${NC}\n"
    printf '%b' "${BLUE}========================================${NC}\n"
    echo ""
}

print_success() {
    printf '%b' "${GREEN}✓ $1${NC}\n"
}

print_info() {
    printf '%b' "${YELLOW}ℹ $1${NC}\n"
}

# Change to collaborative-editor directory
cd "$(dirname "$0")/../collaborative-editor"

print_header "Real-Time Collaborative Editor Demo"

# Check dependencies
if [ ! -d "node_modules" ]; then
    print_info "Installing dependencies..."
    npm install
    echo ""
fi

print_success "Dependencies installed"
echo ""

print_info "Starting collaborative editor server..."
echo ""

# Start the server
node server.js &
SERVER_PID=$!

# Cleanup function
cleanup() {
    echo ""
    print_info "Shutting down server..."
    kill $SERVER_PID 2>/dev/null || true
    wait $SERVER_PID 2>/dev/null || true
    print_success "Server stopped"
}
trap cleanup EXIT INT TERM

# Wait for server to start
sleep 2

print_header "Collaborative Editor is Ready!"

echo "The collaborative editor is now running. To test it:"
echo ""
echo "1. Open your web browser and go to:"
printf '%b' "   ${GREEN}http://localhost:3000${NC}\n"
echo ""
echo "2. Open the same URL in multiple browser windows/tabs"
echo ""
echo "3. Start editing hello.c - changes will sync in real-time!"
echo ""
echo "4. Features to try:"
echo "   • Type in one window and watch it appear in others"
echo "   • Change your username to identify yourself"
echo "   • See who's connected in the sidebar"
echo "   • All changes are automatically saved"
echo ""
print_info "Press Ctrl+C to stop the server"
echo ""

# Wait for interrupt
wait $SERVER_PID
