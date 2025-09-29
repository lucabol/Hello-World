#!/bin/bash
# Binary size check script for Hello World C program
# Ensures the binary remains minimal and detects regressions
#
# Usage: ./test/check_binary_size.sh [binary_name] [--quiet]
#        binary_name: Name of the binary to check (default: hello)
#        --quiet: Reduce output verbosity for CI environments

set -e  # Exit on any error
set -u  # Exit on unset variables  
set -o pipefail  # Exit on pipeline failures

# Default configuration
BINARY_NAME="${1:-hello}"
QUIET_MODE=false
SIZE_THRESHOLD_KB=50  # Conservative threshold: 50KB = 51200 bytes
SIZE_THRESHOLD_BYTES=$((SIZE_THRESHOLD_KB * 1024))

# Parse command line arguments
if [[ ${#} -gt 0 ]]; then
    for arg in "$@"; do
        if [[ "${arg}" == "--quiet" ]]; then
            QUIET_MODE=true
        elif [[ "${arg}" != "${BINARY_NAME}" ]]; then
            BINARY_NAME="${arg}"
        fi
    done
fi

# Colors for output (disabled in quiet mode)
if [[ "${QUIET_MODE}" == "false" ]]; then
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    YELLOW='\033[1;33m'
    BLUE='\033[0;34m'
    NC='\033[0m' # No Color
else
    RED=''
    GREEN=''
    YELLOW=''
    BLUE=''
    NC=''
fi

# Function to print colored messages
print_success() {
    if [[ "${QUIET_MODE}" == "false" ]]; then
        printf '%b' "${GREEN}✓ $1${NC}\n"
    fi
}

print_error() {
    printf '%b' "${RED}✗ $1${NC}\n"
}

print_info() {
    if [[ "${QUIET_MODE}" == "false" ]]; then
        printf '%b' "${BLUE}ℹ $1${NC}\n"
    fi
}

print_warning() {
    if [[ "${QUIET_MODE}" == "false" ]]; then
        printf '%b' "${YELLOW}⚠ $1${NC}\n"
    fi
}

print_info "Checking binary size for: ${BINARY_NAME}"

# Step 1: Verify binary exists
if [[ ! -f "${BINARY_NAME}" ]]; then
    print_error "Binary '${BINARY_NAME}' not found"
    print_error "Make sure to build the binary first: gcc -o ${BINARY_NAME} hello.c"
    exit 1
fi

# Step 2: Get binary size
BINARY_SIZE=$(stat -c%s "${BINARY_NAME}")
BINARY_SIZE_KB=$((BINARY_SIZE / 1024))

print_info "Binary size: ${BINARY_SIZE} bytes (${BINARY_SIZE_KB} KB)"

# Step 3: Check against threshold
if [[ ${BINARY_SIZE} -gt ${SIZE_THRESHOLD_BYTES} ]]; then
    print_error "Binary size exceeds threshold!"
    printf "  Current size: %d bytes (%d KB)\n" "${BINARY_SIZE}" "${BINARY_SIZE_KB}"
    printf "  Threshold:    %d bytes (%d KB)\n" "${SIZE_THRESHOLD_BYTES}" "${SIZE_THRESHOLD_KB}"
    printf "  Excess:       %d bytes (%d KB)\n" "$((BINARY_SIZE - SIZE_THRESHOLD_BYTES))" "$(((BINARY_SIZE - SIZE_THRESHOLD_BYTES) / 1024))"
    
    print_info "Analyzing binary to help debug size increase..."
    
    # Show linked symbols and dependencies
    print_info "Linked libraries (ldd):"
    if command -v ldd >/dev/null 2>&1; then
        ldd "${BINARY_NAME}" 2>/dev/null || echo "  Static binary or ldd failed"
    else
        echo "  ldd not available"
    fi
    
    print_info "Symbol table analysis (nm):"
    if command -v nm >/dev/null 2>&1; then
        # Show symbols sorted by size (if supported)
        if nm --size-sort "${BINARY_NAME}" 2>/dev/null | head -10; then
            echo "  (Top 10 symbols by size shown above)"
        else
            # Fallback to basic symbol listing
            nm "${BINARY_NAME}" 2>/dev/null | wc -l && echo " symbols found" || echo "  nm analysis failed"
        fi
    else
        echo "  nm not available"
    fi
    
    print_info "File type and section information:"
    if command -v file >/dev/null 2>&1; then
        file "${BINARY_NAME}"
    fi
    
    if command -v size >/dev/null 2>&1; then
        size "${BINARY_NAME}"
    fi
    
    exit 1
else
    print_success "Binary size is within acceptable limits"
    if [[ "${QUIET_MODE}" == "false" ]]; then
        printf "  Size: %d bytes (%d KB) ≤ %d KB threshold\n" "${BINARY_SIZE}" "${BINARY_SIZE_KB}" "${SIZE_THRESHOLD_KB}"
        # Show percentage of threshold used
        PERCENTAGE=$((BINARY_SIZE * 100 / SIZE_THRESHOLD_BYTES))
        printf "  Threshold usage: %d%%\n" "${PERCENTAGE}"
    else
        printf "Binary size check: PASSED (%d bytes)\n" "${BINARY_SIZE}"
    fi
fi

# Step 4: Record baseline for historical tracking
BASELINE_FILE="test/binary_size_baseline.txt"
if [[ "${QUIET_MODE}" == "false" ]]; then
    print_info "Recording size baseline to ${BASELINE_FILE}"
fi

# Create baseline file with timestamp and size info
{
    echo "# Binary size baseline - automatically updated"
    echo "# Format: timestamp,binary_name,size_bytes,size_kb"
    echo "# Last updated: $(date -u '+%Y-%m-%d %H:%M:%S UTC')"
    echo "${BINARY_NAME},${BINARY_SIZE},${BINARY_SIZE_KB}"
} > "${BASELINE_FILE}"

if [[ "${QUIET_MODE}" == "false" ]]; then
    print_success "Binary size check completed successfully"
    printf "\n"
    print_info "Summary:"
    printf "  - Binary: %s\n" "${BINARY_NAME}"
    printf "  - Size: %d bytes (%d KB)\n" "${BINARY_SIZE}" "${BINARY_SIZE_KB}"
    printf "  - Threshold: %d KB\n" "${SIZE_THRESHOLD_KB}"
    printf "  - Status: PASSED\n"
fi