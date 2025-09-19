# Makefile for Hello World C Program
# Provides build automation for standard, development, and optimized builds

# Compiler settings
CC = gcc
TARGET = hello
SOURCE = hello.c

# All generated binaries
BINFILES = hello hello_dev hello_debug hello_strict hello_clang

# Compiler flags for different build types
CFLAGS_DEV = -Wall -Wextra
CFLAGS_OPTIMIZED = -O2 -Wall -Wextra
CFLAGS_STRICT = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion
CFLAGS_CLANG = -O2 -Wall -Wextra

# Default target (optimized build as requested)
all: $(TARGET)

# Optimized build (default)
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS_OPTIMIZED) -o $(TARGET) $(SOURCE)

# Development build with warnings
dev: hello_dev
hello_dev: $(SOURCE)
	$(CC) $(CFLAGS_DEV) -o hello_dev $(SOURCE)

# Debug build
debug: hello_debug
hello_debug: $(SOURCE)
	$(CC) -g $(CFLAGS_DEV) -o hello_debug $(SOURCE)

# Strict compilation (maximum warnings)
strict: hello_strict
hello_strict: $(SOURCE)
	$(CC) $(CFLAGS_STRICT) -o hello_strict $(SOURCE)

# Clang build with optimization and warnings
clang: hello_clang
hello_clang: $(SOURCE)
	clang $(CFLAGS_CLANG) -o hello_clang $(SOURCE)

# Clean target - remove all compiled binaries
clean:
	rm -f $(BINFILES) *.exe *.out *.o

# Declare phony targets
.PHONY: all dev debug strict clang clean