# Makefile for Hello World C program

.PHONY: all clean test benchmark benchmark-update-baseline validate unit-test strict debug clang help

# Default target
all: hello

# Build the program
hello: hello.c
	gcc -Wall -Wextra -o hello hello.c

# Build with strict flags (CI-like)
strict: hello.c
	gcc -Wall -Wextra -Wpedantic -Werror -o hello hello.c

# Build with debug symbols
debug: hello.c
	gcc -g -Wall -Wextra -o hello_debug hello.c

# Build with Clang
clang: hello.c
	clang -Wall -Wextra -o hello_clang hello.c

# Run validation tests
validate:
	bash test/validate.sh

# Run validation tests (quiet mode)
test-quiet:
	bash test/validate.sh --quiet

# Run all tests
test: validate
	@echo "All tests passed!"

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
	rm -f hello hello_debug hello_clang hello_optimized hello_strict hello_warnings hello_test* hello_bench_*
	rm -f *.o *.exe *.out
	rm -f benchmarks/benchmark_results.json

# Help target
help:
	@echo "Available targets:"
	@echo "  make              - Build the program (default)"
	@echo "  make strict       - Build with strict warnings (-Werror)"
	@echo "  make debug        - Build with debug symbols"
	@echo "  make clang        - Build with Clang compiler"
	@echo "  make test         - Run validation tests"
	@echo "  make test-quiet   - Run validation tests (quiet mode)"
	@echo "  make validate     - Run validation tests"
	@echo "  make benchmark    - Run performance benchmarks"
	@echo "  make benchmark-update-baseline - Update benchmark baseline"
	@echo "  make benchmark-compare - Compare benchmarks with baseline"
	@echo "  make test-benchmarks - Run benchmark accuracy tests"
	@echo "  make clean        - Remove compiled binaries"
	@echo "  make help         - Show this help message"
