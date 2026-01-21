# Makefile for Hello World and Code Metrics projects

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
CLANG = clang

# Targets
.PHONY: all clean test test-hello test-metrics strict debug help

all: hello code_metrics

hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

code_metrics: code_metrics.c
	$(CC) $(CFLAGS) -o code_metrics code_metrics.c

strict:
	$(CC) -Wall -Wextra -Wpedantic -Werror -o hello hello.c
	$(CC) -Wall -Wextra -Wpedantic -Werror -o code_metrics code_metrics.c

debug:
	$(CC) -Wall -Wextra -Wpedantic -g -o hello hello.c
	$(CC) -Wall -Wextra -Wpedantic -g -o code_metrics code_metrics.c

clang:
	$(CLANG) $(CFLAGS) -o hello hello.c
	$(CLANG) $(CFLAGS) -o code_metrics code_metrics.c

test: test-hello test-metrics

test-hello:
	@echo "Running hello.c validation tests..."
	@bash test/validate.sh

test-metrics:
	@echo "Running code_metrics.c validation tests..."
	@bash test/validate_code_metrics.sh

test-quiet:
	@bash test/validate.sh --quiet

clean:
	rm -f hello hello_debug hello_clang code_metrics *.exe *.out *.o

help:
	@echo "Available targets:"
	@echo "  all          - Build hello and code_metrics (default)"
	@echo "  hello        - Build hello program only"
	@echo "  code_metrics - Build code_metrics program only"
	@echo "  strict       - Build with -Werror flag"
	@echo "  debug        - Build with debug symbols"
	@echo "  clang        - Build using clang compiler"
	@echo "  test         - Run all test suites"
	@echo "  test-hello   - Run hello.c tests only"
	@echo "  test-metrics - Run code_metrics.c tests only"
	@echo "  test-quiet   - Run tests with minimal output"
	@echo "  clean        - Remove all build artifacts"
	@echo "  help         - Show this help message"
