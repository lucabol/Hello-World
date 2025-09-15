# Makefile for Hello World C Program
# Consolidates multiple compilation commands into a single interface

# Compiler and flags
CC = gcc
CLANG = clang
CFLAGS_BASE = 
CFLAGS_WARNINGS = -Wall -Wextra
CFLAGS_DEBUG = -g -Wall -Wextra
CFLAGS_OPTIMIZED = -O2 -Wall -Wextra
CFLAGS_STRICT = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion

# Source and target
SRC = hello.c
TARGET = hello

# Default target (basic build)
all: $(TARGET)

# Basic build
$(TARGET): $(SRC)
	$(CC) $(CFLAGS_BASE) -o $(TARGET) $(SRC)

# Development build with warnings (recommended)
dev: $(SRC)
	$(CC) $(CFLAGS_WARNINGS) -o $(TARGET) $(SRC)

# Debug build
debug: $(SRC)
	$(CC) $(CFLAGS_DEBUG) -o hello_debug $(SRC)

# Optimized build  
optimized: $(SRC)
	$(CC) $(CFLAGS_OPTIMIZED) -o $(TARGET) $(SRC)

# Strict compilation
strict: $(SRC)
	$(CC) $(CFLAGS_STRICT) -o $(TARGET) $(SRC)

# Clang build
clang: $(SRC)
	$(CLANG) -o hello_clang $(SRC)

# Test build and run
test: dev
	./$(TARGET)

# Clean compiled files
clean:
	rm -f $(TARGET) hello_debug hello_clang *.exe *.out *.o

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Basic build (default)"
	@echo "  dev       - Development build with warnings (recommended)"
	@echo "  debug     - Debug build with symbols"
	@echo "  optimized - Optimized build for production"
	@echo "  strict    - Strict compilation with all warnings"
	@echo "  clang     - Build using Clang compiler"
	@echo "  test      - Build and run the program"
	@echo "  clean     - Remove compiled files"
	@echo "  help      - Show this help message"

.PHONY: all dev debug optimized strict clang test clean help