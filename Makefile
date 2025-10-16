# Makefile for Hello World project

.PHONY: all clean unit-test

all: hello

hello: hello.c
	gcc -Wall -Wextra -o hello hello.c

unit-test:
	@bash test/run_unit_tests.sh

clean:
	rm -f hello hello_lib.o test_hello_runner hello_clang
