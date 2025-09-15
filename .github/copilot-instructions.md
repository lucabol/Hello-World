# Hello-World C Program

This is a simple C "Hello World" program. The repository contains a single C source file with minimal dependencies.

**ALWAYS follow these instructions first** and only fall back to additional search or bash commands if the information here is incomplete or found to be in error.

## Working Effectively

### Quick Start - Build and Run
- `make test` -- builds with warnings and runs the program (recommended)
- `make dev && ./hello` -- builds with warnings then runs manually
- Manual: `gcc -Wall -Wextra -o hello hello.c && ./hello`

### Development Workflow (Consolidated)
All compilation commands are now consolidated in the Makefile:
- **Primary build command:** `make` or `make all` (basic build)
- **Development build:** `make dev` -- recommended for development (includes warnings)
- **Debug build:** `make debug` -- creates hello_debug with symbols
- **Optimized build:** `make optimized` -- production build with -O2
- **Strict compilation:** `make strict` -- all warnings and pedantic checks
- **Test build:** `make test` -- build and run in one command

### Legacy Manual Commands (if Makefile unavailable)
- **Basic:** `gcc -o hello hello.c`
- **With warnings:** `gcc -Wall -Wextra -o hello hello.c`
- **Debug:** `gcc -g -Wall -Wextra -o hello_debug hello.c`
- **Optimized:** `gcc -O2 -Wall -Wextra -o hello hello.c`
- **Strict:** `gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c`

### Alternative Compilers
- **Clang:** `make clang` -- consolidated clang build, produces hello_clang
- **Manual Clang:** `clang -o hello hello.c` -- takes ~4 seconds, produces identical output
- **Both GCC and Clang are available** and work correctly with this codebase

### Timing Expectations
- **Compilation time:** Under 1 second with GCC, ~4 seconds with Clang
- **NEVER CANCEL:** Not applicable - all builds complete in under 5 seconds
- **No timeouts needed** - builds are instantaneous

## Validation

### Manual Testing Scenarios
**ALWAYS run these validation steps after making any changes:**
1. **Compile the program:** `make dev` or `gcc -Wall -Wextra -o hello hello.c`
2. **Run the program:** `./hello`
3. **Verify output contains:**
   - "Exit code: 0" on the third line (after blank line)
4. **Check exit code:** `echo $?` should return 0

### Additional Validation
- **Test with strict warnings:** `make strict` -- should compile without warnings
- **Test alternative compiler:** `make clang && ./hello_clang` -- should produce identical output
- **Debug build test:** `make debug && ./hello_debug` -- should work identically

## Common Tasks

### Building
```bash
# Standard build
make

# Development build (recommended)
make dev

# Production build
make optimized

# All build types available
make help
```

### Cleaning
```bash
# Remove compiled binaries (consolidated)
make clean
```

### Testing Changes
```bash
# Quick validation workflow (consolidated)
make test
```

## Repository Structure

### Root Directory Contents
```
.
├── .git/              # Git repository data
├── .gitignore         # Excludes compiled binaries (hello, *.exe, *.out, *.o, etc.)
├── Makefile           # Consolidated build commands for all compilation options
├── README.md          # Updated with consolidated build instructions
└── hello.c            # Main C source file (63 bytes)
```

### Source Code Overview
- **hello.c:** Contains main() function that prints greeting and exit code
- **Language:** C (uses stdio.h)
- **Functionality:** Prints an Hello World style message

## Dependencies and Environment

### Required Tools
- **GCC compiler** (available at `/usr/bin/gcc`, version 13.3.0)
- **GNU Make** (available at `/usr/bin/make`) - now required for consolidated build commands
- **Clang compiler** (available at `/usr/bin/clang`) - alternative option

### No Additional Dependencies
- **No package managers** (npm, pip, etc.) required
- **No external libraries** beyond standard C library
- **Build configuration:** Makefile provided for consolidated compilation commands
- **No CI/CD workflows** currently configured

## Troubleshooting

### Common Issues and Solutions
- **"Command not found" for gcc:** Ensure GCC is installed: `which gcc`
- **Permission denied for ./hello:** Make sure file is executable or use: `chmod +x hello`
- **"No such file" when running ./hello:** Ensure you compiled first: `gcc -o hello hello.c`

### Build Verification
```bash
# Verify compiler availability
which gcc && gcc --version

# Test compilation and execution (consolidated)
make test && echo "Build successful"
```

### Expected Output Format
An Hello World style message

## Development Guidelines

### Code Changes
- **Always compile with warnings:** Use `make dev` or `-Wall -Wextra` flags
- **Test immediately:** Run `make test` after any compilation
- **Validate output format:** Ensure the exact output format is maintained
- **Check exit code:** Program should always return 0

### File Management
- **Compiled binaries are gitignored** - don't commit hello, *.exe, *.out files
- **Source modifications:** Only hello.c should be modified for functional changes
- **Documentation updates:** Update this file if build process changes

## Quick Reference

### One-Line Commands
```bash
# Build and test (consolidated)
make test

# Clean and rebuild
make clean && make dev

# Strict compilation check
make strict
```

### Expected Timings
- **Compilation:** < 1 second (GCC), ~4 seconds (Clang)
- **Execution:** Instantaneous
- **Full workflow:** < 2 seconds total

**Remember:** This is a minimal C project with no complex dependencies. All operations are fast and straightforward.
