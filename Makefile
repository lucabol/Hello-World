# Simple Makefile for Hello-World project with code metrics

CC=gcc
CFLAGS=-Wall -Wextra
TARGETS=hello metrics

all: $(TARGETS)

hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

metrics: metrics.c
	$(CC) $(CFLAGS) -o metrics metrics.c

clean:
	rm -f $(TARGETS) hello_debug hello_clang *.exe *.out *.o

test: all
	@echo "Testing hello program:"
	./hello
	@echo ""
	@echo "Running code metrics analyzer:"
	./metrics

.PHONY: all clean test