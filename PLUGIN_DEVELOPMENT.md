# Plugin Development Guide

## Overview

The Hello World program supports a plugin system that allows external developers to extend functionality without modifying the core hello.c file. Plugins can register message transformers that modify the output message in various ways.

## Plugin Architecture

The plugin system uses dynamic libraries (.so files) that are loaded at runtime from the `plugins/` directory. Each plugin must implement a standard interface defined in `plugin.h`.

## Creating a Plugin

### 1. Include the Plugin Header

```c
#include "../plugin.h"
```

### 2. Implement Your Transformer Function

```c
static int my_transform(const char* input, char* output, size_t output_size) {
    // Transform the input message and write to output
    // Return 0 on success, -1 on error
    
    // Example: simple copy
    if (strlen(input) >= output_size) {
        return -1;  // Buffer too small
    }
    strcpy(output, input);
    return 0;
}
```

### 3. Implement the Plugin Initialization Function

```c
plugin_info_t plugin_init(void) {
    plugin_info_t info = {
        .name = "my_plugin",
        .version = PLUGIN_API_VERSION,
        .description = "Description of what my plugin does"
    };
    
    // Register your transformer
    register_transformer("my_plugin", my_transform);
    
    return info;
}
```

### 4. Build the Plugin

```bash
gcc -Wall -Wextra -std=c99 -fPIC -shared -o plugins/my_plugin.so my_plugin.c
```

## API Reference

### Types

- `plugin_info_t`: Contains plugin metadata (name, version, description)
- `message_transformer_t`: Function pointer type for message transformers
- `plugin_init_t`: Function pointer type for plugin initialization

### Functions

- `register_transformer(name, transformer)`: Register a message transformer function
- `apply_transformers(input, output, size)`: Apply all registered transformers to input
- `load_plugins_from_directory(path)`: Load all .so files from directory
- `cleanup_plugins()`: Clean up loaded plugins

### Constants

- `PLUGIN_API_VERSION`: Current plugin API version (1)
- `MAX_MESSAGE_SIZE`: Maximum message buffer size (256)
- `MAX_PLUGINS`: Maximum number of plugins (32)

### Reverse Plugin
Reverses the character order:
```c
static int reverse_transform(const char* input, char* output, size_t output_size) {
    size_t len = strlen(input);
    if (len >= output_size) return -1;
    
    for (size_t i = 0; i < len; i++) {
        output[i] = input[len - 1 - i];
    }
    output[len] = '\0';
    return 0;
}
```

## Example Combinations

- `uppercase.so` only: "Hello world!" → "HELLO WORLD!"
- `reverse.so` only: "Hello world!" → "!dlrow olleH"  
- `uppercase.so` + `exclamation.so`: "Hello world!" → "HELLO WORLD!!!"
- All plugins: "Hello world!" → "!!!DLROW OLLEH" (order depends on loading sequence)

### Uppercase Plugin
Converts the message to uppercase:
```c
static int uppercase_transform(const char* input, char* output, size_t output_size) {
    size_t len = strlen(input);
    if (len >= output_size) return -1;
    
    for (size_t i = 0; i <= len; i++) {
        output[i] = (char)toupper((unsigned char)input[i]);
    }
    return 0;
}
```

### Exclamation Plugin
Adds extra exclamation marks:
```c
static int exclamation_transform(const char* input, char* output, size_t output_size) {
    size_t len = strlen(input);
    if (len + 2 >= output_size) return -1;
    
    strcpy(output, input);
    strcat(output, "!!");
    return 0;
}
```

## Building and Testing

### Build Everything
```bash
make all plugins
```

### Test Without Plugins
```bash
rm plugins/*.so
./hello
# Output: Hello world!
```

### Test With Plugins
```bash
make plugins
./hello
# Output: HELLO WORLD!!!
```

### Validation
The core validation tests expect the original "Hello world!" output, so remove plugin files before running tests:
```bash
rm plugins/*.so
make test
```

## Plugin Execution Order

Plugins are loaded in the order they appear in the plugins directory. Transformers are applied sequentially, with each transformer's output becoming the input for the next transformer.

## Best Practices

1. Always check buffer sizes to prevent overflows
2. Handle edge cases gracefully (empty strings, etc.)
3. Return appropriate error codes (-1 for errors, 0 for success)
4. Keep transformations simple and focused
5. Test plugins both individually and in combination with others
6. Use meaningful names for plugins and transformers

## Troubleshooting

### Symbol Not Found Errors
Make sure the main executable is built with `-rdynamic` flag to export symbols to plugins.

### Plugin Not Loading
Check that:
- Plugin file has `.so` extension
- Plugin is in the `plugins/` directory  
- Plugin implements `plugin_init()` function
- Plugin compilation succeeded without errors

### Compilation Errors
Ensure you're using the correct compiler flags:
```bash
gcc -Wall -Wextra -std=c99 -fPIC -shared -o plugin.so plugin.c
```