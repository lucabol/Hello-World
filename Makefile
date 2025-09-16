# Simple Makefile for hello.c and its tests

CC = gcc
CFLAGS = -Wall -Wextra
TARGET = hello
TEST_TARGET = test_hello
SOURCE = hello.c
TEST_SOURCE = test_hello.c

# Default target
all: $(TARGET)

# Build the main hello program
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Build the test program
$(TEST_TARGET): $(TEST_SOURCE)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_SOURCE)

# Run tests (requires hello to be built first)
test: $(TARGET) $(TEST_TARGET)
	./$(TEST_TARGET)

# Clean compiled files
clean:
	rm -f $(TARGET) $(TEST_TARGET) *.o

# Build with different compiler options for development
debug: $(SOURCE)
	$(CC) -g -Wall -Wextra -o hello_debug $(SOURCE)

strict: $(SOURCE)
	$(CC) -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello_strict $(SOURCE)

optimized: $(SOURCE)
	$(CC) -O2 -Wall -Wextra -o hello_optimized $(SOURCE)

.PHONY: all test clean debug strict optimized