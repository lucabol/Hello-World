# Makefile for Hello World C Program
# Provides standardized build targets for compilation and testing

# Compiler settings
CC = gcc
CLANG = clang
CFLAGS = -Wall -Wextra
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99

# Output binaries
HELLO_BIN = hello
HELLO_DEBUG = hello_debug
HELLO_CLANG = hello_clang
TEST_RUNNER = test_hello_runner
HELLO_LIB = hello_lib.o

# Source files
SRC = hello.c
TEST_SRC = test/test_hello.c

# Default target: build the main program
all: $(HELLO_BIN)

# Build the main hello program
$(HELLO_BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(HELLO_BIN) $(SRC)

# Build with strict flags (for CI validation)
strict: $(SRC)
	$(CC) $(STRICT_FLAGS) -o $(HELLO_BIN) $(SRC)

# Build with debug symbols
debug: $(SRC)
	$(CC) -g $(CFLAGS) -o $(HELLO_DEBUG) $(SRC)

# Build with Clang
clang: $(SRC)
	$(CLANG) $(CFLAGS) -o $(HELLO_CLANG) $(SRC)

# Run unit tests
unit-test: $(SRC) $(TEST_SRC)
	@echo "Running unit tests..."
	@bash test/run_unit_tests.sh

# Run validation tests (byte-level output validation)
validate: $(SRC)
	@bash test/validate.sh

# Run all tests (unit tests + validation)
test: unit-test validate

# Run tests in quiet mode (for CI)
test-quiet:
	@bash test/validate.sh --quiet

# Clean all build artifacts
clean:
	rm -f $(HELLO_BIN) $(HELLO_DEBUG) $(HELLO_CLANG) $(TEST_RUNNER) $(HELLO_LIB)
	rm -f hello_strict *.exe *.out *.o

# Phony targets (not actual files)
.PHONY: all strict debug clang unit-test validate test test-quiet clean
