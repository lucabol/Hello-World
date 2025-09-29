# Hello World

Test repo for JediMaster

## Quick Start

- `make` - Build the program (builds in ~0.03s)
- `./hello` - Run the program (outputs "Hello world!" with no trailing newline)
- `make test` - Run validation tests to reproduce CI checks locally

## Build Targets

- `make strict` - Build with strict flags (-Werror)
- `make clang` - Build with Clang compiler  
- `make optimized` - Build with -O2 optimization
- `make test-quiet` - Run tests in CI mode