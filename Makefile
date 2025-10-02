# Makefile for Hello World C program

# Compiler settings
CC = gcc
CLANG = clang
CFLAGS = -std=c99
WARNINGS = -Wall -Wextra
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Werror
DEBUG_FLAGS = -g -Wall -Wextra
OPTIMIZE_FLAGS = -O2 -Wall -Wextra

# Source and output files
SRC = hello.c
TARGET = hello
TARGET_DEBUG = hello_debug
TARGET_CLANG = hello_clang
TARGET_STRICT = hello_strict

# Default target
all: $(TARGET)

# Basic build
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Strict build with all warnings treated as errors and pedantic mode enabled
# Includes -Wpedantic to catch language standard violations and -Werror to treat warnings as errors
strict: $(SRC)
	$(CC) $(CFLAGS) $(STRICT_FLAGS) -o $(TARGET) $(SRC)

# Debug build
debug: $(SRC)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $(TARGET_DEBUG) $(SRC)

# Optimized build
optimized: $(SRC)
	$(CC) $(CFLAGS) $(OPTIMIZE_FLAGS) -o $(TARGET) $(SRC)

# Build with Clang
clang: $(SRC)
	$(CLANG) $(CFLAGS) $(WARNINGS) -o $(TARGET_CLANG) $(SRC)

# Run validation tests
test:
	bash test/validate.sh

# Run validation tests in quiet mode (for CI)
test-quiet:
	bash test/validate.sh --quiet

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TARGET_DEBUG) $(TARGET_CLANG) $(TARGET_STRICT) *.exe *.out *.o

# Phony targets
.PHONY: all strict debug optimized clang test test-quiet clean
