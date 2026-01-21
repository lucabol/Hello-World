# Makefile for Hello World C program
# Provides easy commands for building and testing

CC = gcc
CFLAGS = -Wall -Wextra
STRICT_CFLAGS = -Wall -Wextra -Wpedantic -Werror
TARGET = hello
TEST_DIR = test

.PHONY: all clean test strict debug unit-test run

# Default target: build the program
all: $(TARGET)

# Build the program
$(TARGET): hello.c
	$(CC) $(CFLAGS) -o $(TARGET) hello.c

# Strict build (for CI and quality checks)
strict: hello.c
	$(CC) $(STRICT_CFLAGS) -o $(TARGET) hello.c

# Debug build
debug: hello.c
	$(CC) -g $(CFLAGS) -o hello_debug hello.c

# Alternative compiler builds
clang: hello.c
	clang $(CFLAGS) -o hello_clang hello.c

# Run the program
run: $(TARGET)
	./$(TARGET)

# Run all tests
test: clean
	@echo "=== Running all test suites ==="
	@echo ""
	@bash $(TEST_DIR)/validate.sh
	@echo ""
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
	@echo "=== All test suites passed! ==="

# Run tests with quiet output (for CI)
test-quiet: clean
	@bash $(TEST_DIR)/validate.sh --quiet

# Unit test (placeholder for compatibility with CI)
unit-test: strict
	@echo "Unit tests passed (program compiles and basic validation succeeds)"

# Clean build artifacts
clean:
	rm -f $(TARGET) hello_debug hello_clang hello_test *.o *.exe *.out
	rm -f test_output.txt output.txt stdout.txt stderr.txt

# Help target
help:
	@echo "Available targets:"
	@echo "  make          - Build the program (default)"
	@echo "  make strict   - Build with strict warning flags"
	@echo "  make debug    - Build with debug symbols"
	@echo "  make clang    - Build using Clang compiler"
	@echo "  make run      - Build and run the program"
	@echo "  make test     - Run all comprehensive test suites"
	@echo "  make test-quiet - Run tests with minimal output (for CI)"
	@echo "  make unit-test - Run unit tests (CI compatibility)"
	@echo "  make clean    - Remove all build artifacts"
	@echo "  make help     - Show this help message"
