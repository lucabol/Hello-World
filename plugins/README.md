# Example Plugins

This directory contains example plugins demonstrating the Hello World plugin system.

## Available Plugins

### uppercase_plugin.so
Converts the greeting message to uppercase.

**Example:**
```bash
HELLO_PLUGINS=./plugins/uppercase_plugin.so ./hello-plugin
# Output: HELLO WORLD!
```

### bracket_plugin.so
Wraps the greeting message in square brackets.

**Example:**
```bash
HELLO_PLUGINS=./plugins/bracket_plugin.so ./hello-plugin
# Output: [Hello world!]
```

### reverse_plugin.so
Reverses the greeting message.

**Example:**
```bash
HELLO_PLUGINS=./plugins/reverse_plugin.so ./hello-plugin
# Output: !dlrow olleH
```

## Building Plugins

To build all example plugins:
```bash
make plugins
```

To build a specific plugin:
```bash
gcc -shared -fPIC -o plugins/uppercase_plugin.so plugins/uppercase_plugin.c
```

## Creating Your Own Plugin

See the `PLUGIN_SYSTEM.md` documentation for detailed instructions on creating custom plugins.

Basic template:
```c
#include "../plugin_api.h"
#include <stdlib.h>
#include <string.h>

int plugin_init(void) {
    return 0;  // Success
}

char* plugin_transform(const char* input) {
    // Your transformation code here
    return strdup(input);  // Example: no transformation
}
```
