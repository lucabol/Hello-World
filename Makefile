# Makefile for Hello World C Program
# Provides build automation for standard, development, and optimized builds

# Compiler settings
CC = gcc
TARGET = hello
SOURCE = hello.c

# Compiler flags for different build types
CFLAGS_STANDARD = 
CFLAGS_DEV = -Wall -Wextra
CFLAGS_OPTIMIZED = -O2 -Wall -Wextra
CFLAGS_STRICT = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion

# Default target (development build with warnings enabled)
all: dev

# Standard build (minimal flags)
standard: $(TARGET)
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS_STANDARD) -o $(TARGET) $(SOURCE)

# Development build with warnings (now the default)
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
	rm -f $(TARGET) hello_dev hello_optimized hello_strict hello_debug hello_clang *.exe *.out *.o

# Test target - build and run the program using dev target
test: dev
	./hello_dev

# Lint target - run strict compilation to check for warnings
lint: strict
	@echo "Code compiled successfully with strict warnings enabled"

# Help target - show available targets
help:
	@echo "Available targets:"
	@echo "  all        - Development build with warnings (default)"
	@echo "  dev        - Development build with warnings (-Wall -Wextra)"
	@echo "  standard   - Standard build with minimal flags"
	@echo "  optimized  - Optimized build (-O2 -Wall -Wextra)"
	@echo "  strict     - Strict compilation with maximum warnings"
	@echo "  debug      - Debug build with -g flag"
	@echo "  test       - Build and run the program"
	@echo "  lint       - Check code with strict compilation flags"
	@echo "  clean      - Remove all compiled binaries"
	@echo "  help       - Show this help message"

# Declare phony targets
.PHONY: all dev standard optimized strict debug clean test lint help