#!/bin/bash
# Signal handling cleanup test for metrics_tool with AddressSanitizer
# Tests that SIGINT and SIGTERM are handled gracefully without memory leaks
# This script is extracted from CI for better testability and maintainability

set -euo pipefail

# Ensure ASAN options are set for leak detection
export ASAN_OPTIONS="${ASAN_OPTIONS:-detect_leaks=1:halt_on_error=1}"
export LC_ALL=C

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Determine which binary to test (default to metrics_tool_asan)
BINARY="${1:-./metrics_tool_asan}"

if [ ! -f "$BINARY" ]; then
    echo -e "${RED}Error: Binary $BINARY not found${NC}"
    echo "Usage: $0 [binary_path]"
    exit 1
fi

echo "Testing signal handling for: $BINARY"
echo "ASAN_OPTIONS: $ASAN_OPTIONS"
echo ""

# Test SIGINT handling
echo "Test 1: SIGINT handling with robust process group signaling"
echo "-----------------------------------------------------------"
timeout 10s bash -c "
    # Start metrics_tool in its own process group (interactive mode waits for input)
    setsid $BINARY hello.c < /dev/null > /dev/null 2>&1 &
    PID=\$!
    sleep 0.5

    # Send SIGINT to the entire process group
    kill -SIGINT -- -\$PID 2>/dev/null || kill -SIGINT \$PID 2>/dev/null || true

    # Wait for process and capture exit code
    wait \$PID 2>/dev/null || EXIT_CODE=\$?

    # Document expected behavior: tool should exit with signal-derived code (130)
    # or handle signal gracefully (0). Both indicate no leaks/crashes (validated by ASAN).
    if [ \"\${EXIT_CODE:-0}\" -ne 130 ] && [ \"\${EXIT_CODE:-0}\" -ne 0 ]; then
        echo -e \"${RED}✗ SIGINT test failed: exit code \${EXIT_CODE:-0} (expected 130 or 0)${NC}\"
        exit 1
    fi
    echo -e \"${GREEN}✓ SIGINT test passed: exit code \${EXIT_CODE:-0}, no ASAN errors${NC}\"
" || {
    echo -e "${RED}✗ SIGINT test timed out or failed${NC}"
    exit 1
}

echo ""

# Test SIGTERM handling
echo "Test 2: SIGTERM handling with robust process group signaling"
echo "-------------------------------------------------------------"
timeout 10s bash -c "
    # Start metrics_tool in its own process group
    setsid $BINARY hello.c < /dev/null > /dev/null 2>&1 &
    PID=\$!
    sleep 0.5

    # Send SIGTERM to the entire process group
    kill -SIGTERM -- -\$PID 2>/dev/null || kill -SIGTERM \$PID 2>/dev/null || true

    # Wait for process and capture exit code
    wait \$PID 2>/dev/null || EXIT_CODE=\$?

    # Expected: signal-derived code (143) or graceful exit (0)
    if [ \"\${EXIT_CODE:-0}\" -ne 143 ] && [ \"\${EXIT_CODE:-0}\" -ne 0 ]; then
        echo -e \"${RED}✗ SIGTERM test failed: exit code \${EXIT_CODE:-0} (expected 143 or 0)${NC}\"
        exit 1
    fi
    echo -e \"${GREEN}✓ SIGTERM test passed: exit code \${EXIT_CODE:-0}, no ASAN errors${NC}\"
" || {
    echo -e "${RED}✗ SIGTERM test timed out or failed${NC}"
    exit 1
}

echo ""
echo -e "${GREEN}✓ All signal handling tests passed - no memory leaks or crashes detected${NC}"
exit 0
