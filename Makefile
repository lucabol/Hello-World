# Makefile for Hello World C project
# Provides build, test, lint, and format targets

# Compiler settings
CC = gcc
CLANG = clang
CFLAGS = -Wall -Wextra -std=c99
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99

# Output binaries
HELLO = hello
HELLO_CLANG = hello_clang
HELLO_DEBUG = hello_debug
HELLO_LIB = hello_lib.o
TEST_RUNNER = test_hello_runner

# Source files
SOURCES = hello.c
TEST_SOURCES = test/test_hello.c
HEADERS = test/simple_test.h
ALL_C_FILES = $(SOURCES) $(TEST_SOURCES)

# Tools
CLANG_FORMAT = clang-format
CLANG_TIDY = clang-tidy

# Default target
.PHONY: all
all: $(HELLO)

# Build targets
$(HELLO): $(SOURCES)
	$(CC) $(CFLAGS) -o $(HELLO) $(SOURCES)

.PHONY: strict
strict: $(SOURCES)
	$(CC) $(STRICT_FLAGS) -o $(HELLO) $(SOURCES)

.PHONY: clang
clang: $(SOURCES)
	$(CLANG) $(CFLAGS) -o $(HELLO_CLANG) $(SOURCES)

.PHONY: debug
debug: $(SOURCES)
	$(CC) -g $(CFLAGS) -o $(HELLO_DEBUG) $(SOURCES)

# Test targets
.PHONY: test
test: strict
	./test/validate.sh

.PHONY: test-quiet
test-quiet: strict
	./test/validate.sh --quiet

.PHONY: unit-test
unit-test:
	@echo "Building unit tests..."
	$(CC) $(STRICT_FLAGS) -I. -c -o $(HELLO_LIB) $(SOURCES) -DUNIT_TEST
	$(CC) $(STRICT_FLAGS) -I. -o $(TEST_RUNNER) $(TEST_SOURCES) $(HELLO_LIB)
	@echo "Running unit tests..."
	./$(TEST_RUNNER)
	@rm -f $(HELLO_LIB) $(TEST_RUNNER)

# Lint and format targets
.PHONY: format
format:
	@echo "Formatting C source files..."
	$(CLANG_FORMAT) -i $(ALL_C_FILES) $(HEADERS)
	@echo "Formatting complete!"

.PHONY: format-check
format-check:
	@echo "Checking code formatting..."
	@$(CLANG_FORMAT) --dry-run --Werror $(ALL_C_FILES) $(HEADERS)
	@echo "Format check passed!"

.PHONY: lint
lint: format-check
	@echo "Running static analysis with clang-tidy..."
	@$(CLANG_TIDY) $(SOURCES) -- $(CFLAGS) -I.
	@echo "Lint check passed!"

# Clean targets
.PHONY: clean
clean:
	rm -f $(HELLO) $(HELLO_CLANG) $(HELLO_DEBUG) $(HELLO_LIB) $(TEST_RUNNER)
	rm -f *.o *.exe *.out hello_strict

.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all          - Build the hello program (default)"
	@echo "  strict       - Build with strict warning flags"
	@echo "  clang        - Build with Clang compiler"
	@echo "  debug        - Build with debug symbols"
	@echo "  test         - Run validation tests"
	@echo "  test-quiet   - Run validation tests (quiet mode)"
	@echo "  unit-test    - Run unit tests"
	@echo "  format       - Auto-format all C source files"
	@echo "  format-check - Check if files are properly formatted"
	@echo "  lint         - Run format check and static analysis"
	@echo "  clean        - Remove built binaries"
	@echo "  help         - Show this help message"
