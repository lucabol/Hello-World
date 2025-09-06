# Makefile for hello.c with plugin system

CC = gcc
CFLAGS = -Wall -Wextra -fPIC
LDFLAGS = -ldl

# Main program
MAIN_TARGET = hello
MAIN_SOURCE = hello.c

# Plugin sources
PLUGIN_SOURCES = $(wildcard plugins/*.c)
PLUGIN_TARGETS = $(PLUGIN_SOURCES:.c=.so)

# Default target
all: $(MAIN_TARGET) plugins

# Build main program
$(MAIN_TARGET): $(MAIN_SOURCE) plugin.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Build all plugins
plugins: $(PLUGIN_TARGETS)

# Rule to build individual plugins
plugins/%.so: plugins/%.c plugin.h
	$(CC) $(CFLAGS) -shared -o $@ $<

# Clean build artifacts
clean:
	rm -f $(MAIN_TARGET) $(PLUGIN_TARGETS)

# Run the program
run: $(MAIN_TARGET)
	./$(MAIN_TARGET)

# Run without plugins (for testing backward compatibility)
run-no-plugins: $(MAIN_TARGET)
	rm -rf plugins/*.so
	./$(MAIN_TARGET)
	$(MAKE) plugins

# Install example (could copy to system directories)
install: all
	@echo "Plugin system built successfully"
	@echo "Main program: $(MAIN_TARGET)"
	@echo "Plugins: $(PLUGIN_TARGETS)"

.PHONY: all plugins clean run run-no-plugins install