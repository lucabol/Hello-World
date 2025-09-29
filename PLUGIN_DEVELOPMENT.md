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

// Required entry point
plugin_t* get_plugin(void) {
    return &my_plugin;
}
```

### Compilation

Compile your plugin as a shared library:
```bash
gcc -shared -fPIC -o plugins/my_plugin.so my_plugin.c
```

### Plugin Loading Order

Plugins are loaded in the order they are found in the directory. Each plugin's transformation is applied in sequence.

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
- **Parameters**: `input` - The current message
- **Return**: Newly allocated transformed string, or NULL on error
- **Note**: Caller frees the returned string

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

## Error Handling

- Invalid plugins are skipped with warnings
- Missing plugins directory is handled gracefully
- Plugin initialization failures result in plugin being skipped
- Memory allocation failures fall back to original message

## Limitations

- Maximum of 16 plugins can be loaded
- Plugins must be compiled for the same architecture
- Plugin transformations are applied sequentially
- No inter-plugin communication mechanism