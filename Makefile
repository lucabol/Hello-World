# Makefile for Hello World C program

# Compiler
CC = gcc

# Source file
SRC = hello.c

# Target binaries
TARGET = hello
TARGET_DEBUG = hello_debug

# Compiler flags
CFLAGS_DEV = -Wall -Wextra
CFLAGS_OPTIMIZED = -O2 -Wall -Wextra
CFLAGS_DEBUG = -g -Wall -Wextra
CFLAGS_STRICT = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion

# Default target (optimized build)
all: $(TARGET)

# Optimized build (default)
$(TARGET): $(SRC)
	$(CC) $(CFLAGS_OPTIMIZED) -o $(TARGET) $(SRC)

# Development build with warnings
dev: $(SRC)
	$(CC) $(CFLAGS_DEV) -o $(TARGET) $(SRC)

# Debug build
debug: $(SRC)
	$(CC) $(CFLAGS_DEBUG) -o $(TARGET_DEBUG) $(SRC)

# Strict build with all warnings
strict: $(SRC)
	$(CC) $(CFLAGS_STRICT) -o $(TARGET) $(SRC)

# Clean compiled binaries and temporary files (respecting .gitignore)
clean:
	rm -f hello hello_debug hello_optimized hello_strict hello_clang hello_warnings hello_test* *.exe *.out *.o *.obj

# Phony targets
.PHONY: all dev debug strict clean