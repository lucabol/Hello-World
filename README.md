# Hello World
Test repo for JediMaster

## Overview
This is a minimal C "Hello World" program demonstrating basic C programming concepts.

## Current Repository Contents
- `hello.c` - Main C source file that prints "Hello world!" (no trailing newline)
- `Makefile` - Build configuration with standardized targets
- `test/validate.sh` - Validation script for testing the C program
- `.github/workflows/ci.yml` - CI/CD workflow configuration

## Building and Running

### Quick Start
```bash
# Compile the program (using Makefile)
make

# Or compile directly with GCC
gcc -Wall -Wextra -o hello hello.c

# Run the program
./hello
```

### Available Make Targets
```bash
make              # Standard build with warnings
make strict       # Strict build with -Werror (matches CI)
make clang        # Build with Clang compiler
make debug        # Debug build with -g flag
make optimized    # Optimized build with -O2
make clean        # Remove compiled binaries
```

### Running Tests
```bash
# Run validation tests
make test

# Or run directly
./test/validate.sh

# Run with minimal output (CI mode)
make test-quiet
# Or run directly
./test/validate.sh --quiet
```

## CI/CD Status
The current CI workflow includes both C and Node.js jobs. **Note**: The Node.js related jobs will currently fail because the required files are not present in this repository.

### Working CI Jobs
- `build-and-test-gcc` - Builds and tests with GCC ✅
- `build-and-test-clang` - Builds and tests with Clang ✅

### Non-functional CI Jobs (Missing Dependencies)
- `collaborative-editor-tests` - Requires: `package.json`, `test_collab.sh`, `collab_server.js`, `test-ot.js`
- `security-tests` - Requires: `package.json`, `test-security.js`

## Future Development
If collaborative editing features are intended for this repository, the following files would need to be added:
- `package.json` - Node.js dependencies
- `test_collab.sh` - Collaborative testing script
- `collab_server.js` - Collaborative server implementation
- `test-ot.js` - Operational Transform tests
- `test-security.js` - Security tests

Until these files are added, the Node.js CI jobs will continue to fail.