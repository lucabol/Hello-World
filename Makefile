# Makefile for Hello World with voice-driven editing support
# Provides targets for building, testing, and cleaning the project

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -std=c99
STRICT_FLAGS = $(CFLAGS) -Werror

# Build targets
.PHONY: all clean test unit-test strict debug voice-demo

# Default target - build the main program
all: hello

# Build the hello program
hello: hello.c hello.h
	$(CC) $(CFLAGS) -o hello hello.c

# Build with strict flags (CI-like)
strict: hello.c hello.h
	$(CC) $(STRICT_FLAGS) -o hello hello.c

# Build debug version
debug: hello.c hello.h
	$(CC) $(CFLAGS) -g -o hello_debug hello.c

# Build voice demo
voice-demo: voice_demo.c voice.c voice.h hello.c hello.h
	$(CC) $(CFLAGS) -o voice_demo voice_demo.c voice.c hello.c -DUNIT_TEST

# Run unit tests
unit-test:
	@echo "Building unit tests..."
	@$(CC) $(STRICT_FLAGS) -I. -c -o hello_lib.o hello.c -DUNIT_TEST
	@$(CC) $(STRICT_FLAGS) -I. -o test_hello_runner test/test_hello.c hello_lib.o
	@echo "Running unit tests..."
	@./test_hello_runner
	@rm -f hello_lib.o test_hello_runner

# Run validation tests
test: strict
	@bash test/validate.sh

# Run tests quietly (for CI)
test-quiet: strict
	@bash test/validate.sh --quiet

# Clean build artifacts
clean:
	rm -f hello hello_debug hello_strict hello_clang voice_demo voice_demo_test
	rm -f hello_lib.o test_hello_runner
	rm -f *.exe *.out *.o
