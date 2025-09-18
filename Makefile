# Makefile for Hello World C Program
# Provides build automation for standard, development, and optimized builds

# Compiler settings
CC = gcc
CLANG = clang
TARGET = hello
SOURCE = hello.c

# Compiler flags for different build types
CFLAGS_STANDARD = 
CFLAGS_DEV = -Wall -Wextra
CFLAGS_OPTIMIZED = -O2 -Wall -Wextra
CFLAGS_STRICT = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion

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

# Clang builds - parallel to GCC builds
clang: hello_clang
hello_clang: $(SOURCE)
	$(CLANG) $(CFLAGS_STANDARD) -o hello_clang $(SOURCE)

# Clang development build with warnings
clang-dev: hello_clang_dev
hello_clang_dev: $(SOURCE)
	$(CLANG) $(CFLAGS_DEV) -o hello_clang_dev $(SOURCE)

# Clang optimized build
clang-optimized: hello_clang_optimized
hello_clang_optimized: $(SOURCE)
	$(CLANG) $(CFLAGS_OPTIMIZED) -o hello_clang_optimized $(SOURCE)

# Clang strict compilation (maximum warnings)
clang-strict: hello_clang_strict
hello_clang_strict: $(SOURCE)
	$(CLANG) $(CFLAGS_STRICT) -o hello_clang_strict $(SOURCE)

# Clang debug build
clang-debug: hello_clang_debug
hello_clang_debug: $(SOURCE)
	$(CLANG) -g $(CFLAGS_DEV) -o hello_clang_debug $(SOURCE)

# Clean target - remove all compiled binaries
clean:
	rm -f $(TARGET) hello_dev hello_optimized hello_strict hello_debug hello_clang hello_clang_dev hello_clang_optimized hello_clang_strict hello_clang_debug *.exe *.out *.o

# Test target - build and run the program
test: $(TARGET)
	./$(TARGET)

# Help target - show available targets
help:
	@echo "Available targets:"
	@echo "  all        - Standard build (default)"
	@echo "  dev        - Development build with warnings (-Wall -Wextra)"
	@echo "  optimized  - Optimized build (-O2 -Wall -Wextra)"
	@echo "  strict     - Strict compilation with maximum warnings"
	@echo "  debug      - Debug build with -g flag"
	@echo "  clang      - Standard build using Clang compiler"
	@echo "  clang-dev  - Development build using Clang compiler"
	@echo "  clang-optimized - Optimized build using Clang compiler"
	@echo "  clang-strict - Strict compilation using Clang compiler"
	@echo "  clang-debug - Debug build using Clang compiler"
	@echo "  test       - Build and run the program"
	@echo "  clean      - Remove all compiled binaries"
	@echo "  help       - Show this help message"

# Declare phony targets
.PHONY: all dev optimized strict debug clang clang-dev clang-optimized clang-strict clang-debug clean test help