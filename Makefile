# Makefile for Hello World C Program
# Provides standardized build targets for development and CI

# Compiler settings
CC_GCC = gcc
CC_CLANG = clang
CFLAGS_DEFAULT = -Wall -Wextra
CFLAGS_STRICT = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
CFLAGS_DEBUG = -g -Wall -Wextra

# Output binaries
BIN_DEFAULT = hello
BIN_DEBUG = hello_debug
BIN_CLANG = hello_clang
BIN_TEST = test_hello_runner
OBJ_LIB = hello_lib.o

# Source files
SRC_MAIN = hello.c
SRC_TEST = test/test_hello.c

# Default target: build with standard warnings
.PHONY: all
all: $(BIN_DEFAULT)

$(BIN_DEFAULT): $(SRC_MAIN)
	$(CC_GCC) $(CFLAGS_DEFAULT) -o $(BIN_DEFAULT) $(SRC_MAIN)

# Strict build: compile with strict flags matching CI validation
.PHONY: strict
strict:
	$(CC_GCC) $(CFLAGS_STRICT) -o $(BIN_DEFAULT) $(SRC_MAIN)

# Clang build: compile using clang compiler
.PHONY: clang
clang:
	$(CC_CLANG) $(CFLAGS_DEFAULT) -o $(BIN_CLANG) $(SRC_MAIN)

# Debug build: compile with debug symbols
.PHONY: debug
debug:
	$(CC_GCC) $(CFLAGS_DEBUG) -o $(BIN_DEBUG) $(SRC_MAIN)

# Unit tests: compile and run unit tests
.PHONY: unit-test
unit-test:
	@echo "Building unit tests..."
	$(CC_GCC) $(CFLAGS_STRICT) -I. -c -o $(OBJ_LIB) $(SRC_MAIN) -DUNIT_TEST
	$(CC_GCC) $(CFLAGS_STRICT) -I. -o $(BIN_TEST) $(SRC_TEST) $(OBJ_LIB)
	@echo "Running unit tests..."
	./$(BIN_TEST)

# Validation tests: run the validation script
.PHONY: test
test:
	bash test/validate.sh

# Test with quiet mode (for CI)
.PHONY: test-quiet
test-quiet:
	bash test/validate.sh --quiet

# Clean: remove all compiled binaries and test artifacts
.PHONY: clean
clean:
	rm -f $(BIN_DEFAULT) $(BIN_DEBUG) $(BIN_CLANG) $(BIN_TEST) $(OBJ_LIB)
	rm -f hello_strict *.exe *.out *.o

# Help: display available targets
.PHONY: help
help:
	@echo "Available make targets:"
	@echo "  all (default) - Build with gcc -Wall -Wextra"
	@echo "  strict        - Build with strict flags (matches CI validation)"
	@echo "  clang         - Build using clang compiler"
	@echo "  debug         - Build with debug symbols"
	@echo "  unit-test     - Compile and run unit tests"
	@echo "  test          - Run validation tests"
	@echo "  test-quiet    - Run validation tests in quiet mode (CI)"
	@echo "  clean         - Remove all compiled binaries and artifacts"
	@echo "  help          - Display this help message"
