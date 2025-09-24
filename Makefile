# Makefile for Hello World C program
# Provides build targets for development and CI

# Compiler and flags configuration
CC ?= gcc
CLANG ?= clang
CFLAGS ?= -Wall -Wextra -std=c99
OPTFLAGS = -O2
DEBUGFLAGS = -g
# STRICT_FLAGS: includes -Werror to fail on any warnings, ensuring code quality
STRICT_FLAGS = -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror

# Combined flag sets for cleaner target definitions
STRICT_CFLAGS = $(CFLAGS) $(STRICT_FLAGS)
DEBUG_CFLAGS = $(CFLAGS) $(DEBUGFLAGS)
OPT_CFLAGS = $(CFLAGS) $(OPTFLAGS)

# Source files and targets
SOURCE = hello.c
TARGET = hello
DEBUG_TARGET = hello_debug
CLANG_TARGET = hello_clang
STRICT_TARGET = hello_strict

# Default target (optimized build)
all: $(TARGET)

# Optimized build target
$(TARGET): $(SOURCE)
	$(CC) $(OPT_CFLAGS) -o $(TARGET) $(SOURCE)

# Debug build target (debug info without optimization)
debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(SOURCE)
	$(CC) $(DEBUG_CFLAGS) -o $(DEBUG_TARGET) $(SOURCE)

# Strict compilation target (with -Werror for CI/quality assurance)
strict: $(STRICT_TARGET)

$(STRICT_TARGET): $(SOURCE)
	$(CC) $(STRICT_CFLAGS) -o $(STRICT_TARGET) $(SOURCE)

# Clang build target (with optimization)
clang: $(CLANG_TARGET)

$(CLANG_TARGET): $(SOURCE)
	$(CLANG) $(OPT_CFLAGS) -o $(CLANG_TARGET) $(SOURCE)

# Run the default binary
run: $(TARGET)
	./$(TARGET)

# Test target: run validation script (which will call make strict)
test:
	bash test/validate.sh

# Test target with quiet output for CI environments
test-quiet:
	bash test/validate.sh --quiet

# Validate target: run validation script on built binaries
validate: strict
	@chmod +x test/validate.sh
	@echo "=== Validating strict build ==="
	@bash test/validate.sh

# Comprehensive validation target: test all build variants (for CI)
validate-all: all strict clang
	@chmod +x test/validate.sh
	@echo "=== Validating optimized build ==="
	@bash test/validate.sh
	@echo ""
	@echo "=== Validating strict build ==="
	@bash test/validate.sh
	@echo ""
	@echo "=== Validating Clang build ==="
	@bash test/validate.sh

# Clean build artifacts
clean:
	rm -f $(TARGET) $(DEBUG_TARGET) $(CLANG_TARGET) $(STRICT_TARGET) *.exe *.out *.o

# Help target to show available commands
help:
	@echo "Available targets:"
	@echo "  all      - Build optimized hello (default)"
	@echo "  debug    - Build with debug flags (-g)"
	@echo "  strict   - Build with strict warnings and -Werror"
	@echo "  clang    - Build with clang compiler"
	@echo "  run      - Run the default binary"
	@echo "  test     - Build strict and run validation"
	@echo "  test-quiet - Same as test but with minimal output"
	@echo "  validate - Validate strict build output and exit code"
	@echo "  validate-all - Validate all build variants"
	@echo "  clean    - Remove all build artifacts"
	@echo "  help     - Show this help message"

# Declare phony targets
.PHONY: all debug strict clang run test test-quiet validate validate-all clean help