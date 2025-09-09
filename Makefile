# Makefile for Hello World C Program

# Compiler settings
CC = gcc
CLANG = clang
CFLAGS = -Wall -Wextra
DEBUG_FLAGS = -g -Wall -Wextra
OPTIMIZED_FLAGS = -O2 -Wall -Wextra
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion

# Source and target settings
SRC = hello.c
TARGET = hello
DEBUG_TARGET = hello_debug
OPTIMIZED_TARGET = hello_optimized
STRICT_TARGET = hello_strict
CLANG_TARGET = hello_clang

# Default target
.PHONY: all debug optimized strict clang clean test help
.DEFAULT_GOAL := all

# Main targets
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(SRC)
	$(CC) $(DEBUG_FLAGS) -o $(DEBUG_TARGET) $(SRC)

optimized: $(OPTIMIZED_TARGET)

$(OPTIMIZED_TARGET): $(SRC)
	$(CC) $(OPTIMIZED_FLAGS) -o $(OPTIMIZED_TARGET) $(SRC)

strict: $(STRICT_TARGET)

$(STRICT_TARGET): $(SRC)
	$(CC) $(STRICT_FLAGS) -o $(STRICT_TARGET) $(SRC)

clang: $(CLANG_TARGET)

$(CLANG_TARGET): $(SRC)
	$(CLANG) -o $(CLANG_TARGET) $(SRC)

# Testing target
test: $(TARGET)
	./$(TARGET)

# Clean target
clean:
	rm -f $(TARGET) $(DEBUG_TARGET) $(OPTIMIZED_TARGET) $(STRICT_TARGET) $(CLANG_TARGET) *.exe *.out *.o

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the main executable with standard flags (default)"
	@echo "  debug      - Build with debug flags (-g -Wall -Wextra)"
	@echo "  optimized  - Build optimized version (-O2 -Wall -Wextra)"
	@echo "  strict     - Build with strict warning flags"
	@echo "  clang      - Build using clang compiler"
	@echo "  test       - Build and run the program"
	@echo "  clean      - Remove all compiled binaries"
	@echo "  help       - Show this help message"