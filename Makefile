# Makefile for Hello World C program
# Provides build targets for compilation, testing, and validation

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
SOURCES = hello.c
MAIN_TARGET = hello
TEST_TARGET = test/unit_test
TEST_SOURCES = test/unit_test.c

# Default target
all: $(MAIN_TARGET)

# Build the main hello program
$(MAIN_TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(MAIN_TARGET) $(SOURCES)

# Build with strict compilation flags (for CI)
strict: $(SOURCES)
	$(CC) $(STRICT_FLAGS) -o $(MAIN_TARGET) $(SOURCES)

# Build with debug flags
debug: $(SOURCES)
	$(CC) $(CFLAGS) -g -o hello_debug $(SOURCES)

# Build with clang (alternative compiler)
clang: $(SOURCES)
	clang $(CFLAGS) -o hello_clang $(SOURCES)

# Build unit test program
$(TEST_TARGET): $(TEST_SOURCES) $(MAIN_TARGET)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_SOURCES)

# Run unit tests (C-based automated tests)
unit-test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Run validation script (shell-based tests)
validate: $(MAIN_TARGET)
	./test/validate.sh

# Run all tests (both unit tests and validation)
test: unit-test validate

# Run tests quietly (for CI environments)
test-quiet: $(MAIN_TARGET) $(TEST_TARGET)
	./$(TEST_TARGET)
	./test/validate.sh --quiet

# Clean build artifacts
clean:
	rm -f $(MAIN_TARGET) hello_debug hello_clang hello_strict $(TEST_TARGET) *.exe *.out *.o

# Install dependencies (none needed for this simple project)
install:
	@echo "No dependencies to install for this simple C project"

# Help target
help:
	@echo "Available targets:"
	@echo "  all         - Build the main program (default)"
	@echo "  hello       - Build the main program"
	@echo "  strict      - Build with strict compilation flags"
	@echo "  debug       - Build with debug flags"
	@echo "  clang       - Build with clang compiler"
	@echo "  unit-test   - Run C-based unit tests"
	@echo "  validate    - Run shell-based validation script"
	@echo "  test        - Run all tests (unit tests + validation)"
	@echo "  test-quiet  - Run all tests with minimal output (CI-friendly)"
	@echo "  clean       - Remove build artifacts"
	@echo "  help        - Show this help message"

.PHONY: all strict debug clang unit-test validate test test-quiet clean install help