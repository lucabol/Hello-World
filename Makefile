CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -ldl

# Main program
hello: hello.c plugin.c plugin.h
	$(CC) $(CFLAGS) -o hello hello.c plugin.c $(LDFLAGS)

# Build sample plugins
plugins: plugins/uppercase.so plugins/emoji.so

plugins/uppercase.so: plugins/uppercase.c plugin.h
	$(CC) $(CFLAGS) -shared -fPIC -o plugins/uppercase.so plugins/uppercase.c

plugins/emoji.so: plugins/emoji.c plugin.h
	$(CC) $(CFLAGS) -shared -fPIC -o plugins/emoji.so plugins/emoji.c

# Create plugins directory if it doesn't exist
plugins-dir:
	mkdir -p plugins

# Clean build artifacts
clean:
	rm -f hello
	rm -f plugins/*.so

# Test with and without plugins
test: hello
	@echo "Testing without plugins:"
	@mv plugins plugins_backup 2>/dev/null || true
	@./hello
	@echo
	@echo "Testing with plugins:"
	@mv plugins_backup plugins 2>/dev/null || true
	@./hello
	@echo

# Build everything
all: hello plugins

.PHONY: clean test plugins-dir all plugins