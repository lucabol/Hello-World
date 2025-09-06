# Makefile for hello.c with plugin system

CC = gcc
CFLAGS = -Wall -Wextra -fPIC
LDFLAGS = -ldl

# Main program
MAIN_TARGET = hello
MAIN_SOURCE = hello.c

# Test program
TEST_TARGET = test_plugin_system
TEST_SOURCE = test_plugin_system.c

# Security test program
SEC_TEST_TARGET = test_security
SEC_TEST_SOURCE = test_security.c

# Plugin sources
PLUGIN_SOURCES = $(wildcard plugins/*.c)
PLUGIN_TARGETS = $(PLUGIN_SOURCES:.c=.so)

# Default target
all: $(MAIN_TARGET) plugins

# Build main program
$(MAIN_TARGET): $(MAIN_SOURCE) plugin.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Build test program
$(TEST_TARGET): $(TEST_SOURCE) plugin.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Build security test program  
$(SEC_TEST_TARGET): $(SEC_TEST_SOURCE) plugin.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Build all plugins
plugins: $(PLUGIN_TARGETS)

# Rule to build individual plugins
plugins/%.so: plugins/%.c plugin.h
	$(CC) $(CFLAGS) -shared -o $@ $<

# Clean build artifacts
clean:
	rm -f $(MAIN_TARGET) $(TEST_TARGET) $(SEC_TEST_TARGET) $(PLUGIN_TARGETS)

# Run the program
run: $(MAIN_TARGET)
	./$(MAIN_TARGET)

# Run tests
test: $(TEST_TARGET) all
	./$(TEST_TARGET)

# Run security tests
test-security: $(SEC_TEST_TARGET) all
	./$(SEC_TEST_TARGET)

# Run all tests
test-all: test test-security

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

.PHONY: all plugins clean run test test-security test-all run-no-plugins install