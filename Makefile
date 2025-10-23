# Makefile for Hello World C program
# Provides build targets for different compilation modes and testing

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
DEBUG_FLAGS = -g -Wall -Wextra
OPT_FLAGS = -O2 -Wall -Wextra

# Targets
TARGET = hello
DEBUG_TARGET = hello_debug
STRICT_TARGET = hello_strict
CLANG_TARGET = hello_clang
TEST_RUNNER = test_hello_runner

# Source files
SRC = hello.c
HEADER = hello.h
TEST_SRC = test/test_hello.c

# Default target
all: $(TARGET)

# Standard build
$(TARGET): $(SRC) $(HEADER)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Debug build
debug: $(SRC) $(HEADER)
	$(CC) $(DEBUG_FLAGS) -o $(DEBUG_TARGET) $(SRC)

# Optimized build
optimized: $(SRC) $(HEADER)
	$(CC) $(OPT_FLAGS) -o hello_optimized $(SRC)

# Strict build (matches CI validation)
strict: $(SRC) $(HEADER)
	$(CC) $(STRICT_FLAGS) -o $(STRICT_TARGET) $(SRC)

# Clang build
clang: $(SRC) $(HEADER)
	clang $(CFLAGS) -o $(CLANG_TARGET) $(SRC)

# Unit tests
unit-test:
	@bash test/run_unit_tests.sh

# Validation tests
validate:
	@bash test/validate.sh

# Validation tests (quiet mode for CI)
validate-quiet:
	@bash test/validate.sh --quiet

# Run all tests
test: unit-test validate

# Run all tests (quiet mode for CI)
test-quiet: unit-test validate-quiet

# Clean build artifacts
clean:
	rm -f $(TARGET) $(DEBUG_TARGET) $(STRICT_TARGET) $(CLANG_TARGET) hello_optimized
	rm -f $(TEST_RUNNER) hello_lib.o
	rm -f *.exe *.out *.o
	rm -f test/*.o test/*.exe test/*.out

# Phony targets
.PHONY: all debug optimized strict clang unit-test validate validate-quiet test test-quiet clean
