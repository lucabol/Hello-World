# Makefile for Hello World C program and Code Metrics Tool
# Provides convenient build targets including unit testing and code metrics

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
CFLAGS_METRICS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -std=c99 -D_POSIX_C_SOURCE=200809L
TARGET = hello
TEST_RUNNER = test_hello_runner
HELLO_LIB_OBJ = hello_lib.o

# Default target: build the main program and metrics tool
.PHONY: all
all: $(TARGET) metrics_tool

# Build the main hello program
$(TARGET): hello.c hello.h
	$(CC) $(CFLAGS) -o $(TARGET) hello.c

# Build code metrics tool
.PHONY: metrics_tool
metrics_tool: metrics_tool.c code_metrics.c code_metrics.h
	$(CC) $(CFLAGS_METRICS) -o metrics_tool metrics_tool.c code_metrics.c

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
	rm -f $(TARGET) $(TEST_RUNNER) $(HELLO_LIB_OBJ) hello_strict hello_clang hello_debug metrics_tool metrics_tool_clang metrics_tool_asan *.o *.exe *.out

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
