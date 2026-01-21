<<<<<<< HEAD
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
=======
# Makefile for Hello World and Code Metrics projects

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
CLANG = clang

# Targets
.PHONY: all clean test test-hello test-metrics strict debug help

all: hello code_metrics

hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

code_metrics: code_metrics.c
	$(CC) $(CFLAGS) -o code_metrics code_metrics.c

strict:
	$(CC) -Wall -Wextra -Wpedantic -Werror -o hello hello.c
	$(CC) -Wall -Wextra -Wpedantic -Werror -o code_metrics code_metrics.c

debug:
	$(CC) -Wall -Wextra -Wpedantic -g -o hello hello.c
	$(CC) -Wall -Wextra -Wpedantic -g -o code_metrics code_metrics.c

clang:
	$(CLANG) $(CFLAGS) -o hello hello.c
	$(CLANG) $(CFLAGS) -o code_metrics code_metrics.c

test: test-hello test-metrics

test-hello:
	@echo "Running hello.c validation tests..."
	@bash test/validate.sh

test-metrics:
	@echo "Running code_metrics.c validation tests..."
	@bash test/validate_code_metrics.sh

test-quiet:
	@bash test/validate.sh --quiet

clean:
	rm -f hello hello_debug hello_clang code_metrics *.exe *.out *.o

help:
	@echo "Available targets:"
	@echo "  all          - Build hello and code_metrics (default)"
	@echo "  hello        - Build hello program only"
	@echo "  code_metrics - Build code_metrics program only"
	@echo "  strict       - Build with -Werror flag"
	@echo "  debug        - Build with debug symbols"
	@echo "  clang        - Build using clang compiler"
	@echo "  test         - Run all test suites"
	@echo "  test-hello   - Run hello.c tests only"
	@echo "  test-metrics - Run code_metrics.c tests only"
	@echo "  test-quiet   - Run tests with minimal output"
	@echo "  clean        - Remove all build artifacts"
	@echo "  help         - Show this help message"
>>>>>>> main
