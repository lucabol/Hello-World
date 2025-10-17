# Makefile for hello.c with plugin system
#
# Main targets:
#   make              - Build hello program
#   make plugins      - Build all plugins
#   make all          - Build everything (program + plugins)
#   make test         - Run validation tests
#   make unit-test    - Run unit tests
#   make clean        - Remove all build artifacts
#
# Plugin usage:
#   HELLO_PLUGIN=./plugins/uppercase.so ./hello

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c99
LDFLAGS = -ldl

# Source files
HELLO_SRC = hello.c
HELLO_BIN = hello

# Plugin settings
PLUGIN_DIR = plugins
PLUGIN_CFLAGS = -shared -fPIC -Wall -Wextra
PLUGINS = $(PLUGIN_DIR)/uppercase.so $(PLUGIN_DIR)/reverse.so

# Unit test settings
TEST_DIR = test
TEST_RUNNER = test_hello_runner
UNIT_TEST_FLAGS = -DUNIT_TEST

# Default target: build main program
all: $(HELLO_BIN) plugins

# Build main hello program
$(HELLO_BIN): $(HELLO_SRC) hello.h plugin.h
	$(CC) $(CFLAGS) -o $(HELLO_BIN) $(HELLO_SRC) $(LDFLAGS)

# Build all plugins
plugins: $(PLUGINS)

# Build individual plugins
$(PLUGIN_DIR)/uppercase.so: $(PLUGIN_DIR)/uppercase.c plugin.h
	$(CC) $(PLUGIN_CFLAGS) -o $@ $<

$(PLUGIN_DIR)/reverse.so: $(PLUGIN_DIR)/reverse.c plugin.h
	$(CC) $(PLUGIN_CFLAGS) -o $@ $<

# Unit tests
unit-test: hello.h
	$(CC) $(CFLAGS) $(UNIT_TEST_FLAGS) -I. -c -o hello_lib.o $(HELLO_SRC)
	$(CC) $(CFLAGS) -I. -o $(TEST_RUNNER) $(TEST_DIR)/test_hello.c hello_lib.o
	./$(TEST_RUNNER)
	rm -f hello_lib.o $(TEST_RUNNER)

# Validation tests
test: $(HELLO_BIN)
	bash $(TEST_DIR)/validate.sh --quiet

# Strict build (like CI)
strict: $(HELLO_SRC) hello.h plugin.h
	$(CC) -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -o hello_strict $(HELLO_SRC) $(LDFLAGS)

# Run with example plugin
demo-uppercase: all
	@echo "Running with uppercase plugin:"
	@HELLO_PLUGIN=./$(PLUGIN_DIR)/uppercase.so ./$(HELLO_BIN)
	@echo ""

demo-reverse: all
	@echo "Running with reverse plugin:"
	@HELLO_PLUGIN=./$(PLUGIN_DIR)/reverse.so ./$(HELLO_BIN)
	@echo ""

# Clean all build artifacts
clean:
	rm -f $(HELLO_BIN) hello_strict hello_debug hello_clang
	rm -f $(PLUGINS)
	rm -f *.o hello_lib.o
	rm -f $(TEST_RUNNER)

# Help target
help:
	@echo "Available targets:"
	@echo "  make              - Build hello program"
	@echo "  make plugins      - Build all plugins"
	@echo "  make all          - Build everything"
	@echo "  make unit-test    - Run unit tests"
	@echo "  make test         - Run validation tests"
	@echo "  make demo-uppercase - Demo with uppercase plugin"
	@echo "  make demo-reverse   - Demo with reverse plugin"
	@echo "  make clean        - Remove build artifacts"
	@echo ""
	@echo "Plugin usage:"
	@echo "  HELLO_PLUGIN=./plugins/uppercase.so ./hello"

.PHONY: all plugins unit-test test strict clean help demo-uppercase demo-reverse
