# Makefile for Hello World C program
# Provides standardized build targets for development and testing

# Compiler settings
CC := gcc
CFLAGS := -Wall -Wextra
STRICT_FLAGS := -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
OPTFLAGS := -O2

# Target binaries
TARGET := hello
DEBUG_TARGET := hello_debug
CLANG_TARGET := hello_clang

# Source files
SRC := hello.c
HEADERS := hello.h

# Default target - standard build
all: $(TARGET)

# Standard build
$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Strict build with all warnings as errors (matches CI)
strict: $(SRC) $(HEADERS)
	$(CC) $(STRICT_FLAGS) -o $(TARGET) $(SRC)

# Debug build with symbols
debug: $(SRC) $(HEADERS)
	$(CC) -g $(CFLAGS) -o $(DEBUG_TARGET) $(SRC)

# Optimized release build
release: $(SRC) $(HEADERS)
	$(CC) $(OPTFLAGS) $(CFLAGS) -o $(TARGET) $(SRC)

# Alternative compiler build (Clang)
clang: $(SRC) $(HEADERS)
	clang $(CFLAGS) -o $(CLANG_TARGET) $(SRC)

# Run unit tests
unit-test:
	@bash test/run_unit_tests.sh

# Run all tests (unit tests + validation)
test: unit-test
	@bash test/validate.sh

# Quiet test mode (CI-style)
test-quiet:
	@bash test/run_unit_tests.sh
	@bash test/validate.sh --quiet

# Clean build artifacts
clean:
	rm -f $(TARGET) $(DEBUG_TARGET) $(CLANG_TARGET)
	rm -f hello_strict hello_optimized hello_warnings
	rm -f test_hello_runner test_framework_compile
	rm -f *.o *.exe *.out
	rm -f test/*.o test/*.exe test/*.out

# Help target
help:
	@echo "Hello World C Program Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all          - Build the program (default)"
	@echo "  strict       - Build with strict flags (-Werror, matches CI)"
	@echo "  debug        - Build with debug symbols"
	@echo "  release      - Build optimized release version"
	@echo "  clang        - Build with Clang compiler"
	@echo "  unit-test    - Run unit tests only"
	@echo "  test         - Run all tests (unit + validation)"
	@echo "  test-quiet   - Run tests in quiet mode (for CI)"
	@echo "  clean        - Remove all build artifacts"
	@echo "  help         - Show this help message"

.PHONY: all strict debug release clang unit-test test test-quiet clean help
