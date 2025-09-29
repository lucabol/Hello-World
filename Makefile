# Makefile for Hello World C program
# Provides standardized build targets mentioned in copilot-instructions.md

CC = gcc
CFLAGS = -Wall -Wextra
STRICT_CFLAGS = -Wall -Wextra -Wpedantic -Werror
CLANG = clang
TARGET = hello
SOURCE = hello.c

# Default target
all: $(TARGET)

# Standard build
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Strict build (matches CI validation)
strict: $(SOURCE)
	$(CC) $(STRICT_CFLAGS) -o $(TARGET) $(SOURCE)

# Clang build
clang: $(SOURCE)
	$(CLANG) $(CFLAGS) -o hello_clang $(SOURCE)

# Debug build
debug: $(SOURCE)
	$(CC) -g $(CFLAGS) -o hello_debug $(SOURCE)

# Optimized build
optimized: $(SOURCE)
	$(CC) -O2 $(CFLAGS) -o $(TARGET) $(SOURCE)

# Run validation tests
test:
	@./test/validate.sh

# Run tests in quiet mode (CI-style)
test-quiet:
	@./test/validate.sh --quiet

# Clean build artifacts
clean:
	rm -f $(TARGET) hello_clang hello_debug hello_strict *.exe *.out *.o

# Mark targets as phony
.PHONY: all strict clang debug optimized test test-quiet clean