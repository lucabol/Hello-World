# Makefile for Hello World C Program
# Provides build automation for standard, development, and optimized builds

# Default goal
.DEFAULT_GOAL := all

# Compiler settings
CC = gcc
TARGET = hello
SOURCE = hello.c

# Base compiler flags (can be overridden with make CFLAGS=...)
CFLAGS ?= 

# Compiler flags for different build types
CFLAGS_STANDARD = $(CFLAGS)
CFLAGS_DEV = -Wall -Wextra $(CFLAGS)
CFLAGS_OPTIMIZED = -O2 -Wall -Wextra $(CFLAGS)
CFLAGS_STRICT = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -std=c11 $(CFLAGS)

# Default target (standard build)
all: $(TARGET)

# Standard build
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS_STANDARD) -o $(TARGET) $(SOURCE)

# Development build with warnings
dev: hello_dev
hello_dev: $(SOURCE)
	$(CC) $(CFLAGS_DEV) -o hello_dev $(SOURCE)

# Optimized build
optimized: hello_optimized
hello_optimized: $(SOURCE)
	$(CC) $(CFLAGS_OPTIMIZED) -o hello_optimized $(SOURCE)

# Strict compilation (maximum warnings)
strict: hello_strict
hello_strict: $(SOURCE)
	$(CC) $(CFLAGS_STRICT) -o hello_strict $(SOURCE)

# Debug build
debug: hello_debug
hello_debug: $(SOURCE)
	$(CC) -g $(CFLAGS_DEV) -o hello_debug $(SOURCE)

# Clean target - remove all compiled binaries
clean:
	$(RM) $(TARGET) hello_dev hello_optimized hello_strict hello_debug *.exe *.out *.o

# Test target - build and run the program
test: $(TARGET)
	@./$(TARGET) || exit 1

# Help target - show available targets
help:
	@echo "Available targets:"
	@echo "  all        - Standard build (default)"
	@echo "  dev        - Development build with warnings (-Wall -Wextra)"
	@echo "  optimized  - Optimized build (-O2 -Wall -Wextra)"
	@echo "  strict     - Strict compilation with maximum warnings and C11 standard"
	@echo "  debug      - Debug build with -g flag"
	@echo "  test       - Build and run the program"
	@echo "  clean      - Remove all compiled binaries"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Override compiler flags with: make CFLAGS=\"your flags\""

# Declare phony targets
.PHONY: all dev optimized strict debug clean test help
