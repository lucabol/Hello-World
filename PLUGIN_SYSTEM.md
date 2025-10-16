# Plugin System for Hello World

This document describes the plugin architecture for the Hello World program, which allows external developers to extend functionality without modifying the core `hello.c` file.

## Overview

The plugin system enables dynamic loading of shared libraries that can transform the output message of the Hello World program. Plugins are loaded at runtime using environment variables and can be chained to create complex transformations.

## Architecture

### Core Components

1. **plugin_api.h** - Defines the plugin interface that all plugins must implement
2. **plugin_loader.h/.c** - Handles dynamic loading and management of plugins
3. **hello.c** - Core program with plugin integration (conditional compilation)
4. **hello.h** - Exposes core functionality for plugins and tests

### Plugin Interface

Each plugin must implement two functions:

```c
int plugin_init(void);
char* plugin_transform(const char* input);
```

- `plugin_init()`: Called once when the plugin is loaded. Returns 0 on success.
- `plugin_transform()`: Transforms the input message. Must return a heap-allocated string (caller frees).

## Building

### Build the Main Program

```bash
# Without plugin support (default, minimal)
make hello

# With plugin support
make hello-plugin

# Build example plugins
make plugins
```

### Build Custom Plugins

```bash
gcc -shared -fPIC -o my_plugin.so my_plugin.c
```

## Usage

### Running Without Plugins

```bash
./hello          # Outputs: Hello world!
./hello-plugin   # Outputs: Hello world! (same behavior)
```

### Running With Plugins

Use the `HELLO_PLUGINS` environment variable to specify plugins:

```bash
# Single plugin
HELLO_PLUGINS=./plugins/uppercase_plugin.so ./hello-plugin
# Outputs: HELLO WORLD!

# Multiple plugins (chained in order)
HELLO_PLUGINS=./plugins/bracket_plugin.so,./plugins/uppercase_plugin.so ./hello-plugin
# Outputs: [HELLO WORLD!]
```

## Example Plugins

Three example plugins are included:

### 1. Uppercase Plugin
Converts the message to uppercase.
```bash
HELLO_PLUGINS=./plugins/uppercase_plugin.so ./hello-plugin
# Output: HELLO WORLD!
```

### 2. Bracket Plugin
Wraps the message in square brackets.
```bash
HELLO_PLUGINS=./plugins/bracket_plugin.so ./hello-plugin
# Output: [Hello world!]
```

### 3. Reverse Plugin
Reverses the message.
```bash
HELLO_PLUGINS=./plugins/reverse_plugin.so ./hello-plugin
# Output: !dlrow olleH
```

## Creating Custom Plugins

### Step 1: Create Plugin Source

Create a C file implementing the plugin interface:

```c
#include "../plugin_api.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int plugin_init(void) {
    // Initialize plugin (optional setup)
    return 0;  // Return 0 for success
}

char* plugin_transform(const char* input) {
    if (input == NULL) {
        return NULL;
    }
    
    // Allocate memory for transformed output
    size_t len = strlen(input);
    char* output = (char*)malloc(len + 20);
    
    if (output == NULL) {
        return NULL;
    }
    
    // Transform the message
    sprintf(output, ">>> %s <<<", input);
    
    return output;  // Caller will free this
}
```

### Step 2: Compile the Plugin

```bash
gcc -shared -fPIC -o my_plugin.so my_plugin.c
```

### Step 3: Use the Plugin

```bash
HELLO_PLUGINS=./my_plugin.so ./hello-plugin
```

## Plugin Chaining

Plugins are applied in the order specified in the `HELLO_PLUGINS` environment variable:

```bash
# First applies bracket, then uppercase
HELLO_PLUGINS=./plugins/bracket_plugin.so,./plugins/uppercase_plugin.so ./hello-plugin
# Output: [HELLO WORLD!]

# First applies uppercase, then bracket
HELLO_PLUGINS=./plugins/uppercase_plugin.so,./plugins/bracket_plugin.so ./hello-plugin
# Output: [HELLO WORLD!]
```

## Technical Details

### Plugin Loading Process

1. Program checks `HELLO_PLUGINS` environment variable
2. If set, initializes the plugin loader
3. Parses comma-separated plugin paths
4. For each plugin:
   - Loads the shared library using `dlopen()`
   - Resolves `plugin_init` and `plugin_transform` symbols
   - Calls `plugin_init()` to initialize
5. Applies all plugins in sequence to the greeting message
6. Cleans up and unloads plugins before exit

### Memory Management

- Plugins must return heap-allocated strings from `plugin_transform()`
- The plugin loader frees the returned strings after use
- Each plugin receives the output of the previous plugin as input
- Original message is never modified

### Platform Support

- **Linux/Unix**: Uses `dlopen/dlsym` from `<dlfcn.h>`
- **Windows**: Uses `LoadLibrary/GetProcAddress` (with compatibility layer)
- **macOS**: Uses same POSIX API as Linux

### Backward Compatibility

The plugin system is designed with backward compatibility in mind:

- The basic `hello` program has NO plugin support (minimal binary)
- The `hello-plugin` program includes plugin support
- Without the `HELLO_PLUGINS` environment variable, both behave identically
- The original validation tests pass without any changes

## Error Handling

The plugin system handles errors gracefully:

- Invalid plugin paths: Reports error, continues with original message
- Missing plugin functions: Reports error, skips plugin
- Plugin initialization failure: Reports error, skips plugin
- Transform function returns NULL: Reports error, returns NULL
- Memory allocation failures: Reports error, returns NULL

## Testing

Run the test suite:

```bash
# Basic validation tests
make test

# Unit tests
make test-unit

# Manual plugin testing
make hello-plugin plugins
HELLO_PLUGINS=./plugins/uppercase_plugin.so ./hello-plugin
```

## API Reference

### plugin_api.h

```c
// Plugin initialization function type
typedef int (*plugin_init_func)(void);

// Plugin transformation function type
typedef char* (*plugin_transform_func)(const char* input);

// Function name constants
#define PLUGIN_INIT_FUNC "plugin_init"
#define PLUGIN_TRANSFORM_FUNC "plugin_transform"
```

### plugin_loader.h

```c
// Initialize the plugin loader
int plugin_loader_init(void);

// Load a plugin from file
int plugin_loader_load(const char* plugin_path);

// Apply all loaded plugins
char* plugin_loader_apply_transforms(const char* input);

// Get plugin count
int plugin_loader_get_count(void);

// Cleanup all plugins
void plugin_loader_cleanup(void);
```

## Limitations

- Maximum 32 plugins can be loaded simultaneously (configurable via `MAX_PLUGINS`)
- Plugins cannot modify the original message in place
- Plugins must be thread-safe if used in multi-threaded contexts
- Plugin order matters when chaining transformations
- No plugin version checking or dependency management
- No plugin unloading during runtime (all loaded plugins stay until cleanup)

## Best Practices

1. **Always check for NULL** in plugin functions
2. **Free allocated memory** - plugins must return heap-allocated strings
3. **Handle errors gracefully** - return NULL on error
4. **Keep transformations simple** - complex logic should be in separate functions
5. **Test plugins independently** before chaining
6. **Document plugin behavior** and requirements
7. **Use meaningful plugin names** that describe the transformation

## Future Extensions

Potential enhancements to the plugin system:

- Plugin configuration via files or additional environment variables
- Plugin versioning and compatibility checks
- Plugin dependencies and ordering constraints
- Hot-reloading of plugins without restart
- Plugin introspection (list loaded plugins, query capabilities)
- Built-in plugins for common transformations
- Plugin sandboxing for security
