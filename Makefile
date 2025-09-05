# Makefile for Hello World C program

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra
TARGET = hello
SOURCE = hello.c

# Default target
build: $(TARGET)

# Build the program
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Clean compiled binaries
clean:
	rm -f $(TARGET)

# Test the program
test: $(TARGET)
	./$(TARGET)

# Declare phony targets
.PHONY: build clean test