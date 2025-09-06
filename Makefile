# Makefile for Hello World C Program
# Provides automated build targets for different compilation scenarios and comprehensive testing

# Compiler and basic settings
CC = gcc
TARGET = hello
SOURCE = hello.c

# Compiler flags for different build types
CFLAGS_STANDARD = 
CFLAGS_DEV = -Wall -Wextra
CFLAGS_DEBUG = -g -Wall -Wextra
CFLAGS_OPTIMIZED = -O2 -Wall -Wextra
CFLAGS_STRICT = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion

# Default target - standard build
all: $(TARGET)

# Standard build
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS_STANDARD) -o $(TARGET) $(SOURCE)

# Development build with warnings
dev development: $(SOURCE)
	$(CC) $(CFLAGS_DEV) -o $(TARGET) $(SOURCE)

# Debug build with debug symbols and warnings
debug: $(SOURCE)
	$(CC) $(CFLAGS_DEBUG) -o hello_debug $(SOURCE)

# Optimized/release build
optimized release: $(SOURCE)
	$(CC) $(CFLAGS_OPTIMIZED) -o hello_optimized $(SOURCE)

# Strict compilation with all warnings
strict: $(SOURCE)
	$(CC) $(CFLAGS_STRICT) -o hello_strict $(SOURCE)

# Clean compiled binaries
clean:
	rm -f $(TARGET) hello_debug hello_optimized hello_strict hello_clang hello_test* *.exe *.out *.o

# Build and run the program
run: $(TARGET)
	./$(TARGET)

# Run comprehensive automated tests
test: test_hello.sh
	./test_hello.sh

# Alternative compiler build (clang)
clang: $(SOURCE)
	clang -o hello_clang $(SOURCE)

# Install dependencies (none needed for this simple project)
install:
	@echo "No dependencies to install for this simple C project"

# Help target
help:
	@echo "Available targets:"
	@echo "  all (default) - Standard build"
	@echo "  dev           - Development build with warnings"
	@echo "  debug         - Debug build with symbols"
	@echo "  optimized     - Optimized build with -O2"
	@echo "  strict        - Strict build with all warnings"
	@echo "  clean         - Remove compiled binaries"
	@echo "  run           - Build and run the program"
	@echo "  test          - Run comprehensive automated tests"
	@echo "  clang         - Build with clang compiler"
	@echo "  install       - Install dependencies (none needed)"
	@echo "  help          - Show this help message"

# Declare phony targets
.PHONY: all dev development debug optimized release strict clean run test clang install help
