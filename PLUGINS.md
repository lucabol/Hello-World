# Plugin System Documentation

## Overview

The hello.c program now supports a plugin architecture that allows external developers to extend functionality without modifying the core hello.c file directly. The plugin system is completely optional - the original program works exactly as before when plugins are not used.

## Building

### Standard Build (No Plugin Support)
```bash
make hello
# or
gcc -Wall -Wextra -o hello hello.c
```

### Plugin-Enabled Build
```bash
make hello-plugins
# or
gcc -Wall -Wextra -DPLUGIN_ENABLED -ldl -o hello-plugins hello.c
```

### Build Everything (Standard + Plugins + Examples)
```bash
make all
```

## Using Plugins

Plugins are loaded via the `HELLO_PLUGIN` environment variable:

```bash
# Run without plugin (original behavior)
./hello-plugins

# Run with uppercase plugin
HELLO_PLUGIN=./plugins/uppercase.so ./hello-plugins

# Run with exclamation plugin  
HELLO_PLUGIN=./plugins/exclamation.so ./hello-plugins
```

## Plugin Architecture

### Plugin Interface

Plugins implement the interface defined in `plugin.h`:

```c
typedef struct {
    const char* name;        // Plugin name
    const char* version;     // Plugin version
    void (*init)(void);      // Called when plugin loads
    char* (*transform)(const char* input);  // Transform message
    void (*cleanup)(void);   // Called before exit
} plugin_t;

// Required function that each plugin must implement
plugin_t* plugin_register(void);
```

### Plugin Lifecycle

1. **Loading**: Plugin shared library (.so) is loaded via `dlopen()`
2. **Registration**: `plugin_register()` function is called to get plugin interface
3. **Initialization**: `plugin->init()` is called if defined
4. **Transformation**: `plugin->transform()` is called to modify the output message
5. **Cleanup**: `plugin->cleanup()` would be called before exit (currently simplified)

## Creating Plugins

### Basic Plugin Template

```c
#include "../plugin.h"
#include <stdlib.h>
#include <string.h>

static void my_init(void) {
    // Plugin initialization code
}

static char* my_transform(const char* input) {
    if (!input) return NULL;
    
    // Transform the input message
    char* result = malloc(strlen(input) + 1);
    strcpy(result, input);
    // ... modify result ...
    
    return result;  // Caller will free this
}

static void my_cleanup(void) {
    // Plugin cleanup code
}

static plugin_t my_plugin = {
    .name = "myplugin",
    .version = "1.0.0",
    .init = my_init,
    .transform = my_transform,
    .cleanup = my_cleanup
};

plugin_t* plugin_register(void) {
    return &my_plugin;
}
```

### Building Plugins

```bash
# Build individual plugin
gcc -Wall -Wextra -std=c99 -fPIC -shared -o myplugin.so myplugin.c

# Build all example plugins
make plugins
```

## Example Plugins

### Uppercase Plugin (`plugins/uppercase.c`)
Converts the output message to uppercase.

```bash
HELLO_PLUGIN=./plugins/uppercase.so ./hello-plugins
# Output: HELLO WORLD!
```

### Exclamation Plugin (`plugins/exclamation.c`)
Adds exclamation marks to the output message.

```bash
HELLO_PLUGIN=./plugins/exclamation.so ./hello-plugins  
# Output: Hello world!!!!
```

## Testing

```bash
# Test standard version
make test

# Test plugin system
make test-plugins

# Clean up
make clean
```

## Design Principles

1. **Backward Compatibility**: Original hello.c behavior is preserved when built without `-DPLUGIN_ENABLED`
2. **Minimal Core Changes**: Plugin support is conditionally compiled and doesn't affect the core logic
3. **Simple Interface**: Plugin interface is straightforward with clear responsibilities
4. **Memory Management**: Plugins are responsible for allocating transformed strings; caller frees them
5. **Error Tolerance**: System gracefully handles missing or invalid plugins
6. **Environment-Based**: Plugin loading is controlled via environment variables, not command-line arguments

## Limitations

- Only one plugin can be loaded at a time
- Plugins must be compiled as shared libraries (.so files)
- Limited error reporting for plugin loading failures
- No plugin discovery mechanism (plugins must be explicitly specified)
- Simplified cleanup (plugins are not explicitly unloaded)

## Future Enhancements

- Plugin chaining (multiple plugins in sequence)
- Plugin discovery and auto-loading
- Better error reporting and logging
- Configuration file support
- Plugin metadata and dependency management