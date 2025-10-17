# Makefile for Hello World C program
# Provides standardized build targets matching CI/CD requirements

# Compiler configuration
CC ?= gcc
CLANG ?= clang

# Source files
SRC = hello.c
TEST_SRC = test/test_hello.c
HEADER = hello.h

# Output binaries
TARGET = hello
TARGET_DEBUG = hello_debug
TARGET_OPTIMIZED = hello_optimized
TARGET_STRICT = hello_strict
TARGET_CLANG = hello_clang
TEST_RUNNER = test_hello_runner
OBJ_LIB = hello_lib.o

# Compiler flags
CFLAGS_BASE = -std=c99
CFLAGS_WARNINGS = -Wall -Wextra -Wpedantic
CFLAGS_STRICT = $(CFLAGS_WARNINGS) -Wformat=2 -Wconversion -Wsign-conversion -Werror
CFLAGS_DEBUG = $(CFLAGS_WARNINGS) -g
CFLAGS_OPTIMIZED = $(CFLAGS_WARNINGS) -O2

# Control verbosity
VERBOSE ?= 0
ifeq ($(VERBOSE),0)
    Q = @
    QUIET_FLAG = --quiet
else
    Q =
    QUIET_FLAG =
endif

# Default target
.DEFAULT_GOAL := all

# Phony targets
.PHONY: all build strict clang unit-test test clean debug optimized help test-quiet

# Help target
help:
	@echo "Available targets:"
	@echo "  all/build      - Build the standard hello binary (default)"
	@echo "  strict         - Build with strict compilation flags (-Werror)"
	@echo "  clang          - Build using Clang compiler"
	@echo "  unit-test      - Compile and run unit tests"
	@echo "  test           - Run validation tests (requires 'build' or 'strict')"
	@echo "  test-quiet     - Run validation tests with minimal output (CI mode)"
	@echo "  clean          - Remove all generated binaries and artifacts"
	@echo "  debug          - Build with debug symbols"
	@echo "  optimized      - Build with optimizations"
	@echo "  help           - Show this help message"
	@echo ""
	@echo "Variables:"
	@echo "  VERBOSE=1      - Show detailed build commands"
	@echo "  CC=<compiler>  - Override default compiler (default: gcc)"

# Standard build target
all: build

build: $(TARGET)

$(TARGET): $(SRC) $(HEADER)
	@echo "Building $(TARGET) with $(CC)..."
	$(Q)$(CC) $(CFLAGS_BASE) $(CFLAGS_WARNINGS) -o $(TARGET) $(SRC)
	@echo "Build complete: $(TARGET)"

# Strict build (CI quality assurance)
strict: $(TARGET_STRICT)

$(TARGET_STRICT): $(SRC) $(HEADER)
	@echo "Building $(TARGET_STRICT) with strict flags..."
	$(Q)$(CC) $(CFLAGS_BASE) $(CFLAGS_STRICT) -o $(TARGET_STRICT) $(SRC)
	@echo "Strict build complete: $(TARGET_STRICT)"

# Clang build
clang: $(TARGET_CLANG)

$(TARGET_CLANG): $(SRC) $(HEADER)
	@echo "Building $(TARGET_CLANG) with Clang..."
	$(Q)$(CLANG) $(CFLAGS_BASE) $(CFLAGS_WARNINGS) -o $(TARGET_CLANG) $(SRC)
	@echo "Clang build complete: $(TARGET_CLANG)"

# Debug build
debug: $(TARGET_DEBUG)

$(TARGET_DEBUG): $(SRC) $(HEADER)
	@echo "Building $(TARGET_DEBUG) with debug symbols..."
	$(Q)$(CC) $(CFLAGS_BASE) $(CFLAGS_DEBUG) -o $(TARGET_DEBUG) $(SRC)
	@echo "Debug build complete: $(TARGET_DEBUG)"

# Optimized build
optimized: $(TARGET_OPTIMIZED)

$(TARGET_OPTIMIZED): $(SRC) $(HEADER)
	@echo "Building $(TARGET_OPTIMIZED) with optimizations..."
	$(Q)$(CC) $(CFLAGS_BASE) $(CFLAGS_OPTIMIZED) -o $(TARGET_OPTIMIZED) $(SRC)
	@echo "Optimized build complete: $(TARGET_OPTIMIZED)"

# Unit tests
unit-test: $(TEST_RUNNER)
	@echo "Running unit tests..."
	$(Q)./$(TEST_RUNNER)

$(TEST_RUNNER): $(TEST_SRC) $(SRC) $(HEADER) test/simple_test.h
	@echo "Building unit tests..."
	$(Q)$(CC) $(CFLAGS_BASE) $(CFLAGS_STRICT) -I. -c -o $(OBJ_LIB) $(SRC) -DUNIT_TEST
	$(Q)$(CC) $(CFLAGS_BASE) $(CFLAGS_STRICT) -I. -o $(TEST_RUNNER) $(TEST_SRC) $(OBJ_LIB)
	@echo "Unit test build complete"

# Validation tests
test: $(TARGET_STRICT)
	@echo "Running validation tests..."
	$(Q)bash test/validate.sh

# Validation tests (quiet mode for CI)
test-quiet: $(TARGET_STRICT)
	$(Q)bash test/validate.sh --quiet

# Clean target
clean:
	@echo "Cleaning build artifacts..."
	$(Q)rm -f $(TARGET) $(TARGET_DEBUG) $(TARGET_OPTIMIZED) $(TARGET_STRICT) $(TARGET_CLANG)
	$(Q)rm -f $(TEST_RUNNER) $(OBJ_LIB)
	$(Q)rm -f *.exe *.out *.o test/*.o test/*.exe test/*.out
	@echo "Clean complete"
