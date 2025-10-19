# Makefile for Hello World project

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99

# Default target
all: hello spreadsheet

# Build hello program
hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

# Build spreadsheet tool
spreadsheet: spreadsheet.c metrics.c metrics.h
	$(CC) $(CFLAGS) -o spreadsheet spreadsheet.c metrics.c

# Strict build for validation
strict: hello.c
	$(CC) $(STRICT_FLAGS) -o hello_strict hello.c

# Test target
test: strict
	./test/validate.sh

# Clean build artifacts
clean:
	rm -f hello hello_strict spreadsheet *.o metrics.csv

# Build with debug symbols
debug: CFLAGS += -g
debug: all

.PHONY: all clean test strict debug
