CC = gcc
CFLAGS = -Wall -Wextra

# Default target
all: hello

# Main program
hello: hello.c hello_lib.c hello.h
	$(CC) $(CFLAGS) -o hello hello.c hello_lib.c

# Unit tests
test: hello_test
	./hello_test

hello_test: test_hello.c hello_lib.c hello.h
	$(CC) $(CFLAGS) -o hello_test test_hello.c hello_lib.c

# Clean compiled files
clean:
	rm -f hello hello_test *.o

.PHONY: all test clean