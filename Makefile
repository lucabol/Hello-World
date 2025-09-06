# Makefile for Hello World C program
# Provides build automation for compiling, cleaning, and testing

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra
CFLAGS_DEBUG = -g -Wall -Wextra
CFLAGS_OPTIMIZED = -O2 -Wall -Wextra
CFLAGS_STRICT = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion

# Source and target files
SRC = hello.c
TARGET = hello
TARGET_DEBUG = hello_debug
TARGET_OPTIMIZED = hello_optimized
TARGET_STRICT = hello_strict

# Default target
all: $(TARGET)

# Standard build
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Debug build
debug: $(TARGET_DEBUG)
$(TARGET_DEBUG): $(SRC)
	$(CC) $(CFLAGS_DEBUG) -o $(TARGET_DEBUG) $(SRC)

# Optimized build
optimized: $(TARGET_OPTIMIZED)
$(TARGET_OPTIMIZED): $(SRC)
	$(CC) $(CFLAGS_OPTIMIZED) -o $(TARGET_OPTIMIZED) $(SRC)

# Strict compilation build
strict: $(TARGET_STRICT)
$(TARGET_STRICT): $(SRC)
	$(CC) $(CFLAGS_STRICT) -o $(TARGET_STRICT) $(SRC)

# Build all variants
build-all: $(TARGET) $(TARGET_DEBUG) $(TARGET_OPTIMIZED) $(TARGET_STRICT)

# Test target - build and run the program
test: $(TARGET)
	@echo "Running $(TARGET)..."
	@./$(TARGET)
	@echo ""
	@echo "Exit code: $$?"

# Clean compiled binaries
clean:
	rm -f $(TARGET) $(TARGET_DEBUG) $(TARGET_OPTIMIZED) $(TARGET_STRICT)
	rm -f *.exe *.out *.o *.obj

# Alternative compiler support
clang:
	$(MAKE) CC=clang

# Help target
help:
	@echo "Available targets:"
	@echo "  all (default) - Build standard version"
	@echo "  debug         - Build debug version with -g flag"
	@echo "  optimized     - Build optimized version with -O2"
	@echo "  strict        - Build with strict warning flags"
	@echo "  build-all     - Build all variants"
	@echo "  test          - Build and run the program"
	@echo "  clean         - Remove all compiled binaries"
	@echo "  clang         - Build using clang compiler"
	@echo "  help          - Show this help message"

# Declare phony targets
.PHONY: all debug optimized strict build-all test clean clang help