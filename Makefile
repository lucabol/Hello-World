# Makefile for Hello World C program

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
TARGET = hello
TEST_TARGET = test_hello_runner

# Default target: build the program
all: $(TARGET)

# Build the main program
$(TARGET): hello.c hello.h
	$(CC) $(CFLAGS) -o $(TARGET) hello.c

# Build with strict flags (for CI)
strict: hello.c hello.h
	$(CC) $(STRICT_FLAGS) -o $(TARGET) hello.c

# Run unit tests
unit-test:
	@echo "Building unit tests..."
	@$(CC) $(STRICT_FLAGS) -I. -c -o hello_lib.o hello.c -DUNIT_TEST
	@$(CC) $(STRICT_FLAGS) -I. -o $(TEST_TARGET) test/test_hello.c hello_lib.o
	@echo "Running unit tests..."
	@./$(TEST_TARGET)
	@rm -f hello_lib.o $(TEST_TARGET)

# Run validation tests
test: strict
	@bash test/validate.sh

# Run tests quietly (for CI)
test-quiet: strict
	@bash test/validate.sh --quiet

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TARGET)_strict $(TARGET)_debug $(TARGET)_clang *.exe *.out *.o $(TEST_TARGET)

# Build with debug symbols
debug: hello.c hello.h
	$(CC) -g $(CFLAGS) -o $(TARGET)_debug hello.c

# Build with Clang
clang: hello.c hello.h
	clang $(CFLAGS) -o $(TARGET)_clang hello.c

.PHONY: all strict unit-test test test-quiet clean debug clang
