# ADR-003: Dual Compiler Support (GCC and Clang)

## Status
Accepted

## Context
C programs can be compiled with different compilers, each with their own characteristics, optimizations, and warning systems. The most common C compilers on Linux/Unix systems are:
- **GCC (GNU Compiler Collection)**: The default compiler on most Linux distributions
- **Clang/LLVM**: A modern compiler with excellent diagnostics and static analysis

For a simple "Hello World" program, the choice of compiler might seem trivial. However, supporting multiple compilers provides several benefits for code quality, portability, and developer experience.

Key considerations:
1. **Code quality**: Different compilers have different warning systems and may catch different issues
2. **Portability**: Ensuring code works with multiple compilers improves cross-platform compatibility
3. **Performance**: Different compilers may generate code with different performance characteristics
4. **Developer choice**: Contributors may prefer different compilers for their development workflow
5. **CI/CD validation**: Testing with multiple compilers increases confidence in code correctness

## Decision
This project explicitly supports and tests with **both GCC and Clang**. The support includes:

1. **Build compatibility**: Code compiles cleanly with both compilers using strict warning flags
2. **CI/CD validation**: Separate CI jobs test with both GCC and Clang
3. **Identical output**: Both compilers must produce binaries that generate byte-identical output
4. **Documentation**: Build instructions document both compiler options
5. **Make targets**: Makefile provides targets for both compilers (`make` for GCC, `make clang` for Clang)

Build commands:
- **GCC**: `gcc -Wall -Wextra -o hello hello.c`
- **Clang**: `clang -Wall -Wextra -o hello_clang hello.c`

## Consequences

### Positive
- **Better code quality**: Two compilers means two sets of diagnostics and warnings
- **Increased confidence**: If both compilers accept the code, it's more likely to be correct
- **Flexibility**: Developers can use their preferred toolchain
- **Portability assurance**: Code that works with both compilers is more portable
- **Learning opportunity**: Comparing compiler outputs can reveal interesting optimization differences

### Negative
- **CI overhead**: Running tests with both compilers doubles some CI time (though for this small program, the overhead is negligible)
- **Maintenance burden**: Changes must be tested with both compilers
- **Timing differences**: GCC compiles faster (~1 second) than Clang (~4 seconds) for this codebase

### Neutral
- Both compilers produce binaries with identical runtime behavior (verified by byte-level validation)
- The dual compiler approach is documented in copilot-instructions.md for contributor guidance
- Compilation time differences are documented but don't impact development workflow

## Implementation Details

### CI Workflow
The `.github/workflows/ci.yml` defines two separate jobs:
1. `build-and-test-gcc`: Builds and tests with GCC
2. `build-and-test-clang`: Builds and tests with Clang

Both jobs run the same validation tests to ensure output consistency.

### Timing Characteristics
- **GCC compilation**: Under 1 second
- **Clang compilation**: ~4 seconds
- **Runtime**: Both produce identical instantaneous execution

### Strict Compilation Flags
Both compilers use strict warning flags to ensure code quality:
- `-Wall`: Enable all common warnings
- `-Wextra`: Enable additional warnings
- `-Wpedantic`: Enforce strict ISO C compliance
- `-Werror`: Treat warnings as errors (in CI)

## References
- Build instructions: README.md and copilot-instructions.md
- CI configuration: `.github/workflows/ci.yml`
- Validation: ADR-002-byte-level-validation.md
