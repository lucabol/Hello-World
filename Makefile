# Makefile for Hello World project with tests

CC = gcc
CFLAGS = -Wall -Wextra
STRICT_CFLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion
TARGET = hello
TEST_TARGET = test_hello
TEST_RUNNER = test_runner

# Default target
all: $(TARGET)

# Build the main hello program
$(TARGET): hello.c
	$(CC) $(CFLAGS) -o $(TARGET) hello.c

# Build the test program
$(TEST_TARGET): test_hello.c
	$(CC) $(CFLAGS) -o $(TEST_TARGET) test_hello.c

# Run tests (depends on test program)
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Test with strict compilation flags
test-strict: hello.c
	$(CC) $(STRICT_CFLAGS) -o $(TARGET) hello.c
	$(CC) $(CFLAGS) -o $(TEST_TARGET) test_hello.c
	./$(TEST_TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TEST_TARGET) hello_debug hello_clang *.exe *.out *.o

# Alternative compilers
clang: hello.c
	clang -o hello_clang hello.c

# Debug build
debug: hello.c
	$(CC) -g $(CFLAGS) -o hello_debug hello.c

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the hello program (default)"
	@echo "  test       - Build and run unit tests"
	@echo "  test-strict- Build with strict flags and run tests"
	@echo "  clean      - Remove build artifacts"
	@echo "  clang      - Build with clang compiler"
	@echo "  debug      - Build with debug symbols"
	@echo "  help       - Show this help message"

# Declare phony targets
.PHONY: all test test-strict clean clang debug help