# Makefile for Hello World project with Code Metrics tool

CC = gcc
CFLAGS = -Wall -Wextra
TARGETS = hello metrics

.PHONY: all clean test

all: $(TARGETS)

hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

metrics: metrics.c
	$(CC) $(CFLAGS) -o metrics metrics.c

test: all
	@echo "Testing hello program:"
	./hello
	@echo "\nTesting metrics tool:"
	@echo "1" | ./metrics | head -20

clean:
	rm -f $(TARGETS) hello_debug hello_optimized hello_strict *.exe *.out *.o

# Build with debug symbols
debug: hello.c metrics.c
	$(CC) $(CFLAGS) -g -o hello_debug hello.c
	$(CC) $(CFLAGS) -g -o metrics_debug metrics.c

# Build optimized versions
optimized: hello.c metrics.c
	$(CC) $(CFLAGS) -O2 -o hello_optimized hello.c
	$(CC) $(CFLAGS) -O2 -o metrics_optimized metrics.c

# Run the metrics tool interactively
run-metrics: metrics
	./metrics