# Makefile for Hello World C Program
# Provides standardized build targets for local development and CI

# Compiler settings
CC = gcc
CLANG = clang

# Compiler flags
CFLAGS = -Wall -Wextra
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
DEBUG_FLAGS = -g -Wall -Wextra
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

# Default target - standard build
.PHONY: all
all: $(HELLO)

$(HELLO): $(SRC) $(HEADER)
	$(CC) $(CFLAGS) -o $(HELLO) $(SRC) $(LDFLAGS)

# Strict build with all warnings as errors (for CI)
.PHONY: strict
strict: $(SRC) $(HEADER)
	$(CC) $(STRICT_FLAGS) -o $(HELLO_STRICT) $(SRC) $(LDFLAGS)

# Build with clang compiler
.PHONY: clang
clang: $(SRC) $(HEADER)
	$(CLANG) $(CFLAGS) -o $(HELLO_CLANG) $(SRC) $(LDFLAGS)

# Debug build with debugging symbols
.PHONY: debug
debug: $(SRC) $(HEADER)
	$(CC) $(DEBUG_FLAGS) -o $(HELLO_DEBUG) $(SRC) $(LDFLAGS)

# Unit tests target - compile hello.c as library and link with test
.PHONY: unit-test
unit-test: $(SRC) $(HEADER) $(TEST_SRC) $(TEST_HEADER)
	@echo "Building unit tests..."
	$(CC) $(STRICT_FLAGS) -I. -c -o $(HELLO_LIB) $(SRC) $(UNIT_TEST_FLAGS)
	$(CC) $(STRICT_FLAGS) -I. -o $(TEST_RUNNER) $(TEST_SRC) $(HELLO_LIB) $(LDFLAGS)
	@echo "Running unit tests..."
	./$(TEST_RUNNER)
	@rm -f $(HELLO_LIB) $(TEST_RUNNER)

# Test target - run validation script
.PHONY: test
test:
	bash test/validate.sh

# Test target with quiet output (for CI)
.PHONY: test-quiet
test-quiet:
	bash test/validate.sh --quiet

# Clean all build artifacts
.PHONY: clean
clean:
	rm -f $(HELLO) $(HELLO_STRICT) $(HELLO_CLANG) $(HELLO_DEBUG)
	rm -f $(HELLO_LIB) $(TEST_RUNNER)
	rm -f *.o *.exe *.out
	rm -f test/*.o test/*.exe test/*.out

# Help target
.PHONY: help
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
	@echo "Variables:"
	@echo "  CC            - C compiler (default: gcc)"
	@echo "  CLANG         - Clang compiler (default: clang)"
	@echo "  CFLAGS        - Standard compiler flags"
	@echo "  STRICT_FLAGS  - Strict compiler flags for quality assurance"
	@echo "  DEBUG_FLAGS   - Compiler flags for debugging builds"
	@echo "  LDFLAGS       - Linker flags"
