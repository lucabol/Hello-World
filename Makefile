# Makefile for Hello World C program
#
# Build targets:
#   all        - Default build (basic compilation with C standard)
#   strict     - Strict build with -Wpedantic and -Werror (produces hello_strict for CI use)
#   debug      - Debug build with -g flag (produces hello_debug)
#   optimized  - Optimized build with -O2 (produces hello_optimized)
#   clang      - Build with Clang compiler (produces hello_clang)
#   unit-test  - Build and run unit tests
#   test       - Run validation script
#   test-quiet - Run validation script in quiet mode (for CI)
#   clean      - Remove all build artifacts
#
# All specialized builds (strict/debug/optimized/clang) produce separate binaries
# to avoid clobbering the default build output.

# Compiler settings
CC = gcc
CLANG ?= clang
CSTD ?= c99
CFLAGS = -std=$(CSTD)
WARNINGS = -Wall -Wextra
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Werror
STRICT_ALL_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
DEBUG_FLAGS = -g $(WARNINGS)
OPTIMIZE_FLAGS = -O2 $(WARNINGS)

# Source files
SRC = hello.c
TEST_SRC = test/test_hello.c

# Binary targets
TARGET = hello
TARGET_STRICT = hello_strict
TARGET_DEBUG = hello_debug
TARGET_OPTIMIZED = hello_optimized
TARGET_CLANG = hello_clang
TEST_RUNNER = test_hello_runner
HELLO_OBJ = hello_lib.o

# Default target
all: $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Strict build with pedantic warnings and -Werror
# Produces hello_strict binary to avoid clobbering default build
strict: $(SRC)
	$(CC) $(CFLAGS) $(STRICT_FLAGS) -o $(TARGET_STRICT) $(SRC)

# Debug build
debug: $(SRC)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $(TARGET_DEBUG) $(SRC)

# Optimized build
# Produces hello_optimized binary to avoid clobbering default build
optimized: $(SRC)
	$(CC) $(CFLAGS) $(OPTIMIZE_FLAGS) -o $(TARGET_OPTIMIZED) $(SRC)

# Build with Clang
clang: $(SRC)
	$(CLANG) $(CFLAGS) $(WARNINGS) -o $(TARGET_CLANG) $(SRC)

# Build and run unit tests
# Compiles hello.c with -DUNIT_TEST to exclude main(), links with test runner
unit-test:
	@echo "Building unit tests..."
	$(CC) $(CFLAGS) $(STRICT_ALL_FLAGS) -I. -c -o $(HELLO_OBJ) $(SRC) -DUNIT_TEST
	$(CC) $(CFLAGS) $(STRICT_ALL_FLAGS) -I. -o $(TEST_RUNNER) $(TEST_SRC) $(HELLO_OBJ)
	@echo "Running unit tests..."
	./$(TEST_RUNNER)
	@rm -f $(HELLO_OBJ) $(TEST_RUNNER)

# Run validation tests
test:
	bash test/validate.sh

# Run validation tests in quiet mode (for CI)
test-quiet:
	bash test/validate.sh --quiet

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TARGET_STRICT) $(TARGET_DEBUG) $(TARGET_OPTIMIZED) $(TARGET_CLANG) *.exe *.out *.o
	rm -f $(TEST_RUNNER) $(HELLO_OBJ)
	rm -f test/*.o test/*.exe

# Phony targets
.PHONY: all strict debug optimized clang unit-test test test-quiet clean
