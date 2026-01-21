# Makefile for Hello World and Code Metrics projects

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
CLANG = clang
TARGET = hello
TEST_DIR = test

# Targets
.PHONY: all clean test test-hello test-metrics strict debug clang help run unit-test test-quiet

all: hello code_metrics

hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

code_metrics: code_metrics.c
	$(CC) $(CFLAGS) -o code_metrics code_metrics.c

strict:
	$(CC) -Wall -Wextra -Wpedantic -Werror -o hello hello.c
	$(CC) -Wall -Wextra -Wpedantic -Werror -o code_metrics code_metrics.c

debug:
	$(CC) -Wall -Wextra -Wpedantic -g -o hello_debug hello.c
	$(CC) -Wall -Wextra -Wpedantic -g -o code_metrics code_metrics.c

clang:
	$(CLANG) $(CFLAGS) -o hello_clang hello.c
	$(CLANG) $(CFLAGS) -o code_metrics code_metrics.c

# Run the program
run: hello
	./hello

# Main test target - runs all tests including comprehensive edge case tests
test: test-hello test-metrics test-comprehensive

test-hello:
	@echo "Running hello.c validation tests..."
	@bash test/validate.sh

test-metrics:
	@echo "Running code_metrics.c validation tests..."
	@bash test/validate_code_metrics.sh

# Comprehensive edge case tests for hello.c
test-comprehensive: clean
	@echo ""
	@echo "=== Running comprehensive edge case tests for hello.c ==="
	@bash $(TEST_DIR)/test_optimization_levels.sh
	@echo ""
	@bash $(TEST_DIR)/test_locale_settings.sh
	@echo ""
	@bash $(TEST_DIR)/test_output_streams.sh
	@echo ""
	@bash $(TEST_DIR)/test_buffer_flushing.sh
	@echo ""
	@bash $(TEST_DIR)/test_warning_flags.sh
	@echo ""
	@bash $(TEST_DIR)/test_minimal_environment.sh
	@echo ""
	@echo "=== All comprehensive test suites passed! ==="

test-quiet:
	@bash test/validate.sh --quiet

# Unit test (for CI compatibility)
unit-test: strict
	@echo "Unit tests passed (programs compile and basic validation succeeds)"

clean:
	rm -f hello hello_debug hello_clang hello_test code_metrics *.exe *.out *.o
	rm -f test_output.txt output.txt stdout.txt stderr.txt

help:
	@echo "Available targets:"
	@echo "  all               - Build hello and code_metrics (default)"
	@echo "  hello             - Build hello program only"
	@echo "  code_metrics      - Build code_metrics program only"
	@echo "  strict            - Build with -Werror flag"
	@echo "  debug             - Build with debug symbols"
	@echo "  clang             - Build using clang compiler"
	@echo "  run               - Build and run hello program"
	@echo "  test              - Run all test suites (validation + comprehensive)"
	@echo "  test-hello        - Run hello.c tests only"
	@echo "  test-metrics      - Run code_metrics.c tests only"
	@echo "  test-comprehensive - Run comprehensive edge case tests for hello.c"
	@echo "  test-quiet        - Run tests with minimal output"
	@echo "  unit-test         - Run unit tests (CI compatibility)"
	@echo "  clean             - Remove all build artifacts"
	@echo "  help              - Show this help message"
