# Plugins Directory

This directory contains plugin implementations for the hello.c plugin system.

## Available Plugins

### uppercase.c
Converts the input message to uppercase.

**Example:**
- Input: `"Hello world!"`
- Output: `"HELLO WORLD!"`

### reverse.c
Reverses the input message character by character.

**Example:**
- Input: `"Hello world!"`
- Output: `"!dlrow olleH"`

### prefix.c
Adds a prefix (">>> ") to the input message.

**Example:**
- Input: `"Hello world!"`
- Output: `">>> Hello world!"`

## Creating Your Own Plugin

To create a new plugin:

1. Create a new `.c` file in this directory
2. Include the plugin header: `#include "../plugin.h"`
3. Implement a transform function: `const char* my_transform(const char* input)`
4. Create a plugin descriptor: `const plugin_t my_plugin = { .name = "my_plugin", .transform = my_transform }`
5. Build your application with the plugin source file

See `../PLUGIN_GUIDE.md` for detailed instructions and examples.

## Plugin Requirements

- Each plugin must use a static buffer for output (max 256 bytes)
- Transform function must return a valid, null-terminated string
- Plugins should not modify the input string
- Plugins are not thread-safe due to static buffer usage

## Testing Your Plugin

You can test your plugin using the test framework:

1. Add test cases to `../test/test_plugin.c`
2. Build and run: `../test/run_plugin_tests.sh`

Or create a standalone test program:

```c
#include <stdio.h>
#include "plugin.h"

extern const plugin_t my_plugin;

int main(void) {
    plugin_init();
    plugin_register(&my_plugin);
    
    const char* result = plugin_apply_all("Test input");
    printf("Result: %s\n", result);
    
    return 0;
}
```

Build with: `gcc -o test test.c plugin.c plugins/my_plugin.c`
