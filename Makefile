# Makefile for Hello World C program and Code Metrics Tool
# Provides convenient build targets including unit testing and code metrics
#
# Note: -Werror is intentionally enabled for CI quality assurance to catch potential issues early.
# Contributors should ensure code compiles cleanly with the same flags.

CC = gcc
# Standard flags for hello program (strict with -Werror for quality)
CFLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
# Flags for metrics tool (requires POSIX for getline)
CFLAGS_METRICS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -D_POSIX_C_SOURCE=200809L
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

# Build with Clang (portable without -Werror for cross-compiler compatibility)
.PHONY: clang
clang:
	clang -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -std=c99 -o hello_clang hello.c

# Build code metrics tool with Clang (portable)
.PHONY: metrics_tool_clang
metrics_tool_clang: metrics_tool.c code_metrics.c code_metrics.h
	clang -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -std=c99 -D_POSIX_C_SOURCE=200809L -o metrics_tool_clang metrics_tool.c code_metrics.c

# Build code metrics tool with AddressSanitizer for memory leak detection
.PHONY: metrics_tool_asan
metrics_tool_asan: metrics_tool.c code_metrics.c code_metrics.h
	$(CC) -Wall -Wextra -Wpedantic -std=c99 -D_POSIX_C_SOURCE=200809L -fsanitize=address,undefined -fno-omit-frame-pointer -g -o metrics_tool_asan metrics_tool.c code_metrics.c
