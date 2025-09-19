# Simple Makefile for Hello World program

CC = gcc
CFLAGS = -Wall -Wextra
TARGET = hello
SOURCE = hello.c

# Default target
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Development build with debug info
debug: $(SOURCE)
	$(CC) $(CFLAGS) -g -o hello_debug $(SOURCE)

# Optimized build
optimized: $(SOURCE)
	$(CC) $(CFLAGS) -O2 -o hello_opt $(SOURCE)

# Clean compiled files
clean:
	rm -f $(TARGET) hello_debug hello_opt *.o

# Test the program
test: $(TARGET)
	./$(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  make          - Build the program"
	@echo "  make debug    - Build with debug info"
	@echo "  make optimized - Build optimized version"
	@echo "  make test     - Build and run the program"
	@echo "  make clean    - Remove compiled files"
	@echo "  make help     - Show this help"

.PHONY: debug optimized clean test help