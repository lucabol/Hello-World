# Makefile for Hello World C Program
# Provides convenience targets for building, testing, and benchmarking

# Compiler settings
CC = gcc
CLANG = clang
CFLAGS = -Wall -Wextra
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Werror

# Source and output
SRC = hello.c
TARGET = hello

# Default target
.PHONY: all
all: $(TARGET)

# Build with GCC
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Build with strict flags (CI-compatible)
.PHONY: strict
strict:
	$(CC) $(STRICT_FLAGS) -o $(TARGET) $(SRC)

# Build with Clang
.PHONY: clang
clang:
	$(CLANG) $(CFLAGS) -o hello_clang $(SRC)

# Build with debug symbols
.PHONY: debug
debug:
	$(CC) -g $(CFLAGS) -o hello_debug $(SRC)

# Run validation tests
.PHONY: test
test: strict
	@bash test/validate.sh

# Run validation tests quietly (CI-style)
.PHONY: test-quiet
test-quiet: strict
	@bash test/validate.sh --quiet

# Run benchmark suite with default settings (5 iterations)
.PHONY: benchmark
benchmark:
	@bash benchmark.sh $(SRC) 5 benchmark_results.json

# Run quick benchmark (3 iterations for faster results)
.PHONY: benchmark-quick
benchmark-quick:
	@bash benchmark.sh $(SRC) 3 benchmark_results.json

# Run extensive benchmark (10 iterations for more accurate results)
.PHONY: benchmark-extensive
benchmark-extensive:
	@bash benchmark.sh $(SRC) 10 benchmark_results.json

# Test the benchmark script itself
.PHONY: test-benchmark
test-benchmark:
	@bash test/test_benchmark.sh

# Clean build artifacts
.PHONY: clean
clean:
	rm -f $(TARGET) hello_debug hello_clang hello_strict
	rm -f *.o *.exe *.out
	rm -f benchmark_results.json

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all              - Build the program with GCC (default)"
	@echo "  strict           - Build with strict compiler flags"
	@echo "  clang            - Build with Clang compiler"
	@echo "  debug            - Build with debug symbols"
	@echo "  test             - Run validation tests"
	@echo "  test-quiet       - Run validation tests quietly"
	@echo "  benchmark        - Run performance benchmark (5 iterations)"
	@echo "  benchmark-quick  - Run quick benchmark (3 iterations)"
	@echo "  benchmark-extensive - Run extensive benchmark (10 iterations)"
	@echo "  test-benchmark   - Test the benchmark script"
	@echo "  clean            - Remove build artifacts"
	@echo "  help             - Show this help message"

# Unit test target (placeholder for CI compatibility)
.PHONY: unit-test
unit-test:
	@echo "Unit tests not yet implemented - this is a simple hello world program"
	@exit 0
