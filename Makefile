# Makefile for Hello World C program
# Provides standardized build targets for development and CI

# Default compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Werror
CLANG = clang
DEBUG_FLAGS = -g -Wall -Wextra
OPTIMIZED_FLAGS = -O2 -Wall -Wextra

# Target binary name
TARGET = hello
SOURCE = hello.c

# Default target - standard build
.PHONY: all
all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Strict compilation (matches CI validation)
.PHONY: strict
strict: hello_strict

hello_strict: $(SOURCE)
	$(CC) $(STRICT_FLAGS) -o hello_strict $(SOURCE)

# Clang build
.PHONY: clang
clang: hello_clang

hello_clang: $(SOURCE)
	$(CLANG) $(CFLAGS) -o hello_clang $(SOURCE)

# Debug build
.PHONY: debug
debug: hello_debug

hello_debug: $(SOURCE)
	$(CC) $(DEBUG_FLAGS) -o hello_debug $(SOURCE)

# Optimized build for performance comparison
.PHONY: optimized
optimized: hello_optimized

hello_optimized: $(SOURCE)
	$(CC) $(OPTIMIZED_FLAGS) -o hello_optimized $(SOURCE)

# Run validation tests (reproduce CI locally)
.PHONY: test
test:
	bash test/validate.sh

# Run validation tests in quiet mode (CI-style output)
.PHONY: test-quiet
test-quiet:
	bash test/validate.sh --quiet

# Clean compiled binaries
.PHONY: clean
clean:
	rm -f $(TARGET) hello_strict hello_clang hello_debug hello_optimized hello_test* voice_demo voice_demo_test *.exe *.out *.o

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all         - Build hello (default)"
	@echo "  strict      - Build with strict flags (-Werror)"
	@echo "  clang       - Build with Clang compiler"
	@echo "  debug       - Build with debug symbols"
	@echo "  optimized   - Build with -O2 optimization"
	@echo "  test        - Run validation tests"
	@echo "  test-quiet  - Run validation tests (quiet mode)"
	@echo "  clean       - Remove compiled binaries"
	@echo "  help        - Show this help message"