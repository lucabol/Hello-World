# Makefile for Hello World program with automated testing

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = hello
TEST_TARGET = test_hello
LIB_OBJ = hello_lib.o

# Default target
all: $(TARGET)

# Build the main program
$(TARGET): hello.c $(LIB_OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) hello.c $(LIB_OBJ)

# Build the library object file
$(LIB_OBJ): hello_lib.c hello.h
	$(CC) $(CFLAGS) -c hello_lib.c

# Build and run tests
test: $(TEST_TARGET) $(TARGET)
	./$(TEST_TARGET)

# Build the test program
$(TEST_TARGET): test_hello.c $(LIB_OBJ) test_framework.h hello.h
	$(CC) $(CFLAGS) -o $(TEST_TARGET) test_hello.c $(LIB_OBJ)

# Clean built files
clean:
	rm -f $(TARGET) $(TEST_TARGET) $(LIB_OBJ)

# Run the main program
run: $(TARGET)
	./$(TARGET)

# Build with debug information
debug: CFLAGS += -g
debug: $(TARGET)

# Build with optimization
release: CFLAGS += -O2
release: $(TARGET)

.PHONY: all test clean run debug release