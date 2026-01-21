<<<<<<< HEAD
# Makefile for Hello World C program

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra
STRICT_CFLAGS = -Wall -Wextra -Wpedantic -Werror

# Source files
SOURCES = hello.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = hello

# Build targets
.PHONY: all clean format format-check test strict debug

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

strict: $(SOURCES)
	$(CC) $(STRICT_CFLAGS) -o $(TARGET) $(SOURCES)

debug: $(SOURCES)
	$(CC) $(CFLAGS) -g -o $(TARGET)_debug $(SOURCES)

# Format all C source files
format:
	@echo "Formatting C source files..."
	@clang-format -i $(SOURCES)
	@echo "Formatting complete."

# Check if code is properly formatted (non-destructive)
format-check:
	@echo "Checking code formatting..."
	@for file in $(SOURCES); do \
		if ! clang-format "$$file" | diff -u "$$file" - > /dev/null; then \
			echo "ERROR: $$file is not properly formatted"; \
			echo "Run 'make format' to fix formatting issues"; \
			exit 1; \
		fi; \
	done
	@echo "All files are properly formatted."

# Run tests (placeholder for CI)
test: strict
	@echo "Running tests..."
	@./$(TARGET)
	@echo "Tests passed."

clean:
	rm -f $(TARGET) $(TARGET)_debug *.o *.out *.exe
=======
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
>>>>>>> main
