# Makefile for Hello World C program
# Provides standardized build targets for development and CI

CC = gcc
# Developer-friendly flags (warnings without -Werror for local development)
CFLAGS_DEV = -Wall -Wextra -Wpedantic -std=c99
# Strict flags for CI and strict target
CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c99
STRICT_FLAGS = -Wformat=2 -Wconversion -Wsign-conversion
BINARY = hello
CLANG_BINARY = hello_clang
DEBUG_BINARY = hello_debug
TEST_RUNNER = test_hello_runner
TEST_LIB = hello_lib.o

.PHONY: all clean test unit-test strict debug clang test-quiet dev

# Default target - build with developer-friendly flags (no -Werror)
all: $(BINARY)

# Developer build (no -Werror, easier for local development)
dev: hello.c hello.h
	$(CC) $(CFLAGS_DEV) -o $(BINARY) hello.c

$(BINARY): hello.c hello.h
	$(CC) $(CFLAGS_DEV) -o $(BINARY) hello.c

# Strict compilation (CI-like with extra warnings and -Werror)
strict: hello.c hello.h
	$(CC) $(CFLAGS) $(STRICT_FLAGS) -o $(BINARY) hello.c

# Debug build with symbols
debug: hello.c hello.h
	$(CC) -g $(CFLAGS_DEV) -o $(DEBUG_BINARY) hello.c

# Build with Clang
clang: hello.c hello.h
	clang $(CFLAGS_DEV) -o $(CLANG_BINARY) hello.c

# Build unit test library and runner, then run tests
# This target is self-contained and doesn't rely on external scripts to compile
unit-test: $(TEST_RUNNER)
	@./$(TEST_RUNNER)

# Build the test runner by compiling hello.c as a library with -DUNIT_TEST
# and linking it with the test file
$(TEST_RUNNER): hello.c hello.h test/test_hello.c test/simple_test.h
	$(CC) $(CFLAGS) $(STRICT_FLAGS) -I. -c -o $(TEST_LIB) hello.c -DUNIT_TEST
	$(CC) $(CFLAGS) $(STRICT_FLAGS) -I. -o $(TEST_RUNNER) test/test_hello.c $(TEST_LIB)

# Run validation tests (verbose)
test: strict
	@bash test/validate.sh

# Run validation tests (quiet mode for CI)
test-quiet: strict
	@bash test/validate.sh --quiet

# Clean compiled binaries and object files
clean:
	rm -f $(BINARY) $(DEBUG_BINARY) $(CLANG_BINARY) $(TEST_RUNNER) $(TEST_LIB) *.o *.exe *.out
