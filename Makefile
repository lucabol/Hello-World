CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lncurses

# Default target
all: hello metrics

# Build the original hello program
hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

# Build the metrics analyzer
metrics: metrics.c
	$(CC) $(CFLAGS) -o metrics metrics.c $(LDFLAGS)

# Clean compiled files
clean:
	rm -f hello metrics hello_debug hello_clang *.exe *.out *.o

# Test both programs
test: all
	@echo "Testing hello program:"
	./hello
	@echo ""
	@echo "Testing metrics analyzer:"
	./metrics hello.c

# Help target
help:
	@echo "Available targets:"
	@echo "  all     - Build both hello and metrics programs (default)"
	@echo "  hello   - Build only the hello program"
	@echo "  metrics - Build only the metrics analyzer"
	@echo "  test    - Build and test both programs"
	@echo "  clean   - Remove compiled files"
	@echo "  help    - Show this help message"

.PHONY: all clean test help