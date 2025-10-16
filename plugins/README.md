# Plugins Directory

This directory contains example plugins for the hello.c plugin system.

## Available Plugins

### example_uppercase.c
Converts the message to uppercase.

**Usage:**
```bash
gcc -o hello hello_plugin_enabled.c plugin.c plugins/example_uppercase.c -DLOAD_UPPERCASE
./hello
# Output: HELLO WORLD!
```

### example_reverse.c
Reverses the message.

**Usage:**
```bash
gcc -o hello hello_plugin_enabled.c plugin.c plugins/example_reverse.c -DLOAD_REVERSE
./hello
# Output: !dlrow olleH
```

## Creating Your Own Plugin

1. Create a new file in this directory: `example_yourplugin.c`
2. Include the plugin header: `#include "../plugin.h"`
3. Implement your transformation function
4. Implement the initialization function: `void plugin_yourplugin_init(void)`

See the example plugins for reference.

For detailed documentation, see [../PLUGIN_SYSTEM.md](../PLUGIN_SYSTEM.md)
