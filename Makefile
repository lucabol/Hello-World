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
# Note: voice_demo is standalone and does not link with hello.c
# voice.c has its own static get_greeting() for demonstration purposes
voice-demo: voice_demo.c voice.c voice.h
	$(CC) $(CFLAGS) -o voice_demo voice_demo.c voice.c

# Run unit tests
# Builds hello.c as a library object with UNIT_TEST defined (disables main())
# Links with test/test_hello.c and runs the test suite
# Ensures proper cleanup even on failure
unit-test:
	@echo "Building unit tests..."
	@$(CC) $(STRICT_FLAGS) -I. -c -o hello_lib.o hello.c -DUNIT_TEST || { echo "Error: Failed to compile hello.c for unit tests"; exit 1; }
	@$(CC) $(STRICT_FLAGS) -I. -o test_hello_runner test/test_hello.c hello_lib.o || { rm -f hello_lib.o; echo "Error: Failed to link unit tests"; exit 1; }
	@echo "Running unit tests..."
	@./test_hello_runner || { EXIT_CODE=$$?; rm -f hello_lib.o test_hello_runner; echo "Error: Unit tests failed"; exit $$EXIT_CODE; }
	@rm -f hello_lib.o test_hello_runner
	@echo ""
	@echo "Building voice command unit tests..."
	@$(CC) $(STRICT_FLAGS) -I. -o test_voice_runner test/test_voice.c voice.c || { echo "Error: Failed to compile voice unit tests"; exit 1; }
	@echo "Running voice command unit tests..."
	@./test_voice_runner > /tmp/voice_test_output.txt 2>&1 || { EXIT_CODE=$$?; rm -f test_voice_runner; cat /tmp/voice_test_output.txt | grep -E "(Running|✓|✗|Total|Passed|Failed|All tests)" || cat /tmp/voice_test_output.txt; echo "Error: Voice unit tests failed"; exit $$EXIT_CODE; }
	@cat /tmp/voice_test_output.txt | grep -E "(Running|✓|✗|Total|Passed|Failed|All tests)" || cat /tmp/voice_test_output.txt
	@rm -f test_voice_runner /tmp/voice_test_output.txt

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
