#!/bin/bash
# Unit test runner for hello.c
# Compiles and runs the unit tests using a lightweight testing framework

set -e  # Exit on any error
set -u  # Exit on unset variables
set -o pipefail  # Exit on pipeline failures

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Print colored messages
print_info() {
    printf '%b' "${YELLOW}ℹ $1${NC}\n"
}

print_success() {
    printf '%b' "${GREEN}✓ $1${NC}\n"
}

print_error() {
    printf '%b' "${RED}✗ $1${NC}\n"
}

# Change to repository root
cd "$(dirname "$0")/.."

print_info "Building unit tests..."

# Use the Makefile to build the library and test runner
# This eliminates the need for -DUNIT_TEST hacks
if make test 2>&1; then
    print_success "Unit tests completed successfully"
    exit 0
else
    print_error "Unit tests failed"
    exit 1
fi
