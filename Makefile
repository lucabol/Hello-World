# Makefile for Hello World C program
# Provides standard targets referenced by README.md and CI workflows
#
# Main targets:
#   make         - Build standard binary with optimization (default)
#   make strict  - Build with strict warnings and -Werror (used by CI validation)
#   make test    - Run validation script (CI validation step)
#   make unit-test - Compile and run unit tests (CI unit test step)
#   make clang   - Build with Clang compiler (CI Clang build)
#   make debug   - Build with debug symbols (development)
#   make clean   - Remove all build artifacts

# Compiler settings
CC = gcc
CLANG = clang
CFLAGS = -O2 -Wall -Wextra
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
DEBUG_FLAGS = -g -Wall -Wextra

# Source files
SRC = hello.c
TEST_SRC = test/test_hello.c

# Output binaries
OUT = hello
OUT_CLANG = hello_clang
OUT_DEBUG = hello_debug
OUT_STRICT = hello_strict
TEST_RUNNER = test_hello_runner

# Phony targets (not actual files)
.PHONY: all strict test unit-test clang debug clean

# Default target - referenced in README "Quick Start" section
# Builds optimized binary: gcc -O2 -Wall -Wextra -o hello hello.c
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

# Strict build - referenced in README "Development Workflow" and CI validation
# Uses -Werror to fail on any warnings (CI quality assurance)
# Command: gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -std=c99 -Werror -o hello_strict hello.c
strict:
	$(CC) $(STRICT_FLAGS) -o $(OUT_STRICT) $(SRC)

# Test target - referenced in README "Validation" section and CI workflow (ci.yml line 24)
# Runs validation script that verifies program output and exit code
# CI uses: bash test/validate.sh --quiet
test:
	bash test/validate.sh --quiet

# Unit test target - referenced in CI workflow (ci.yml line 21)
# Compiles and runs unit tests using the test runner script
# CI uses: make unit-test
unit-test:
	bash test/run_unit_tests.sh

# Clang build - referenced in README "Alternative Compilers" and CI workflow (ci.yml line 34)
# Builds with Clang for compatibility testing
# Command: clang -Wall -Wextra -o hello_clang hello.c
clang:
	$(CLANG) -Wall -Wextra -o $(OUT_CLANG) $(SRC)

# Debug build - referenced in README "Development Workflow"
# Builds with debug symbols for debugging
# Command: gcc -g -Wall -Wextra -o hello_debug hello.c
debug:
	$(CC) $(DEBUG_FLAGS) -o $(OUT_DEBUG) $(SRC)

# Clean target - referenced in README "Common Tasks" section
# Removes all compiled binaries and test artifacts
# Pattern matches from .gitignore to ensure consistency
clean:
	rm -f $(OUT) $(OUT_DEBUG) $(OUT_STRICT) $(OUT_CLANG)
	rm -f hello_optimized hello_warnings hello_test* voice_demo voice_demo_test $(TEST_RUNNER)
	rm -f *.exe *.out *.o
	rm -f test/*.o test/*.exe test/*.out
