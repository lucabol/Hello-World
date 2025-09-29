# Makefile for Hello World C program and code metrics tool
# Supports multiple build targets with different optimization levels

CC=gcc
CFLAGS=-Wall -Wextra
STRICT_FLAGS=-Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
DEBUG_FLAGS=-g -Wall -Wextra
OPTIMIZE_FLAGS=-O2 -Wall -Wextra

# Main targets
.PHONY: all clean test test-quiet strict debug optimized clang help metrics-test

# Default target
all: hello metrics

# Primary hello world program
hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

# Code metrics analyzer
metrics: metrics.c
	$(CC) $(CFLAGS) -o metrics metrics.c

# Strict compilation (CI-style with -Werror)
strict: hello.c
	$(CC) $(STRICT_FLAGS) -o hello_strict hello.c

# Debug build
debug: hello.c
	$(CC) $(DEBUG_FLAGS) -o hello_debug hello.c

# Optimized build
optimized: hello.c
	$(CC) $(OPTIMIZE_FLAGS) -o hello_optimized hello.c

# Build with clang (alternative compiler)
clang: hello.c
	clang -o hello_clang hello.c

# Metrics tool with strict flags
metrics-strict: metrics.c
	$(CC) $(STRICT_FLAGS) -o metrics_strict metrics.c

# Test target - runs validation script
test:
	./test/validate.sh

# Quiet test for CI environments
test-quiet:
	./test/validate.sh --quiet

# Test metrics tool functionality
metrics-test: metrics
	@echo "Testing metrics tool functionality..."
	@./metrics hello.c
	@echo
	@echo "Testing CSV output:"
	@./metrics --csv hello.c
	@echo
	@echo "Testing summary output:"
	@./metrics --summary hello.c

# Clean build artifacts
clean:
	rm -f hello hello_debug hello_optimized hello_strict hello_clang hello_warnings hello_test*
	rm -f metrics metrics_strict
	rm -f voice_demo voice_demo_test
	rm -f *.exe *.out *.o

# Help target
help:
	@echo "Available targets:"
	@echo "  all           - Build hello and metrics programs (default)"
	@echo "  hello         - Build basic hello world program"
	@echo "  metrics       - Build code metrics analyzer"
	@echo "  strict        - Build with strict compilation flags (-Werror)"
	@echo "  debug         - Build with debug symbols"
	@echo "  optimized     - Build with optimization (-O2)"
	@echo "  clang         - Build with clang compiler"
	@echo "  test          - Run validation tests"
	@echo "  test-quiet    - Run validation tests (quiet mode)"
	@echo "  metrics-test  - Test metrics tool functionality"
	@echo "  clean         - Remove all build artifacts"
	@echo "  help          - Show this help message"
	@echo
	@echo "Usage examples:"
	@echo "  make                    # Build both hello and metrics"
	@echo "  make test               # Run all tests"
	@echo "  make metrics-test       # Test metrics functionality"
	@echo "  make metrics && ./metrics --help    # Build and show metrics help"