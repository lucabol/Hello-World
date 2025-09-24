# Makefile for Hello World C program
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
STRICT_FLAGS = -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror

SOURCE = hello.c
TARGET = hello
STRICT_TARGET = hello_strict

# Default target
all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -O2 -o $(TARGET) $(SOURCE)

# Strict compilation target for validation
strict: $(STRICT_TARGET)

$(STRICT_TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(STRICT_FLAGS) -o $(STRICT_TARGET) $(SOURCE)

# Clean build artifacts
clean:
	rm -f $(TARGET) $(STRICT_TARGET) *.exe *.out *.o

.PHONY: all strict clean