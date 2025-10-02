# Makefile for Hello World C program
# Automates build targets and testing workflow

# Compiler settings
CC = gcc
CLANG = clang
CFLAGS = -Wall -Wextra
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
DEBUG_FLAGS = -g -Wall -Wextra
OPTIMIZE_FLAGS = -O2 -Wall -Wextra

# Source and target files
SRC = hello.c
TARGET = hello
TARGET_DEBUG = hello_debug
TARGET_STRICT = hello_strict
TARGET_CLANG = hello_clang
TARGET_OPTIMIZE = hello_optimized

# Default target
.PHONY: all
all: $(TARGET)

# Standard build
$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC)

# Strict build (CI-compatible with strict warnings and -Werror)
.PHONY: strict
strict: $(SRC)
	$(CC) $(STRICT_FLAGS) -o $(TARGET_STRICT) $(SRC)

# Debug build
.PHONY: debug
debug: $(SRC)
	$(CC) $(DEBUG_FLAGS) -o $(TARGET_DEBUG) $(SRC)

# Clang build
.PHONY: clang
clang: $(SRC)
	$(CLANG) -o $(TARGET_CLANG) $(SRC)

# Optimized build
.PHONY: optimized
optimized: $(SRC)
	$(CC) $(OPTIMIZE_FLAGS) -o $(TARGET_OPTIMIZE) $(SRC)

# Clean build artifacts
.PHONY: clean
clean:
	rm -f $(TARGET) $(TARGET_DEBUG) $(TARGET_STRICT) $(TARGET_CLANG) $(TARGET_OPTIMIZE)
	rm -f *.exe *.out *.o

# Run validation tests
.PHONY: test
test: strict
	./test/validate.sh

# Run tests with quiet output (CI-style)
.PHONY: test-quiet
test-quiet: strict
	./test/validate.sh --quiet

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all        - Build the program (default)"
	@echo "  strict     - Build with strict compilation flags (CI-compatible)"
	@echo "  debug      - Build with debug symbols"
	@echo "  clang      - Build using Clang compiler"
	@echo "  optimized  - Build with optimization flags"
	@echo "  clean      - Remove all compiled binaries"
	@echo "  test       - Run validation tests"
	@echo "  test-quiet - Run validation tests with minimal output"
	@echo "  help       - Show this help message"
