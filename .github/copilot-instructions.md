# Hello-World C Program

This is a simple C "Hello World" program. The repository contains a single C source file with minimal dependencies.

**ALWAYS follow these instructions first** and only fall back to additional search or bash commands if the information here is incomplete or found to be in error.

## Working Effectively

### Quick Start - Build and Run
- `make` or `gcc -o hello hello.c` -- compiles in under 1 second
- `./hello` -- runs the program and outputs exactly "Hello world!" (with trailing newline)

### Development Workflow
- **Primary build command:** `make` or `gcc -o hello hello.c`
- **Build with warnings:** `gcc -Wall -Wextra -o hello hello.c` -- recommended for development
- **Strict build (CI-like):** `make strict` -- uses -Werror and matches CI validation
- **Debug build:** `gcc -g -Wall -Wextra -o hello_debug hello.c`
- **Optimized build:** `gcc -O2 -Wall -Wextra -o hello hello.c`

### Alternative Compilers
- **Clang:** `clang -o hello hello.c` -- takes ~4 seconds, produces identical output
- **Both GCC and Clang are available** and work correctly with this codebase

### Timing Expectations
- **Compilation time:** Under 1 second with GCC, ~4 seconds with Clang
- **NEVER CANCEL:** Not applicable - all builds complete in under 5 seconds
- **No timeouts needed** - builds are instantaneous

## Validation

### CI Process and Local Testing
**To reproduce CI failures locally, run:**
- `make test` -- runs the exact same validation as CI
- `./test/validate.sh` -- direct validation script (what CI uses)
- `make test-quiet` -- CI-style output (minimal verbosity)

**CI runs these steps:**
1. Build using `make strict` (with -Werror for quality assurance)
2. Execute `./test/validate.sh` which validates:
   - Strict compilation passes without warnings/errors
   - Program outputs exactly "Hello world!" (with trailing newline)
   - Program exits with code 0
   - Byte-level output verification using hex dumps

### Manual Testing Scenarios
**ALWAYS run these validation steps after making any changes:**
1. **Compile the program:** `make strict` (or `gcc -Wall -Wextra -Wpedantic -Werror -o hello hello.c`)
2. **Run the program:** `./hello`
3. **Verify output is exactly:** `Hello world!` (with trailing newline)
4. **Check exit code:** `echo $?` should return 0

### Additional Validation
- **Test with strict warnings:** `make strict` -- should compile without warnings and errors
- **Test alternative compiler:** `make clang` -- should produce identical output
- **Debug build test:** `make debug` -- should work identically
- **Run CI validation:** `make test` or `./test/validate.sh` -- reproduces CI checks locally

## Common Tasks

### Building
```bash
# Standard build
gcc -o hello hello.c

# Development build (recommended)
gcc -Wall -Wextra -o hello hello.c

# Production build
gcc -O2 -Wall -Wextra -o hello hello.c
```

### Cleaning
```bash
# Remove compiled binaries
rm -f hello hello_debug hello_clang *.exe *.out
```

### Testing Changes
```bash
# Quick validation workflow (matches CI)
make test

# Alternative: direct validation script
./test/validate.sh
```

## Repository Structure

### Root Directory Contents
```
.
├── .git/              # Git repository data
├── .gitignore         # Excludes compiled binaries (hello, *.exe, *.out, *.o, etc.)
├── README.md          # Basic repository description: "Test repo for JediMaster"
└── hello.c            # Main C source file (157 bytes)
```

### Source Code Overview
- **hello.c:** Contains main() function that prints greeting and exit code
- **Language:** C (uses stdio.h)
- **Functionality:** Prints an Hello World style message

## Dependencies and Environment

### Required Tools
- **GCC compiler** (available at `/usr/bin/gcc`, version 13.3.0)
- **GNU Make** (available at `/usr/bin/make`) - optional, no Makefile provided
- **Clang compiler** (available at `/usr/bin/clang`) - alternative option

### No Additional Dependencies
- **No package managers** (npm, pip, etc.) required
- **No external libraries** beyond standard C library
- **Has build configuration:** Makefile with standardized targets
- **Has CI/CD validation:** test/validate.sh script ensures output compliance

## Troubleshooting

### Common Issues and Solutions
- **"Command not found" for gcc:** Ensure GCC is installed: `which gcc`
- **Permission denied for ./hello:** Make sure file is executable or use: `chmod +x hello`
- **"No such file" when running ./hello:** Ensure you compiled first: `gcc -o hello hello.c`

### Build Verification
```bash
# Verify compiler availability
which gcc && gcc --version

# Test compilation and execution
gcc -o hello hello.c && ./hello && echo "Build successful"
```

### Expected Output Format
**Exact output:** "Hello world!" (with trailing newline, exit code 0)

This format is validated by test scripts and CI to ensure exact byte-level compliance.

## Development Guidelines

### Code Changes
- **Always use make targets:** Use `make test` to validate changes
- **Test immediately:** Run `make test` after any compilation to match CI
- **Validate exact output format:** Must output exactly "Hello world!" with trailing newline
- **Check exit code:** Program should always return 0

### File Management
- **Compiled binaries are gitignored** - don't commit hello, *.exe, *.out files
- **Source modifications:** Only hello.c should be modified for functional changes
- **Documentation updates:** Update this file if build process changes

### Quick Reference

### One-Line Commands
```bash
# Build and test (recommended)
make test

# Clean and rebuild
make clean && make

# Test strict compilation (CI-like)
make strict
```

### Expected Timings
- **Compilation:** < 1 second (GCC), ~4 seconds (Clang)
- **Execution:** Instantaneous
- **Full workflow:** < 2 seconds total

**Remember:** This is a minimal C project with no complex dependencies. All operations are fast and straightforward.
