# Makefile for Hello World C program
# Provides convenient build targets including unit testing

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
TARGET = hello
TEST_RUNNER = test_hello_runner
HELLO_LIB_OBJ = hello_lib.o

# Default target: build the main program
.PHONY: all
all: $(TARGET)

# Build the main hello program
$(TARGET): hello.c hello.h
	$(CC) -o $(TARGET) hello.c

# Unit test target (required by CI)
.PHONY: unit-test
unit-test:
	@echo "Building unit tests..."
	@$(CC) $(CFLAGS) -I. -c -o $(HELLO_LIB_OBJ) hello.c -DUNIT_TEST
	@$(CC) $(CFLAGS) -I. -o $(TEST_RUNNER) test/test_hello.c $(HELLO_LIB_OBJ)
	@echo "Running unit tests..."
	@./$(TEST_RUNNER)
	@rm -f $(HELLO_LIB_OBJ) $(TEST_RUNNER)

# Clean build artifacts
.PHONY: clean
clean:
	rm -f $(TARGET) $(TEST_RUNNER) $(HELLO_LIB_OBJ) hello_strict hello_clang hello_debug *.o *.exe *.out

# Strict build (matches CI validation)
.PHONY: strict
strict:
	$(CC) $(CFLAGS) -o hello_strict hello.c

# Build with debug symbols
.PHONY: debug
debug:
	$(CC) -g -Wall -Wextra -o hello_debug hello.c

# Run validation tests
.PHONY: test
test: strict
	@bash test/validate.sh

# Build with Clang
.PHONY: clang
clang:
	clang -Wall -Wextra -o hello_clang hello.c
