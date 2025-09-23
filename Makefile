# Makefile for Hello World C program
# Standardizes build commands and provides consistent targets for development and CI

# Compiler and flags configuration
CC ?= gcc
CLANG ?= clang
CFLAGS ?= -Wall -Wextra -std=c99
OPTFLAGS = -O2
DEBUGFLAGS = -g
STRICT_FLAGS = -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror

# Source files and targets
SOURCE = hello.c
TARGET = hello
DEBUG_TARGET = hello_debug
CLANG_TARGET = hello_clang
STRICT_TARGET = hello_strict

# Default target (optimized build)
all: $(TARGET)

# Optimized build target
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(OPTFLAGS) -o $(TARGET) $(SOURCE)

# Debug build target
debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -o $(DEBUG_TARGET) $(SOURCE)

# Strict compilation target (with -Werror)
strict: $(STRICT_TARGET)

$(STRICT_TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(STRICT_FLAGS) -o $(STRICT_TARGET) $(SOURCE)

# Clang build target
clang: $(CLANG_TARGET)

$(CLANG_TARGET): $(SOURCE)
	$(CLANG) $(CFLAGS) $(OPTFLAGS) -o $(CLANG_TARGET) $(SOURCE)

# Run the default binary
run: $(TARGET)
	./$(TARGET)

# Test target: build strict and run validation script  
test: strict
	bash test/validate.sh

# Clean build artifacts
clean:
	rm -f $(TARGET) $(DEBUG_TARGET) $(CLANG_TARGET) $(STRICT_TARGET) *.exe *.out *.o

# Help target to show available commands
help:
	@echo "Available targets:"
	@echo "  all      - Build optimized hello (default)"
	@echo "  debug    - Build with debug flags (-g)"
	@echo "  strict   - Build with strict warnings and -Werror"
	@echo "  clang    - Build with clang compiler"
	@echo "  run      - Run the default binary"
	@echo "  test     - Build strict and run (for CI/validation)"
	@echo "  clean    - Remove all build artifacts"
	@echo "  help     - Show this help message"

# Declare phony targets
.PHONY: all debug strict clang run test clean help
