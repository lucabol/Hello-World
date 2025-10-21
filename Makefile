# Makefile for Hello World C program
# Provides standard build, test, and clean targets

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra
OPTFLAGS = -O2
DEBUGFLAGS = -g
STRICTFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c99

# Source and output files
SRC = hello.c
TARGET = hello
TARGET_DEBUG = hello_debug
TARGET_CLANG = hello_clang

# Test settings
TEST_VALIDATE = test/validate.sh
TEST_UNIT = test/run_unit_tests.sh

# Default target: optimized build
.PHONY: all
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(OPTFLAGS) $(CFLAGS) -o $(TARGET) $(SRC)

# Strict build with all warnings as errors (CI-like)
.PHONY: strict
strict: $(SRC)
	$(CC) $(OPTFLAGS) $(STRICTFLAGS) -o $(TARGET) $(SRC)

# Debug build with debug symbols
.PHONY: debug
debug: $(SRC)
	$(CC) $(DEBUGFLAGS) $(CFLAGS) -o $(TARGET_DEBUG) $(SRC)

# Build with clang compiler
.PHONY: clang
clang: $(SRC)
	clang $(OPTFLAGS) $(CFLAGS) -o $(TARGET_CLANG) $(SRC)

# Run unit tests
.PHONY: unit-test
unit-test:
	bash $(TEST_UNIT)

# Run validation tests (verbose)
.PHONY: test
test: strict
	bash $(TEST_VALIDATE)

# Run validation tests (quiet mode for CI)
.PHONY: test-quiet
test-quiet: strict
	bash $(TEST_VALIDATE) --quiet

# Clean up build artifacts
.PHONY: clean
clean:
	rm -f $(TARGET) $(TARGET_DEBUG) $(TARGET_CLANG)
	rm -f hello_strict hello_optimized hello_warnings hello_test*
	rm -f voice_demo voice_demo_test test_hello_runner
	rm -f *.o *.exe *.out
	rm -f test/*.o test/*.exe test/*.out
