# Makefile for Hello World C program
# Simple and POSIX compatible

# Variables - can be overridden (e.g., CC=clang make)
CC = gcc
CFLAGS = -O2 -Wall -Wextra
DEBUGFLAGS = -g -Wall -Wextra
STRICTFLAGS = -Wall -Wextra -Wpedantic -Werror -Wformat=2 -Wconversion -Wsign-conversion
SOURCE = hello.c
TARGET = hello
DEBUG_TARGET = hello_debug
STRICT_TARGET = hello_strict
CLANG_TARGET = hello_clang

# Default target - optimized build
all: $(TARGET)

# Optimized build
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Debug build
debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(SOURCE)
	$(CC) $(DEBUGFLAGS) -o $(DEBUG_TARGET) $(SOURCE)

# Strict build with warnings as errors
strict: $(STRICT_TARGET)

$(STRICT_TARGET): $(SOURCE)
	$(CC) $(STRICTFLAGS) -o $(STRICT_TARGET) $(SOURCE)

# Clang build
clang: $(CLANG_TARGET)

$(CLANG_TARGET): $(SOURCE)
	clang $(CFLAGS) -o $(CLANG_TARGET) $(SOURCE)

# Build and run
run: $(TARGET)
	./$(TARGET)

# Test - build and run, verify successful execution
test: $(TARGET)
	./$(TARGET)
	@echo "Test passed: program executed successfully"

# Clean build artifacts (matches .gitignore patterns)
clean:
	rm -f $(TARGET) $(DEBUG_TARGET) $(STRICT_TARGET) $(CLANG_TARGET)
	rm -f hello_warnings hello_optimized hello_test*
	rm -f *.exe *.out *.o *.obj

# Phony targets
.PHONY: all debug strict clang run test clean