# Makefile for Hello World C program
#
# Build targets:
#   all        - Default build (basic compilation with C standard)
#   strict     - Strict build with -Wpedantic and -Werror (produces hello_strict for CI use)
#   debug      - Debug build with -g flag (produces hello_debug)
#   optimized  - Optimized build with -O2 (produces hello_optimized)
#   clang      - Build with Clang compiler (produces hello_clang)
#   test       - Run validation script
#   test-quiet - Run validation script in quiet mode (for CI)
#   clean      - Remove all build artifacts
#   help       - Show available targets
#
# All specialized builds (strict/debug/optimized/clang) produce separate binaries
# to avoid clobbering the default build output.

# Compiler settings
CC = gcc
CLANG ?= clang
CSTD ?= c99
CFLAGS = -std=$(CSTD)
WARNINGS = -Wall -Wextra
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror
DEBUG_FLAGS = -g -Wall -Wextra
OPTIMIZE_FLAGS = -O2 -Wall -Wextra

# Source and output files
SRC = hello.c
TARGET = hello
TARGET_STRICT = hello_strict
TARGET_DEBUG = hello_debug
TARGET_OPTIMIZED = hello_optimized
TARGET_CLANG = hello_clang

# Validation script (must exist in repository and be executable)
# The script supports --quiet flag for CI environments
VALIDATE_SCRIPT = test/validate.sh

# Phony targets
.PHONY: all strict debug optimized clang test test-quiet clean help

# Default target
all: $(TARGET)

# Basic build
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $<

# Strict build with all warnings treated as errors and pedantic mode enabled
# Includes -Wpedantic to catch language standard violations and -Werror to treat warnings as errors
# Produces hello_strict binary to avoid clobbering default build
strict: $(SRC)
	$(CC) $(CFLAGS) $(STRICT_FLAGS) -o $(TARGET_STRICT) $<

# Debug build
debug: $(SRC)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $(TARGET_DEBUG) $<

# Optimized build
# Produces hello_optimized binary to avoid clobbering default build
optimized: $(SRC)
	$(CC) $(CFLAGS) $(OPTIMIZE_FLAGS) -o $(TARGET_OPTIMIZED) $<

# Build with Clang
clang: $(SRC)
	$(CLANG) $(CFLAGS) $(WARNINGS) -o $(TARGET_CLANG) $<

# Run validation tests
# Expects: test/validate.sh to exist and be executable
# Exit code: 0 on success, non-zero on failure (propagated from validation script)
test: strict
	@if [ ! -f $(VALIDATE_SCRIPT) ]; then \
		echo "Error: $(VALIDATE_SCRIPT) not found"; \
		exit 1; \
	fi
	@if [ ! -x $(VALIDATE_SCRIPT) ]; then \
		echo "Error: $(VALIDATE_SCRIPT) is not executable"; \
		exit 1; \
	fi
	bash $(VALIDATE_SCRIPT)

# Run validation tests in quiet mode (for CI)
# Uses --quiet flag supported by the validation script
test-quiet: strict
	@if [ ! -f $(VALIDATE_SCRIPT) ]; then \
		echo "Error: $(VALIDATE_SCRIPT) not found"; \
		exit 1; \
	fi
	@if [ ! -x $(VALIDATE_SCRIPT) ]; then \
		echo "Error: $(VALIDATE_SCRIPT) is not executable"; \
		exit 1; \
	fi
	bash $(VALIDATE_SCRIPT) --quiet

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TARGET_STRICT) $(TARGET_DEBUG) $(TARGET_OPTIMIZED) $(TARGET_CLANG) *.exe *.out *.o

# Help target
help:
	@echo "Available targets:"
	@echo "  all (default) - Build with basic warnings"
	@echo "  strict        - Build with strict flags (produces hello_strict for CI)"
	@echo "  debug         - Build with debug symbols (produces hello_debug)"
	@echo "  optimized     - Build with -O2 optimization (produces hello_optimized)"
	@echo "  clang         - Build with Clang compiler (produces hello_clang)"
	@echo "  test          - Run validation tests"
	@echo "  test-quiet    - Run tests with minimal output (CI)"
	@echo "  clean         - Remove build artifacts"
	@echo "  help          - Show this help message"
