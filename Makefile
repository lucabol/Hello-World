# Simple Makefile for Hello World C program
# Variables
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = hello
SOURCE = hello.c

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Test target - build and run the program
test: $(TARGET)
	@echo "Running $(TARGET)..."
	@./$(TARGET)
	@echo "Program completed successfully."

# Clean compiled files
clean:
	@echo "Cleaning up..."
	@rm -f $(TARGET)
	@echo "Clean complete."

# Install to system (optional)
install: $(TARGET)
	@echo "Installing $(TARGET) to /usr/local/bin..."
	@cp $(TARGET) /usr/local/bin/
	@echo "Installation complete."

# Uninstall from system (optional)
uninstall:
	@echo "Removing $(TARGET) from /usr/local/bin..."
	@rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstall complete."

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build the program (default)"
	@echo "  $(TARGET)     - Build the program"
	@echo "  test      - Build and run the program"
	@echo "  clean     - Remove compiled files"
	@echo "  install   - Install to /usr/local/bin"
	@echo "  uninstall - Remove from /usr/local/bin"
	@echo "  help      - Show this help message"

# Declare phony targets
.PHONY: all clean test install uninstall help