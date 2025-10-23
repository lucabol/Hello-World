# Makefile for Hello World and Code Metrics Tool
# Supports GCC and Clang with strict compilation flags

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -std=c99 -D_POSIX_C_SOURCE=200809L
CFLAGS_STRICT = $(CFLAGS) -Werror

# Targets
.PHONY: all clean test strict metrics help

all: hello metrics_tool

# Hello world program
hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

# Code metrics tool
metrics_tool: metrics_tool.c code_metrics.c code_metrics.h
	$(CC) $(CFLAGS) -o metrics_tool metrics_tool.c code_metrics.c

# Strict build (with -Werror)
strict: CFLAGS += -Werror
strict: clean all

# Run tests
test: all
	./test/validate.sh

# Run tests quietly (for CI)
test-quiet: all
	./test/validate.sh --quiet

# Clean build artifacts
clean:
	rm -f hello hello_strict hello_debug hello_clang metrics_tool
	rm -f *.o *.out

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Build hello and metrics_tool (default)"
	@echo "  hello        - Build hello.c"
	@echo "  metrics_tool - Build code metrics tool"
	@echo "  strict       - Build with -Werror flag"
	@echo "  test         - Run all tests"
	@echo "  test-quiet   - Run tests in quiet mode (for CI)"
	@echo "  clean        - Remove build artifacts"
	@echo "  help         - Show this help message"
