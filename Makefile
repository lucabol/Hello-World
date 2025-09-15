# Makefile for Hello World C Program
# Provides build automation for standard, development, and optimized builds

# Compiler settings
CC = gcc
CFLAGS_BASE = -Wall -Wextra
CFLAGS_DEBUG = -g $(CFLAGS_BASE)
CFLAGS_OPTIMIZED = -O2 $(CFLAGS_BASE)
CFLAGS_STRICT = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion

# Source and target files
SRC = hello.c
TARGET = hello
TARGET_DEBUG = hello_debug
TARGET_OPTIMIZED = hello_optimized
TARGET_STRICT = hello_strict

# Default target - standard build with warnings
.PHONY: all
all: $(TARGET)

# Standard build (equivalent to development build with warnings)
$(TARGET): $(SRC)
	$(CC) $(CFLAGS_BASE) -o $(TARGET) $(SRC)

# Development build (same as standard for this simple project)
.PHONY: dev development
dev development: $(TARGET)

# Debug build with debug symbols
.PHONY: debug
debug: $(TARGET_DEBUG)

$(TARGET_DEBUG): $(SRC)
	$(CC) $(CFLAGS_DEBUG) -o $(TARGET_DEBUG) $(SRC)

# Optimized build for production
.PHONY: optimized release
optimized release: $(TARGET_OPTIMIZED)

$(TARGET_OPTIMIZED): $(SRC)
	$(CC) $(CFLAGS_OPTIMIZED) -o $(TARGET_OPTIMIZED) $(SRC)

# Strict build with comprehensive warnings
.PHONY: strict
strict: $(TARGET_STRICT)

$(TARGET_STRICT): $(SRC)
	$(CC) $(CFLAGS_STRICT) -o $(TARGET_STRICT) $(SRC)

# Clean compiled binaries
.PHONY: clean
clean:
	rm -f $(TARGET) $(TARGET_DEBUG) $(TARGET_OPTIMIZED) $(TARGET_STRICT) *.exe *.out *.o

# Test the program (builds and runs default target)
.PHONY: test
test: $(TARGET)
	./$(TARGET)

# Help target to display available targets
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all          - Build standard version (default)"
	@echo "  dev          - Build development version (same as all)"
	@echo "  debug        - Build debug version with debug symbols"
	@echo "  optimized    - Build optimized version for production"
	@echo "  strict       - Build with strict warning flags"
	@echo "  test         - Build and run the program"
	@echo "  clean        - Remove all compiled binaries"
	@echo "  help         - Display this help message"
	@echo ""
	@echo "Build commands used:"
	@echo "  Standard:  $(CC) $(CFLAGS_BASE) -o $(TARGET) $(SRC)"
	@echo "  Debug:     $(CC) $(CFLAGS_DEBUG) -o $(TARGET_DEBUG) $(SRC)"
	@echo "  Optimized: $(CC) $(CFLAGS_OPTIMIZED) -o $(TARGET_OPTIMIZED) $(SRC)"
	@echo "  Strict:    $(CC) $(CFLAGS_STRICT) -o $(TARGET_STRICT) $(SRC)"