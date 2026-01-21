# Makefile for Hello World and Code Metrics projects
# Provides convenience targets for building, testing, and benchmarking

# Compiler settings
CC = gcc
CLANG = clang
CFLAGS = -Wall -Wextra -Wpedantic
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Werror

# Source and output files
HELLO_SRC = hello.c
HELLO_TARGET = hello
METRICS_SRC = code_metrics.c
METRICS_TARGET = code_metrics

# Default target - build both programs
.PHONY: all
all: $(HELLO_TARGET) $(METRICS_TARGET)

# Build hello with GCC
$(HELLO_TARGET): $(HELLO_SRC)
	$(CC) $(CFLAGS) -o $(HELLO_TARGET) $(HELLO_SRC)

# Build code_metrics with GCC
$(METRICS_TARGET): $(METRICS_SRC)
	$(CC) $(CFLAGS) -o $(METRICS_TARGET) $(METRICS_SRC)

# Build with strict flags (CI-compatible)
.PHONY: strict
strict:
	$(CC) $(STRICT_FLAGS) -o $(HELLO_TARGET) $(HELLO_SRC)
	$(CC) $(STRICT_FLAGS) -o $(METRICS_TARGET) $(METRICS_SRC)

# Build with Clang
.PHONY: clang
clang:
	$(CLANG) $(CFLAGS) -o hello_clang $(HELLO_SRC)
	$(CLANG) $(CFLAGS) -o code_metrics_clang $(METRICS_SRC)

# Build with debug symbols
.PHONY: debug
debug:
	$(CC) $(CFLAGS) -g -o hello_debug $(HELLO_SRC)
	$(CC) $(CFLAGS) -g -o code_metrics_debug $(METRICS_SRC)

# Run all tests
.PHONY: test
test: test-hello test-metrics

# Run hello.c validation tests
.PHONY: test-hello
test-hello:
	@echo "Running hello.c validation tests..."
	@bash test/validate.sh

# Run code_metrics validation tests
.PHONY: test-metrics
test-metrics:
	@echo "Running code_metrics.c validation tests..."
	@bash test/validate_code_metrics.sh

# Run validation tests quietly (CI-style)
.PHONY: test-quiet
test-quiet:
	@bash test/validate.sh --quiet

# Run benchmark suite with default settings (5 iterations)
.PHONY: benchmark
benchmark:
	@bash benchmark.sh $(HELLO_SRC) 5 benchmark_results.json

# Run quick benchmark (3 iterations for faster results)
.PHONY: benchmark-quick
benchmark-quick:
	@bash benchmark.sh $(HELLO_SRC) 3 benchmark_results.json

# Run extensive benchmark (10 iterations for more accurate results)
.PHONY: benchmark-extensive
benchmark-extensive:
	@bash benchmark.sh $(HELLO_SRC) 10 benchmark_results.json

# Test the benchmark script itself
.PHONY: test-benchmark
test-benchmark:
	@bash test/test_benchmark.sh

# Clean build artifacts
.PHONY: clean
clean:
	rm -f $(HELLO_TARGET) hello_debug hello_clang hello_strict
	rm -f $(METRICS_TARGET) code_metrics_debug code_metrics_clang
	rm -f *.o *.exe *.out
	rm -f benchmark_results.json

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all              - Build hello and code_metrics (default)"
	@echo "  hello            - Build hello program only"
	@echo "  code_metrics     - Build code_metrics program only"
	@echo "  strict           - Build with strict compiler flags"
	@echo "  clang            - Build with Clang compiler"
	@echo "  debug            - Build with debug symbols"
	@echo "  test             - Run all test suites"
	@echo "  test-hello       - Run hello.c tests only"
	@echo "  test-metrics     - Run code_metrics.c tests only"
	@echo "  test-quiet       - Run tests with minimal output"
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
