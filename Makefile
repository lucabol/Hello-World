# Makefile for Hello World project

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99

# Default target
all: hello spreadsheet

# Build hello program
hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

# Build spreadsheet tool
spreadsheet: spreadsheet.c metrics.c metrics.h
	$(CC) $(CFLAGS) -o spreadsheet spreadsheet.c metrics.c

# Strict build for validation
strict: hello.c
	$(CC) $(STRICT_FLAGS) -o hello_strict hello.c

# Strict build for spreadsheet tool
strict-spreadsheet: spreadsheet.c metrics.c metrics.h
	$(CC) $(STRICT_FLAGS) -o spreadsheet_strict spreadsheet.c metrics.c

# Strict build for all
strict-all: strict strict-spreadsheet

# Test target for hello.c
test: strict
	./test/validate.sh

# Test target for metrics
test-metrics: spreadsheet
	./test/test_metrics.sh

# Test all
test-all: test test-metrics

# Clean build artifacts
clean:
	rm -f hello hello_strict spreadsheet spreadsheet_strict *.o metrics.csv test/test_metrics_runner test/test_*.c.tmp

# Build with debug symbols
debug: CFLAGS += -g
debug: all

.PHONY: all clean test test-metrics test-all strict strict-spreadsheet strict-all debug
