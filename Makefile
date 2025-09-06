# Makefile for Hello World C Program
# Provides automated build targets for different compilation modes

# Compiler settings
CC = gcc
CFLAGS_STANDARD = 
CFLAGS_DEV = -Wall -Wextra
CFLAGS_DEBUG = -g -Wall -Wextra
CFLAGS_OPTIMIZED = -O2 -Wall -Wextra
CFLAGS_STRICT = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion

# Source and target files
SRC = hello.c
TARGET = hello
TARGET_DEBUG = hello_debug

# Default target
all: $(TARGET)

# Standard build
$(TARGET): $(SRC)
	$(CC) $(CFLAGS_STANDARD) -o $(TARGET) $(SRC)

# Development build with warnings
dev: $(SRC)
	$(CC) $(CFLAGS_DEV) -o $(TARGET) $(SRC)

# Debug build with debugging symbols and warnings
debug: $(SRC)
	$(CC) $(CFLAGS_DEBUG) -o $(TARGET_DEBUG) $(SRC)

# Optimized build for production
optimized: $(SRC)
	$(CC) $(CFLAGS_OPTIMIZED) -o $(TARGET) $(SRC)

# Strict compilation with all warnings
strict: $(SRC)
	$(CC) $(CFLAGS_STRICT) -o $(TARGET) $(SRC)

# Clean compiled binaries
clean:
	rm -f $(TARGET) $(TARGET_DEBUG) *.exe *.out *.o

# Run the program
run: $(TARGET)
	./$(TARGET)

# Test build - compiles and runs
test: dev
	./$(TARGET)

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build with standard settings (default)"
	@echo "  dev       - Build with development flags (-Wall -Wextra)"
	@echo "  debug     - Build with debug symbols and warnings"
	@echo "  optimized - Build with optimization (-O2)"
	@echo "  strict    - Build with strict warning flags"
	@echo "  clean     - Remove compiled binaries"
	@echo "  run       - Build and run the program"
	@echo "  test      - Build with dev flags and run"
	@echo "  help      - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make              # Standard build"
	@echo "  make dev          # Development build"
	@echo "  make CC=clang dev # Use clang compiler"

# Phony targets
.PHONY: all dev debug optimized strict clean run test help