# Makefile for Hello World C Project
# Provides compilation and testing targets for CI/CD integration

.PHONY: all clean test unit-test strict debug clang help

# Default target
all: hello

# Compilation targets
hello: hello.c
	gcc -Wall -Wextra -o hello hello.c

strict: hello.c
	gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -o hello_strict hello.c

debug: hello.c
	gcc -g -Wall -Wextra -o hello_debug hello.c

clang: hello.c
	clang -Wall -Wextra -o hello_clang hello.c

# Testing targets
unit-test:
	@echo "Running C unit tests..."
	@bash test/run_unit_tests.sh

test: strict
	@echo "Running all tests..."
	@bash test/validate.sh --quiet
	@bash test/test_editor_output.sh
	@node test/test_escape_html.js
	@node test/test_xss_integration.js
	@echo "All tests passed!"

# Clean up compiled binaries
clean:
	rm -f hello hello_strict hello_debug hello_clang hello_lib.o test_hello_runner *.o *.out

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build hello (default)"
	@echo "  hello      - Build with standard flags"
	@echo "  strict     - Build with strict compiler flags"
	@echo "  debug      - Build with debug symbols"
	@echo "  clang      - Build with clang compiler"
	@echo "  unit-test  - Run C unit tests"
	@echo "  test       - Run all tests (validation, editor, XSS)"
	@echo "  clean      - Remove compiled binaries"
	@echo "  help       - Show this help message"
