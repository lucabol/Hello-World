# Makefile for Hello-World C Program
# Provides automated build targets for different compilation scenarios

# Compiler and source files
CC = gcc
SOURCE = hello.c
TARGET = hello
DEBUG_TARGET = hello_debug

# Compiler flags
CFLAGS = 
DEVFLAGS = -Wall -Wextra
DEBUGFLAGS = -g -Wall -Wextra
OPTFLAGS = -O2 -Wall -Wextra
STRICTFLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion

# Default target (standard build)
all: $(TARGET)

# Standard build
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Development build with warnings
dev: $(SOURCE)
	$(CC) $(DEVFLAGS) -o $(TARGET) $(SOURCE)

# Debug build with debug symbols and warnings
debug: $(SOURCE)
	$(CC) $(DEBUGFLAGS) -o $(DEBUG_TARGET) $(SOURCE)

# Optimized build
optimized: $(SOURCE)
	$(CC) $(OPTFLAGS) -o $(TARGET) $(SOURCE)

# Strict compilation with all warnings
strict: $(SOURCE)
	$(CC) $(STRICTFLAGS) -o $(TARGET) $(SOURCE)

# Build and run the program
test: $(TARGET)
	./$(TARGET)

# Clean compiled binaries
clean:
	rm -f $(TARGET) $(DEBUG_TARGET) *.exe *.out *.o

# Show available targets
help:
	@echo "Available targets:"
	@echo "  all       - Standard build (default)"
	@echo "  dev       - Development build with warnings"
	@echo "  debug     - Debug build with debug symbols"
	@echo "  optimized - Optimized build (-O2)"
	@echo "  strict    - Strict compilation with all warnings"
	@echo "  test      - Build and run the program"
	@echo "  clean     - Remove compiled binaries"
	@echo "  help      - Show this help message"

# Declare phony targets
.PHONY: all dev debug optimized strict test clean help