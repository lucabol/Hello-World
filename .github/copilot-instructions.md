# Hello-World C Program

This is a simple C "Hello World" program that prints "Ciao, Mondo!" and displays an exit code. The repository contains a single C source file with minimal dependencies.

**ALWAYS follow these instructions first** and only fall back to additional search or bash commands if the information here is incomplete or found to be in error.

## Working Effectively

### Quick Start - Build and Run
- `gcc -o hello hello.c` -- compiles in under 1 second
- `./hello` -- runs the program and displays "Ciao, Mondo!" followed by "Exit code: 0"

### Development Workflow
- **Primary build command:** `gcc -o hello hello.c`
- **Build with warnings:** `gcc -Wall -Wextra -o hello hello.c` -- recommended for development
- **Debug build:** `gcc -g -Wall -Wextra -o hello_debug hello.c`
- **Optimized build:** `gcc -O2 -Wall -Wextra -o hello hello.c`
- **Strict compilation:** `gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c`

### Alternative Compilers
- **Clang:** `clang -o hello hello.c` -- takes ~4 seconds, produces identical output
- **Both GCC and Clang are available** and work correctly with this codebase

### Timing Expectations
- **Compilation time:** Under 1 second with GCC, ~4 seconds with Clang
- **NEVER CANCEL:** Not applicable - all builds complete in under 5 seconds
- **No timeouts needed** - builds are instantaneous

## Validation

### Manual Testing Scenarios
**ALWAYS run these validation steps after making any changes:**
1. **Compile the program:** `gcc -Wall -Wextra -o hello hello.c`
2. **Run the program:** `./hello`
3. **Verify output contains:**
   - "Ciao, Mondo!" on the first line
   - "Exit code: 0" on the third line (after blank line)
4. **Check exit code:** `echo $?` should return 0

### Additional Validation
- **Test with strict warnings:** `gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c` -- should compile without warnings
- **Test alternative compiler:** `clang -o hello_clang hello.c && ./hello_clang` -- should produce identical output
- **Debug build test:** `gcc -g -o hello_debug hello.c && ./hello_debug` -- should work identically

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
# Quick validation workflow
gcc -Wall -Wextra -o hello hello.c && ./hello
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
- **Functionality:** Prints "Ciao, Mondo!" followed by exit code 0

## Dependencies and Environment

### Required Tools
- **GCC compiler** (available at `/usr/bin/gcc`, version 13.3.0)
- **GNU Make** (available at `/usr/bin/make`) - optional, no Makefile provided
- **Clang compiler** (available at `/usr/bin/clang`) - alternative option

### No Additional Dependencies
- **No package managers** (npm, pip, etc.) required
- **No external libraries** beyond standard C library
- **No build configuration files** (CMakeLists.txt, Makefile, etc.)
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

# Test compilation and execution
gcc -o hello hello.c && ./hello && echo "Build successful"
```

### Expected Output Format
```
Ciao, Mondo!

Exit code: 0
```

## Development Guidelines

### Code Changes
- **Always compile with warnings:** Use `-Wall -Wextra` flags
- **Test immediately:** Run `./hello` after any compilation
- **Validate output format:** Ensure the exact output format is maintained
- **Check exit code:** Program should always return 0

### File Management
- **Compiled binaries are gitignored** - don't commit hello, *.exe, *.out files
- **Source modifications:** Only hello.c should be modified for functional changes
- **Documentation updates:** Update this file if build process changes

## Quick Reference

### One-Line Commands
```bash
# Build and test
gcc -Wall -Wextra -o hello hello.c && ./hello

# Clean and rebuild
rm -f hello && gcc -o hello hello.c

# Strict compilation check
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c
```

### Expected Timings
- **Compilation:** < 1 second (GCC), ~4 seconds (Clang)
- **Execution:** Instantaneous
- **Full workflow:** < 2 seconds total

**Remember:** This is a minimal C project with no complex dependencies. All operations are fast and straightforward.