# Makefile for Hello World with CUnit Tests

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
CUNIT_FLAGS = -lcunit
TARGET = hello
TEST_TARGET = test_hello
TEST_RUNNER = run_tests

# Source files
MAIN_SRC = hello.c
LIB_SRC = hello_lib.c
TEST_SRC = test_hello.c

# Header files
HEADERS = hello.h

# Default target - builds the main program
all: $(TARGET)

# Build the main hello program
$(TARGET): $(MAIN_SRC) $(LIB_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(MAIN_SRC) $(LIB_SRC)

# Build the test executable
$(TEST_TARGET): $(TEST_SRC) $(LIB_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_SRC) $(LIB_SRC) $(CUNIT_FLAGS)

# Run tests - builds hello first, then builds and runs tests
test: $(TARGET) $(TEST_TARGET)
	@echo "Running CUnit tests..."
	./$(TEST_TARGET)

# Build both main program and tests
build-all: $(TARGET) $(TEST_TARGET)

# Clean up compiled files
clean:
	rm -f $(TARGET) $(TEST_TARGET)

# Run the main program
run: $(TARGET)
	./$(TARGET)

# Development build with debugging symbols
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# Strict compilation with additional warnings
strict: CFLAGS += -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion
strict: $(TARGET)

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the main hello program (default)"
	@echo "  test       - Build and run CUnit tests"
	@echo "  build-all  - Build both main program and tests"
	@echo "  run        - Run the main hello program"
	@echo "  debug      - Build with debugging symbols"
	@echo "  strict     - Build with strict compiler warnings"
	@echo "  clean      - Remove compiled files"
	@echo "  help       - Show this help message"

# Declare phony targets
.PHONY: all test build-all run debug strict clean help