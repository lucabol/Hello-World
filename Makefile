# Makefile for Hello World C Program
# Provides automated build targets for different compilation scenarios

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
	$(CC) $(CFLAGS_OPTIMIZED) -o $(TARGET) $(SOURCE)

# Strict compilation with all warnings
strict: $(SOURCE)
	$(CC) $(CFLAGS_STRICT) -o $(TARGET) $(SOURCE)

# Clean compiled binaries
clean:
	rm -f $(TARGET) hello_debug hello_clang *.exe *.out *.o

# Build and run the program
test run: $(TARGET)
	./$(TARGET)

# Alternative compiler build (clang)
clang: $(SOURCE)
	clang -o hello_clang $(SOURCE)

# Help target
help:
	@echo "Available targets:"
	@echo "  all (default) - Standard build"
	@echo "  dev           - Development build with warnings"
	@echo "  debug         - Debug build with symbols"
	@echo "  optimized     - Optimized build with -O2"
	@echo "  strict        - Strict build with all warnings"
	@echo "  clean         - Remove compiled binaries"
	@echo "  test/run      - Build and run the program"
	@echo "  clang         - Build with clang compiler"
	@echo "  help          - Show this help message"

# Declare phony targets
.PHONY: all dev development debug optimized release strict clean test run clang help