# Plugin System Documentation

## Overview

The plugin system allows external developers to extend hello.c functionality without modifying the core file. Plugins can transform the greeting message through a well-defined interface.

## Architecture

### Components

1. **plugin.h** - Plugin API interface definition
2. **plugin.c** - Plugin system implementation (registry, loading, chain execution)
3. **plugins/** - Directory containing plugin implementations
4. **hello_plugin_demo.c** - Demonstration program
5. **Makefile** - Build system with plugin support

### Plugin Interface

Each plugin must implement the `plugin_info_t` structure:

```c
typedef struct plugin_info {
    int api_version;                    /* Must be PLUGIN_API_VERSION */
    char name[PLUGIN_NAME_MAX];         /* Plugin name */
    char version[PLUGIN_VERSION_MAX];   /* Plugin version string */
    plugin_init_fn init;                /* Initialization (optional) */
    plugin_transform_fn transform;      /* Transformation (required) */
    plugin_cleanup_fn cleanup;          /* Cleanup (optional) */
} plugin_info_t;
```

### Plugin Chain

Plugins are applied sequentially in the order they are registered:

```
Input → Plugin 1 → Plugin 2 → Plugin 3 → Output
```

Each plugin receives the output of the previous plugin as its input.

## Quick Start

### Building

```bash
# Build original hello (no plugins)
make

# Build plugin system demo
make plugin-demo

# Run the demo
./hello_plugin_demo
```

### Creating a Plugin

1. Create a new file in `plugins/` directory:

```c
#include "../plugin.h"
#include <string.h>

/* Transform function */
static int my_transform(const char* input, char* output, size_t output_size) {
    /* Your transformation logic */
    strncpy(output, input, output_size - 1);
    output[output_size - 1] = '\0';
    return PLUGIN_SUCCESS;
}

/* Export plugin info */
plugin_info_t plugin_info = {
    .api_version = PLUGIN_API_VERSION,
    .name = "my_plugin",
    .version = "1.0.0",
    .init = NULL,
    .transform = my_transform,
    .cleanup = NULL
};
```

2. Compile the plugin:

```bash
gcc -Wall -Wextra -c plugins/my_plugin.c -o plugins/my_plugin.o
```

3. Link with your program:

```bash
gcc -o my_program my_program.c plugin.o plugins/my_plugin.o
```

## Example Plugins

### uppercase.c

Converts the greeting to uppercase.

**Input:** `Hello world!`  
**Output:** `HELLO WORLD!`

**Implementation:**
```c
for (i = 0; i < len; i++) {
    output[i] = toupper(input[i]);
}
```

### exclaim.c

Adds extra exclamation marks.

**Input:** `Hello world!`  
**Output:** `Hello world!!!`

**Implementation:**
```c
strncpy(output, input, output_size - 3);
output[len] = '!';
output[len + 1] = '!';
output[len + 2] = '\0';
```

## API Reference

### Plugin System Functions

#### `void plugin_system_init(void)`
Initialize the plugin system. Must be called before using any other plugin functions.

#### `int plugin_register(plugin_info_t* info)`
Register a statically linked plugin. Returns `PLUGIN_SUCCESS` on success.

#### `int plugin_apply_all(const char* input, char* output, size_t output_size)`
Apply all registered plugins in sequence. Returns `PLUGIN_SUCCESS` on success.

#### `void plugin_system_cleanup(void)`
Cleanup and unload all plugins. Calls each plugin's cleanup function if provided.

### Plugin Functions

#### `plugin_init_fn`
```c
typedef int (*plugin_init_fn)(void);
```
Called when the plugin is loaded. Optional.

#### `plugin_transform_fn`
```c
typedef int (*plugin_transform_fn)(const char* input, char* output, size_t output_size);
```
Transforms the input string. Required.

#### `plugin_cleanup_fn`
```c
typedef int (*plugin_cleanup_fn)(void);
```
Called before unloading the plugin. Optional.

## Best Practices

### Plugin Development

1. **Validate inputs**: Always check for NULL pointers and buffer sizes
2. **Return proper codes**: Use `PLUGIN_SUCCESS` or `PLUGIN_ERROR`
3. **Avoid side effects**: Don't modify global state
4. **Keep it simple**: Each plugin should do one thing well
5. **Check buffer sizes**: Always ensure output buffer is large enough

### Error Handling

```c
static int safe_transform(const char* input, char* output, size_t output_size) {
    if (!input || !output || output_size == 0) {
        return PLUGIN_ERROR;
    }
    
    size_t len = strlen(input);
    if (len >= output_size) {
        return PLUGIN_ERROR;
    }
    
    /* Safe transformation */
    strncpy(output, input, output_size - 1);
    output[output_size - 1] = '\0';
    
    return PLUGIN_SUCCESS;
}
```

### Buffer Management

- Always check that output buffer is large enough
- Use `strncpy` instead of `strcpy`
- Always null-terminate output strings
- Leave room for null terminator when calculating sizes

## Advanced Usage

### Multiple Plugins

```c
plugin_system_init();

/* Register plugins in desired order */
plugin_register(&uppercase_info);
plugin_register(&exclaim_info);

/* Apply all plugins */
char output[256];
plugin_apply_all("Hello world!", output, sizeof(output));
/* Result: "HELLO WORLD!!!" */

plugin_system_cleanup();
```

### Conditional Plugin Loading

```c
if (enable_uppercase) {
    plugin_register(&uppercase_info);
}

if (enable_exclaim) {
    plugin_register(&exclaim_info);
}
```

## Testing

### Manual Testing

```bash
# Build and run demo
make plugin-demo
./hello_plugin_demo

# Expected output:
# Example 1: No plugins
# Hello world!
#
# Example 2: With uppercase plugin
# HELLO WORLD!
```

### Integration Testing

The plugin system is designed to be integrated with existing test infrastructure. Plugins should not break the original functionality when none are loaded.

## Limitations

### Current Limitations

1. **Static linking only**: Dynamic plugin loading (dlopen) not yet implemented
2. **Fixed buffer size**: Plugin chain uses fixed 256-byte buffers
3. **No plugin dependencies**: Plugins cannot declare dependencies on other plugins
4. **No configuration**: No plugin configuration file support
5. **Single-threaded**: Not designed for concurrent access

### Future Enhancements

- Dynamic plugin loading with dlopen/LoadLibrary
- Plugin configuration files (JSON/YAML)
- Plugin dependency resolution
- Plugin priorities and ordering
- Async plugin execution
- Thread-safe plugin registry
- Plugin hot-reloading

## Security Considerations

### Input Validation

Plugins must validate all inputs to prevent:
- Buffer overflows
- NULL pointer dereferences
- Format string vulnerabilities

### Sandboxing

Future versions may include:
- Resource limits (CPU, memory)
- Capability-based security
- Plugin isolation

## Troubleshooting

### Common Issues

**Plugin not transforming output**
- Check that `plugin_register()` returns `PLUGIN_SUCCESS`
- Verify plugin's `transform` function is not NULL
- Ensure plugin API version matches

**Compilation errors**
- Include `plugin.h` in plugin source files
- Use correct flags: `-Wall -Wextra -std=c99`
- Link with `plugin.o`

**Runtime errors**
- Check buffer sizes are adequate
- Verify input is properly null-terminated
- Ensure proper error handling in transform function

## Examples

See the `plugins/` directory for working examples:
- `plugins/uppercase.c` - Text transformation
- `plugins/exclaim.c` - Text augmentation
- `plugins/README.md` - Plugin development guide

## Contributing

When contributing new plugins:
1. Follow the coding style of existing plugins
2. Include comprehensive error checking
3. Document the plugin's purpose and behavior
4. Test with various inputs
5. Ensure no memory leaks

## License

This plugin system is part of the Hello World project.
