# Hello World Plugin System

This document describes how to develop and use plugins for the Hello World C program.

## Overview

The plugin system allows external developers to extend the functionality of hello.c without modifying the core file. Plugins can transform the output message in various ways.

## Plugin Architecture

- **Plugin Interface**: Defined in `plugin.h`
- **Plugin Loader**: Implemented in `plugin.c`
- **Plugin Directory**: `plugins/` (created automatically)
- **Plugin Format**: Shared libraries (.so files)

## Default Behavior

When no plugins are present, the program outputs exactly: `Hello world!` (no trailing newline)

## Plugin Development

### Basic Plugin Structure

```c
#include "../plugin.h"

// Required functions
static int my_plugin_init(void) {
    return 0; // Return 0 for success, non-zero for failure
}

static char* my_plugin_transform(const char* input) {
    // Transform the input and return a new allocated string
    // The caller will free the returned string
    // Return NULL on error
}

static void my_plugin_cleanup(void) {
    // Optional cleanup code
}

static int my_plugin_version(void) {
    return PLUGIN_API_VERSION;
}

// Plugin descriptor
static plugin_t my_plugin = {
    .name = "my_plugin",
    .description = "Description of what this plugin does",
    .init = my_plugin_init,
    .transform = my_plugin_transform,
    .cleanup = my_plugin_cleanup,
    .version = my_plugin_version
};

// Required entry point (exported symbol)
__attribute__((visibility("default")))
plugin_t* get_plugin(void) {
    return &my_plugin;
}
```

### Compilation

Compile your plugin as a shared library with hidden visibility:
```bash
gcc -Wall -Wextra -std=c99 -shared -fPIC -fvisibility=hidden -o plugins/my_plugin.so my_plugin.c
```

The `-fvisibility=hidden` flag ensures that only the `get_plugin` function (marked with `__attribute__((visibility("default")))`) is exported from the plugin.

### Plugin Loading Order

Plugins are loaded in the order they are found in the directory. Each plugin's transformation is applied in sequence.

### Plugin Entry Point

All plugins MUST export a function called `get_plugin` with this exact signature:

```c
__attribute__((visibility("default")))
plugin_t* get_plugin(void);
```

This function should return a pointer to a static `plugin_t` structure containing the plugin's metadata and function pointers. The structure will be copied by the main program, so it can be static.

**Note**: The `__attribute__((visibility("default")))` is required because plugins are compiled with `-fvisibility=hidden` to prevent accidental symbol exports.

## Security and Safety Considerations

**⚠️ IMPORTANT SECURITY NOTICE ⚠️**

Loading plugins involves executing arbitrary code from shared libraries. This presents significant security risks:

### Risks
- **Arbitrary Code Execution**: Plugins can execute any code with the same privileges as the main program
- **Memory Corruption**: Poorly written plugins can corrupt memory or cause crashes
- **Data Access**: Plugins have full access to program memory and can read/modify any data
- **System Access**: Plugins inherit all system permissions of the main program

### Best Practices
- **Only Load Trusted Plugins**: Never load plugins from untrusted sources
- **Code Review**: Review all plugin source code before compilation and deployment
- **Principle of Least Privilege**: Run the program with minimal necessary permissions
- **Sandboxing**: Consider running in containers or sandboxed environments
- **Input Validation**: Plugins should validate all inputs and handle errors gracefully
- **Resource Limits**: Be aware that plugins can consume unlimited CPU, memory, or other resources

### Recommended Deployment Practices
- Ship only plugins you have personally reviewed and trust
- Use containerization (Docker, etc.) to limit system access
- Run as non-root user with restricted permissions
- Monitor resource usage in production environments
- Have incident response procedures for plugin-related security issues

## Example Plugins

### Uppercase Plugin (`plugins/uppercase.c`)
Converts the message to uppercase.

### Emoji Plugin (`plugins/emoji.c`)
Adds emoji decorations to the message.

## Building and Testing

### Build the main program:
```bash
gcc -o hello hello.c plugin.c -ldl
```

### Test without plugins:
```bash
# Remove or rename plugins directory
mv plugins plugins_disabled
./hello
# Output: Hello world!
```

### Test with plugins:
```bash
# Ensure plugins directory exists with compiled plugins
./hello
# Output will be transformed by loaded plugins
```

## Plugin API Reference

### Plugin Structure (`plugin_t`)
- `name`: Plugin name (for identification)
- `description`: Human-readable description
- `init`: Initialization function (called once at startup)
- `transform`: Message transformation function (called for each message)
- `cleanup`: Cleanup function (called at program exit)
- `version`: API version function (for compatibility)

### Plugin Functions

#### `init` Function
- **Signature**: `int init(void)`
- **Purpose**: Initialize plugin resources
- **Return**: 0 for success, non-zero for failure

#### `transform` Function
- **Signature**: `char* transform(const char* input)`
- **Purpose**: Transform the input message
- **Parameters**: `input` - The current message (read-only, do not modify or free)
- **Return**: Newly allocated transformed string using `malloc()`, or NULL on error
- **Memory Management**: 
  - The plugin MUST allocate the return string using `malloc()` or compatible allocators
  - The caller (main program) will `free()` the returned string
  - If the function returns NULL, the main program falls back to the original message
  - The input parameter should NOT be freed by the plugin

#### `cleanup` Function
- **Signature**: `void cleanup(void)`
- **Purpose**: Clean up plugin resources

#### `version` Function
- **Signature**: `int version(void)`
- **Purpose**: Return plugin API version
- **Return**: Must return `PLUGIN_API_VERSION`

### Constants
- `PLUGIN_API_VERSION`: Current API version (1)
- `MAX_MESSAGE_LEN`: Maximum message length (256)

### API Versioning
The plugin API uses `PLUGIN_API_VERSION` to ensure compatibility:

- **Current Version**: 1 (initial release)
- **Version Check**: Plugins must return `PLUGIN_API_VERSION` from their `version()` function
- **Compatibility**: Plugins with different API versions are rejected during loading
- **Future Changes**: API version will increment when breaking changes are made to:
  - Function signatures in `plugin_t` structure
  - Expected behavior of plugin functions
  - Constants or data structures
- **Plugin Updates**: When API version changes, existing plugins must be updated and recompiled

## Error Handling

The plugin system includes comprehensive error handling:

### Plugin Loading Errors
When plugins fail to load, warnings are printed to stderr in this format:
- `Warning: Cannot load plugin <path>: <dlerror_message>` - Shared library loading failed
- `Warning: Plugin <path> missing get_plugin function` - Required entry point not found
- `Warning: Plugin <path> has invalid descriptor` - get_plugin() returned NULL or invalid structure
- `Warning: Plugin <path> has incompatible API version` - Version mismatch with PLUGIN_API_VERSION
- `Warning: Plugin <path> initialization failed` - init() function returned non-zero

### Runtime Behavior
- **Failed Plugins**: Skipped entirely; cleanup() is only called for successfully initialized plugins
- **Transform Failures**: If transform() returns NULL, the input message continues unchanged to the next plugin
- **Memory Allocation Failures**: Program falls back to original message
- **Missing Plugins Directory**: Silently ignored (not an error condition)

### Warning Output
Warnings are designed to be informative but not disruptive:
- Printed only once per plugin during startup
- Use stderr to avoid interfering with stdout message
- Include specific error details when available
- Normal operation continues even with plugin failures

## Limitations

- Maximum of 16 plugins can be loaded
- Plugins must be compiled for the same architecture
- Plugin transformations are applied sequentially
- No inter-plugin communication mechanism