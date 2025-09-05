# Code Metrics Spreadsheet Interface Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGETS = hello metrics

.PHONY: all clean test demo

# Build all targets
all: $(TARGETS)

# Build hello world program
hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

# Build metrics analyzer
metrics: metrics.c
	$(CC) $(CFLAGS) -o metrics metrics.c

# Run a quick demo of the metrics analyzer
demo: metrics
	@echo "=== Running Code Metrics Demo ==="
	@echo "1" | ./metrics hello.c

# Test both programs
test: all
	@echo "Testing hello world program..."
	./hello
	@echo "\nTesting metrics analyzer..."
	@echo "2" | ./metrics hello.c

# Clean compiled binaries
clean:
	rm -f $(TARGETS) hello_debug hello_clang *.exe *.out *.o

# Install (copy to system path)
install: all
	@echo "To install system-wide, run: sudo cp metrics /usr/local/bin/"

# Help
help:
	@echo "Available targets:"
	@echo "  all     - Build all programs"
	@echo "  hello   - Build hello world program"
	@echo "  metrics - Build code metrics analyzer"
	@echo "  demo    - Run a demo of the metrics analyzer"
	@echo "  test    - Test both programs"
	@echo "  clean   - Remove compiled binaries"
	@echo "  help    - Show this help message"