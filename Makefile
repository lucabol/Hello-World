# Makefile for Hello World C program and Code Metrics tool
# Supports multiple build targets and configurations

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
DEBUG_FLAGS = -g -Wall -Wextra -std=c99
OPTIMIZED_FLAGS = -O2 -Wall -Wextra -std=c99

# Targets
HELLO = hello
METRICS = code_metrics
STRICT = hello_strict
DEBUG = hello_debug
OPTIMIZED = hello_optimized

# Source files
HELLO_SRC = hello.c
METRICS_SRC = code_metrics.c

# Default target - build both hello and code_metrics
all: $(HELLO) $(METRICS)

# Build hello program
$(HELLO): $(HELLO_SRC)
	$(CC) $(CFLAGS) -o $(HELLO) $(HELLO_SRC)

# Build code metrics tool
$(METRICS): $(METRICS_SRC)
	$(CC) $(CFLAGS) -o $(METRICS) $(METRICS_SRC)

# Build with strict flags (for CI validation)
strict: $(HELLO_SRC)
	$(CC) $(STRICT_FLAGS) -o $(STRICT) $(HELLO_SRC)

# Build debug version
debug: $(HELLO_SRC)
	$(CC) $(DEBUG_FLAGS) -o $(DEBUG) $(HELLO_SRC)

# Build optimized version
optimized: $(HELLO_SRC)
	$(CC) $(OPTIMIZED_FLAGS) -o $(OPTIMIZED) $(HELLO_SRC)

# Build with clang (alternative compiler)
clang: $(HELLO_SRC)
	clang -o hello_clang $(HELLO_SRC)

# Run validation tests
test: strict
	./test/validate.sh

# Run in quiet mode (for CI)
test-quiet: strict
	./test/validate.sh --quiet

# Run the metrics tool on hello.c
metrics: $(METRICS)
	./$(METRICS)

# Clean all build artifacts
clean:
	rm -f $(HELLO) $(METRICS) $(STRICT) $(DEBUG) $(OPTIMIZED) hello_clang
	rm -f *.o *.exe *.out
	rm -f test/*.o test/*.exe test/*.out test_hello_runner hello_lib.o

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build hello and code_metrics (default)"
	@echo "  hello      - Build hello program"
	@echo "  code_metrics - Build code metrics tool"
	@echo "  strict     - Build with strict compilation flags"
	@echo "  debug      - Build debug version"
	@echo "  optimized  - Build optimized version"
	@echo "  clang      - Build with clang compiler"
	@echo "  test       - Run validation tests"
	@echo "  test-quiet - Run tests in quiet mode (CI)"
	@echo "  metrics    - Run code metrics analyzer on hello.c"
	@echo "  clean      - Remove all build artifacts"
	@echo "  help       - Show this help message"

.PHONY: all strict debug optimized clang test test-quiet metrics clean help
