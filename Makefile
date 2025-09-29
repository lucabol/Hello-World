CC = gcc
CFLAGS = -Wall -Wextra -std=c99
PLUGIN_CFLAGS = $(CFLAGS) -shared -fPIC -fvisibility=hidden
LDFLAGS = -ldl

# Main program
hello: hello.c plugin.c plugin.h
	$(CC) $(CFLAGS) -o hello hello.c plugin.c $(LDFLAGS)

# Build sample plugins
plugins: plugins/uppercase.so plugins/emoji.so

plugins/uppercase.so: plugins/uppercase.c plugin.h | plugins-dir
	$(CC) $(PLUGIN_CFLAGS) -o plugins/uppercase.so plugins/uppercase.c

plugins/emoji.so: plugins/emoji.c plugin.h | plugins-dir
	$(CC) $(PLUGIN_CFLAGS) -o plugins/emoji.so plugins/emoji.c

# Create plugins directory if it doesn't exist
plugins-dir:
	mkdir -p plugins

# Clean build artifacts
clean:
	rm -f hello
	rm -f plugins/*.so

# Test with and without plugins (non-destructive)
test: hello plugins
	@echo "Testing backward compatibility (no plugins):"
	@./test/test_backward_compatibility.sh
	@echo "Testing with plugins loaded:"
	@PLUGINS_DIR=plugins ./hello
	@echo

# CI target for automated testing
ci: clean all test
	@echo "All CI checks passed!"

# Build everything
all: hello plugins

.PHONY: clean test plugins-dir all plugins ci