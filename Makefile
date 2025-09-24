# Makefile for Hello World C program
# Standardizes build commands and provides consistent targets for development and CI
#
# IMPORTANT: The 'strict' target uses -Werror which causes builds to fail on any warning
# Override with: make strict STRICT_FLAGS="-Wpedantic -Wformat=2 -Wconversion -Wsign-conversion"
#
# Environment Requirements:
# - GCC 4.8+ or Clang 3.5+ (tested with GCC 13.3.0, Clang 14.0)
# - Linux/Unix environment (flags -fno-exceptions -fno-asynchronous-unwind-tables)
# - Size reduction flags are for C code compatibility and minimal binary size

# Compiler and flags configuration
CC ?= gcc
CLANG ?= clang
CFLAGS ?= -Wall -Wextra -std=c99
OPTFLAGS = -O2
DEBUGFLAGS = -g
# Size reduction flags for minimal binary size
# -s: strip symbols (linker flag, safe for all builds)
# -fno-exceptions and -fno-asynchronous-unwind-tables: mainly for C++ builds, 
# may be ignored in pure C but accepted by GCC 4.8+ and Clang 3.5+
SIZE_FLAGS ?= -s -fno-exceptions -fno-asynchronous-unwind-tables
# STRICT_FLAGS: includes -Werror to fail on any warnings, ensuring code quality
# WARNING: -Werror causes builds to fail on any warning - may be disruptive
# Can be overridden to disable -Werror in development:
# Example: make strict STRICT_FLAGS="-Wpedantic -Wformat=2 -Wconversion -Wsign-conversion"
STRICT_FLAGS ?= -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror

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
OPTIMIZED_TARGET = hello_optimized

# Default target builds optimized hello binary (with -O2)
all: $(TARGET)

# Default optimized build target (hello binary with -O2)
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

# Clang build target (with optimization) - falls back to CC if clang not available
clang: $(CLANG_TARGET)

$(CLANG_TARGET): $(SOURCE)
	@if command -v $(CLANG) >/dev/null 2>&1; then \
		$(CLANG) $(OPT_CFLAGS) -o $(CLANG_TARGET) $(SOURCE); \
	else \
		echo "Warning: clang not found, using $(CC) instead"; \
		$(CC) $(OPT_CFLAGS) -o $(CLANG_TARGET) $(SOURCE); \
	fi

# Optimized build target with strip and size reduction flags for minimal size
optimized: $(OPTIMIZED_TARGET)

$(OPTIMIZED_TARGET): $(SOURCE)
	$(CC) $(OPT_CFLAGS) $(SIZE_FLAGS) -o $(OPTIMIZED_TARGET) $(SOURCE)

# Run the default binary
run: $(TARGET)
	./$(TARGET)

# Test target: run validation script (which will call make strict)
test:
	bash test/validate.sh

# Test target with quiet output for CI environments
test-quiet:
	bash test/validate.sh --quiet

# Validate target: run validation script on built binaries (compatibility with main branch)
validate: strict
	@echo "=== Validating strict build ==="
	@bash test/validate.sh ./$(STRICT_TARGET)

# Comprehensive validation target: test all build variants (for CI)
# Validates all available build variants, skipping unavailable ones with clear messages
validate-all: all debug strict optimized
	@echo "=== Validating default build ==="
	@bash test/validate.sh ./$(TARGET)
	@echo ""
	@echo "=== Validating debug build ==="
	@bash test/validate.sh ./$(DEBUG_TARGET)
	@echo ""
	@echo "=== Validating strict build ==="
	@bash test/validate.sh ./$(STRICT_TARGET)
	@echo ""
	@echo "=== Validating optimized build ==="
	@bash test/validate.sh ./$(OPTIMIZED_TARGET)
	@echo ""
	@echo "=== Validating Clang build ==="
	@if command -v $(CLANG) >/dev/null 2>&1; then \
		$(MAKE) clang && bash test/validate.sh ./$(CLANG_TARGET); \
	else \
		echo "Skipping Clang build - clang not available"; \
	fi

# Clean build artifacts
clean:
	rm -f $(TARGET) $(DEBUG_TARGET) $(CLANG_TARGET) $(STRICT_TARGET) $(OPTIMIZED_TARGET) *.exe *.out *.o

# Help target to show available commands
help:
	@echo "Available targets:"
	@echo "  all      - Build default optimized hello (default)"
	@echo "  debug    - Build with debug flags (-g)"
	@echo "  strict   - Build with strict warnings and -Werror"
	@echo "  clang    - Build with clang compiler"
	@echo "  optimized - Build with -O2 -s and size reduction flags for minimal size"
	@echo "  run      - Run the default binary"
	@echo "  test     - Build strict and run (for CI/validation)"
	@echo "  test-quiet - Same as test but with minimal output"
	@echo "  validate - Validate strict build output and exit code"
	@echo "  validate-all - Validate all build variants"
	@echo "  clean    - Remove all build artifacts"
	@echo "  help     - Show this help message"

# Declare phony targets
.PHONY: all debug strict clang optimized run test test-quiet validate validate-all clean help
