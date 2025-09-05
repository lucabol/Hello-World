# Makefile for Hello-World C program
# Provides standardized build management

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

# Target executable
TARGET = hello
SOURCE = hello.c

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

# Release build (optimized)
release: CFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET) *.o *.exe *.out

# Install (copy to /usr/local/bin)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Uninstall
uninstall:
	rm -f /usr/local/bin/$(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Check for memory leaks (if valgrind is available)
check: $(TARGET)
	@which valgrind > /dev/null 2>&1 && valgrind --leak-check=full ./$(TARGET) || ./$(TARGET)

# Display help
help:
	@echo "Available targets:"
	@echo "  all       - Build the program (default)"
	@echo "  debug     - Build with debug flags"
	@echo "  release   - Build with optimization flags"
	@echo "  clean     - Remove build artifacts"
	@echo "  run       - Build and run the program"
	@echo "  check     - Run with valgrind if available"
	@echo "  install   - Install to /usr/local/bin"
	@echo "  uninstall - Remove from /usr/local/bin"
	@echo "  help      - Show this help message"

# Declare phony targets
.PHONY: all debug release clean run check install uninstall help