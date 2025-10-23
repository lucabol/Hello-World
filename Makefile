# Makefile for Hello World C Program
# Provides standardized build targets for local development and CI

# Compiler settings
CC = gcc
CLANG = clang

# Compiler flags
CFLAGS = -Wall -Wextra
STRICT_FLAGS = $(CFLAGS) -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
DEBUG_FLAGS = -g $(CFLAGS)
UNIT_TEST_FLAGS = -DUNIT_TEST

# Linker flags (currently none needed)
LDFLAGS =

# Source files
SRC = hello.c
HEADER = hello.h
TEST_SRC = test/test_hello.c
TEST_HEADER = test/simple_test.h

# Output binaries
HELLO = hello
HELLO_STRICT = hello_strict
HELLO_CLANG = hello_clang
HELLO_DEBUG = hello_debug
HELLO_LIB = hello_lib.o
TEST_RUNNER = test_hello_runner

# Phony targets (targets that don't represent files)
.PHONY: all strict clang debug unit-test test test-quiet clean help

# Default target - standard build
all: $(HELLO)

$(HELLO): $(SRC) $(HEADER)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Strict build with all warnings as errors (for CI)
strict: $(SRC) $(HEADER)
	$(CC) $(STRICT_FLAGS) -o $(HELLO_STRICT) $(SRC) $(LDFLAGS)

# Build with clang compiler
# Note: You can also use CC=clang for standard builds
clang: $(SRC) $(HEADER)
	$(CLANG) $(CFLAGS) -o $(HELLO_CLANG) $(SRC) $(LDFLAGS)

# Debug build with debugging symbols
debug: $(SRC) $(HEADER)
	$(CC) $(DEBUG_FLAGS) -o $(HELLO_DEBUG) $(SRC) $(LDFLAGS)

# Unit tests target - compile hello.c as library and link with test
# Uses trap to ensure cleanup happens even on test failure
unit-test: $(SRC) $(HEADER) $(TEST_SRC) $(TEST_HEADER)
	@echo "Building unit tests..."
	@trap 'rm -f $(HELLO_LIB) $(TEST_RUNNER)' EXIT; \
	$(CC) $(STRICT_FLAGS) -I. -c -o $(HELLO_LIB) $(SRC) $(UNIT_TEST_FLAGS) && \
	$(CC) $(STRICT_FLAGS) -I. -o $(TEST_RUNNER) $(TEST_SRC) $(HELLO_LIB) $(LDFLAGS) && \
	(echo "Running unit tests..." && ./$(TEST_RUNNER))

# Test target - run validation script
test:
	bash test/validate.sh

# Test target with quiet output (for CI)
test-quiet:
	bash test/validate.sh --quiet

# Clean all build artifacts
clean:
	rm -f $(HELLO) $(HELLO_STRICT) $(HELLO_CLANG) $(HELLO_DEBUG)
	rm -f $(HELLO_LIB) $(TEST_RUNNER)
	rm -f *.o *.exe *.out
	rm -f test/*.o test/*.exe test/*.out

# Help target
help:
	@echo "Available targets:"
	@echo "  all (default) - Build the hello program"
	@echo "  strict        - Build with strict compiler warnings and -Werror"
	@echo "  clang         - Build using clang compiler"
	@echo "  debug         - Build with debugging symbols"
	@echo "  unit-test     - Compile and run unit tests"
	@echo "  test          - Run validation test script"
	@echo "  test-quiet    - Run validation test script with minimal output"
	@echo "  clean         - Remove all build artifacts"
	@echo "  help          - Show this help message"
	@echo ""
	@echo "Variables (can be overridden on command line):"
	@echo "  CC            - C compiler (default: gcc)"
	@echo "                  Example: make CC=clang"
	@echo "  CLANG         - Clang compiler (default: clang)"
	@echo "  CFLAGS        - Standard compiler flags (default: -Wall -Wextra)"
	@echo "                  Example: make CFLAGS='-O2 -Wall'"
	@echo "  STRICT_FLAGS  - Strict compiler flags for quality assurance"
	@echo "  DEBUG_FLAGS   - Compiler flags for debugging builds"
	@echo "  LDFLAGS       - Linker flags"
	@echo "                  Example: make LDFLAGS='-lm'"
