# Makefile for hello.c project
# Provides automated builds and testing

CC = gcc
CFLAGS = -Wall -Wextra
TARGET = hello
SOURCE = hello.c

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Build with debug information
debug: $(SOURCE)
	$(CC) -g $(CFLAGS) -o hello_debug $(SOURCE)

# Build optimized version
optimized: $(SOURCE)
	$(CC) -O2 $(CFLAGS) -o hello_optimized $(SOURCE)

# Build with strict warnings
strict: $(SOURCE)
	$(CC) $(CFLAGS) -Wpedantic -Wformat=2 -o hello_strict $(SOURCE)

# Run automated tests
test: test_hello.sh
	./test_hello.sh

# Clean build artifacts
clean:
	rm -f $(TARGET) hello_debug hello_optimized hello_strict hello_test*

# Install dependencies (none needed for this simple project)
install:
	@echo "No dependencies to install for this simple C project"

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build the main executable (default)"
	@echo "  debug     - Build with debug information"
	@echo "  optimized - Build optimized version"
	@echo "  strict    - Build with strict warnings"
	@echo "  test      - Run automated tests"
	@echo "  clean     - Remove build artifacts"
	@echo "  help      - Show this help message"

.PHONY: all debug optimized strict test clean install help