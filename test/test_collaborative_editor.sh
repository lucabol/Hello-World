#!/bin/bash
# Integration test for collaborative editor
# Tests the server startup and basic API functionality

set -e
set -u
set -o pipefail

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

print_success() {
    printf '%b' "${GREEN}✓ $1${NC}\n"
}

print_error() {
    printf '%b' "${RED}✗ $1${NC}\n"
}

print_info() {
    printf '%b' "${YELLOW}ℹ $1${NC}\n"
}

# Change to repository root
cd "$(dirname "$0")/.."

print_info "Testing collaborative editor..."

# Check if Node.js is available
if ! command -v node &> /dev/null; then
    print_error "Node.js is not installed"
    exit 1
fi
print_success "Node.js is available"

# Check if npm packages are installed
if [ ! -d "collaborative-editor/node_modules" ]; then
    print_info "Installing npm packages..."
    cd collaborative-editor
    npm install --silent
    cd ..
    print_success "npm packages installed"
else
    print_success "npm packages already installed"
fi

# Start server in background
print_info "Starting collaborative editor server..."
cd collaborative-editor
node server.js > /tmp/collab-server.log 2>&1 &
SERVER_PID=$!
cd ..

# Function to cleanup server on exit
cleanup() {
    if [ -n "${SERVER_PID:-}" ]; then
        kill $SERVER_PID 2>/dev/null || true
        wait $SERVER_PID 2>/dev/null || true
    fi
}
trap cleanup EXIT

# Wait for server to start
sleep 2

# Check if server is running
if ! ps -p $SERVER_PID > /dev/null; then
    print_error "Server failed to start"
    cat /tmp/collab-server.log
    exit 1
fi
print_success "Server started successfully (PID: $SERVER_PID)"

# Test HTTP endpoint
print_info "Testing HTTP endpoint..."
if curl -s http://localhost:3000/ | grep -q "Collaborative Editor"; then
    print_success "HTTP endpoint is accessible"
else
    print_error "HTTP endpoint test failed"
    exit 1
fi

# Test API endpoint
print_info "Testing API endpoint..."
if curl -s http://localhost:3000/api/content | grep -q "content"; then
    print_success "API endpoint is working"
else
    print_error "API endpoint test failed"
    exit 1
fi

# Verify API returns valid JSON
print_info "Verifying API response..."
API_RESPONSE=$(curl -s http://localhost:3000/api/content)
if echo "$API_RESPONSE" | python3 -c "import sys, json; json.load(sys.stdin)" 2>/dev/null; then
    print_success "API returns valid JSON"
else
    print_error "API response is not valid JSON"
    exit 1
fi

# Check that content includes hello.c code
if echo "$API_RESPONSE" | grep -q "include.*stdio"; then
    print_success "API returns hello.c content"
else
    print_error "API content doesn't match hello.c"
    exit 1
fi

print_success "All tests passed!"
echo ""
print_info "Server is running at http://localhost:3000"
print_info "Press Ctrl+C to stop the server"
echo ""

# Keep server running for manual testing
wait $SERVER_PID
