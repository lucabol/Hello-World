CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -std=c99
LDFLAGS =

# Default target
all: hello

# Build the main program
hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c $(LDFLAGS)

# Unit tests target
unit-test: test/test_hello.c hello.c
	$(CC) $(CFLAGS) -DUNIT_TEST -I. -c -o hello_lib.o hello.c
	$(CC) $(CFLAGS) -I. -o test_hello_runner test/test_hello.c hello_lib.o
	./test_hello_runner
	rm -f hello_lib.o test_hello_runner

# Clean build artifacts
clean:
	rm -f hello hello_clang hello_debug hello_lib.o test_hello_runner *.o

.PHONY: all unit-test clean
