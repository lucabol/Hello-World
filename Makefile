# Makefile for Hello World C Program
# Provides build automation for standard, development, and optimized builds

# Compiler settings
CC = gcc
TARGET = hello
SOURCE = hello.c

# Compiler flags for different build types
CFLAGS_DEV = -Wall -Wextra
CFLAGS_OPTIMIZED = -O2 -Wall -Wextra
CFLAGS_STRICT = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion

# Default target (optimized build as requested)
all: $(TARGET)

# Optimized build (default)
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS_OPTIMIZED) -o $(TARGET) $(SOURCE)

# Development build with warnings
dev: $(SOURCE)
	$(CC) $(CFLAGS_DEV) -o $(TARGET) $(SOURCE)

# Debug build
debug: $(SOURCE)
	$(CC) -g $(CFLAGS_DEV) -o hello_debug $(SOURCE)

# Strict compilation (maximum warnings)
strict: $(SOURCE)
	$(CC) $(CFLAGS_STRICT) -o $(TARGET) $(SOURCE)

# Clang build
clang: $(SOURCE)
	clang -o hello_clang $(SOURCE)

# Clean target - remove all compiled binaries
clean:
	rm -f $(TARGET) hello_debug hello_clang *.exe *.out *.o

# Declare phony targets
.PHONY: all dev debug strict clang clean