# Makefile for Hello World and Code Metrics with Performance Benchmarking

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
CLANG = clang

# Targets
.PHONY: all clean test test-hello test-metrics test-benchmarks strict debug clang benchmark benchmark-update-baseline benchmark-compare validate unit-test help

# Default target - build both programs
all: hello code_metrics

# Build hello program
hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

# Build code_metrics program
code_metrics: code_metrics.c
	$(CC) $(CFLAGS) -o code_metrics code_metrics.c

# Build with strict flags (CI-like)
strict:
	$(CC) -Wall -Wextra -Wpedantic -Werror -o hello hello.c
	$(CC) -Wall -Wextra -Wpedantic -Werror -o code_metrics code_metrics.c

# Build with debug symbols
debug:
	$(CC) -Wall -Wextra -Wpedantic -g -o hello hello.c
	$(CC) -Wall -Wextra -Wpedantic -g -o code_metrics code_metrics.c

# Build with Clang
clang:
	$(CLANG) $(CFLAGS) -o hello hello.c
	$(CLANG) $(CFLAGS) -o code_metrics code_metrics.c

# Run all tests
test: test-hello test-metrics

# Run hello.c validation tests
test-hello:
	@echo "Running hello.c validation tests..."
	@bash test/validate.sh

# Run code_metrics.c validation tests
test-metrics:
	@echo "Running code_metrics.c validation tests..."
	@bash test/validate_code_metrics.sh

# Run validation tests (quiet mode)
test-quiet:
	@bash test/validate.sh --quiet

# Run validation tests (alias)
validate:
	bash test/validate.sh

# Run unit tests (stub for compatibility)
unit-test:
	@echo "No unit tests defined yet"

# Run performance benchmarks
benchmark:
	bash benchmarks/run_benchmarks.sh

# Run benchmarks and update baseline
benchmark-update-baseline:
	bash benchmarks/run_benchmarks.sh --update-baseline

# Run benchmark comparison
benchmark-compare:
	python3 benchmarks/compare_benchmarks.py

# Run benchmark accuracy tests
test-benchmarks:
	bash test/test_benchmarks.sh

# Clean compiled binaries
clean:
	rm -f hello hello_debug hello_clang hello_optimized hello_strict hello_warnings hello_test* hello_bench_* code_metrics
	rm -f *.o *.exe *.out
	rm -f benchmarks/benchmark_results.json

# Help target
help:
	@echo "Available targets:"
	@echo "  all                - Build hello and code_metrics (default)"
	@echo "  hello              - Build hello program only"
	@echo "  code_metrics       - Build code_metrics program only"
	@echo "  strict             - Build with -Werror flag"
	@echo "  debug              - Build with debug symbols"
	@echo "  clang              - Build using clang compiler"
	@echo "  test               - Run all test suites"
	@echo "  test-hello         - Run hello.c tests only"
	@echo "  test-metrics       - Run code_metrics.c tests only"
	@echo "  test-quiet         - Run tests with minimal output"
	@echo "  validate           - Run validation tests"
	@echo "  unit-test          - Run unit tests (stub)"
	@echo "  benchmark          - Run performance benchmarks"
	@echo "  benchmark-update-baseline - Update benchmark baseline"
	@echo "  benchmark-compare  - Compare benchmarks with baseline"
	@echo "  test-benchmarks    - Run benchmark accuracy tests"
	@echo "  clean              - Remove all build artifacts"
	@echo "  help               - Show this help message"
