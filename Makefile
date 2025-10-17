# Makefile for Hello World C Program
# Builds the hello program with separate library and main components

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
INCLUDES = -I.

# Output binaries
TARGET = hello
TEST_RUNNER = test_hello_runner

# Source files
LIB_SRC = hello_lib.c
MAIN_SRC = main.c
TEST_SRC = test/test_hello.c

# Object files
LIB_OBJ = hello_lib.o
MAIN_OBJ = main.o

# Default target - builds the hello program
all: $(TARGET)

# Build the hello executable
$(TARGET): $(LIB_OBJ) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(MAIN_OBJ) $(LIB_OBJ)

# Compile library object file
$(LIB_OBJ): $(LIB_SRC) hello.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $(LIB_SRC) -o $(LIB_OBJ)

# Compile main object file
$(MAIN_OBJ): $(MAIN_SRC) hello.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $(MAIN_SRC) -o $(MAIN_OBJ)

# Build with strict flags (for CI/validation)
strict: CFLAGS = $(STRICT_FLAGS)
strict: clean $(TARGET)

# Build unit tests
test: $(TEST_RUNNER)
	./$(TEST_RUNNER)

# Build test runner
$(TEST_RUNNER): $(LIB_OBJ) $(TEST_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TEST_RUNNER) $(TEST_SRC) $(LIB_OBJ)

# Run validation script
validate: $(TARGET)
	./test/validate.sh

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TEST_RUNNER) *.o *.exe *.out
	rm -f hello_debug hello_optimized hello_strict hello_clang hello_warnings

# Alternative compiler builds
clang: CC = clang
clang: clean $(TARGET)

debug: CFLAGS += -g
debug: clean
	$(CC) $(CFLAGS) $(INCLUDES) -o hello_debug $(MAIN_SRC) $(LIB_SRC)

optimized: CFLAGS += -O2
optimized: clean
	$(CC) $(CFLAGS) $(INCLUDES) -o hello_optimized $(MAIN_SRC) $(LIB_SRC)

.PHONY: all strict test validate clean clang debug optimized
