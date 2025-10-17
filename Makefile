# Makefile for Hello World C program
# Provides build, test, and size check targets

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
OPTIMIZE_FLAGS = -Os -s

# Output binaries
HELLO = hello
HELLO_DEBUG = hello_debug
HELLO_OPTIMIZED = hello_optimized
HELLO_STRICT = hello_strict
HELLO_CLANG = hello_clang

# Source files
SRC = hello.c

# Default target
.PHONY: all
all: $(HELLO)

# Standard build
$(HELLO): $(SRC)
	$(CC) $(CFLAGS) -o $(HELLO) $(SRC)

# Debug build
.PHONY: debug
debug:
	$(CC) -g $(CFLAGS) -o $(HELLO_DEBUG) $(SRC)

# Strict build (CI-like)
.PHONY: strict
strict:
	$(CC) $(STRICT_FLAGS) -o $(HELLO_STRICT) $(SRC)

# Clang build
.PHONY: clang
clang:
	clang $(CFLAGS) -o $(HELLO_CLANG) $(SRC)

# Optimized build for size checking
$(HELLO_OPTIMIZED): $(SRC)
	$(CC) $(OPTIMIZE_FLAGS) -o $(HELLO_OPTIMIZED) $(SRC)

# Size target: builds optimized binary and reports size
.PHONY: size
size: $(HELLO_OPTIMIZED)
	@echo "=== Binary Size Report ==="
	@echo "Optimized binary: $(HELLO_OPTIMIZED)"
	@echo -n "Size (bytes): "
	@stat -c%s $(HELLO_OPTIMIZED) 2>/dev/null || stat -f%z $(HELLO_OPTIMIZED) 2>/dev/null
	@echo ""
	@echo "Detailed size breakdown:"
	@size $(HELLO_OPTIMIZED) 2>/dev/null || echo "size command not available"
	@echo ""
	@echo "File info:"
	@ls -lh $(HELLO_OPTIMIZED)

# Unit test target
.PHONY: unit-test
unit-test:
	@bash test/run_unit_tests.sh

# Validation test target
.PHONY: test
test: strict
	@bash test/validate.sh

# Quiet test (CI-style)
.PHONY: test-quiet
test-quiet: strict
	@bash test/validate.sh --quiet

# Clean build artifacts
.PHONY: clean
clean:
	rm -f $(HELLO) $(HELLO_DEBUG) $(HELLO_OPTIMIZED) $(HELLO_STRICT) $(HELLO_CLANG)
	rm -f *.o *.out *.exe
	rm -f test_hello_runner hello_lib.o

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all          - Build standard binary (default)"
	@echo "  debug        - Build with debug symbols"
	@echo "  strict       - Build with strict warnings (CI-like)"
	@echo "  clang        - Build with clang compiler"
	@echo "  size         - Build optimized binary and report size"
	@echo "  unit-test    - Run unit tests"
	@echo "  test         - Run validation tests"
	@echo "  test-quiet   - Run validation tests (quiet mode)"
	@echo "  clean        - Remove build artifacts"
	@echo "  help         - Show this help message"
