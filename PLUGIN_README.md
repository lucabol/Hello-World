# Plugin System for hello.c

This document describes how external developers can create plugins to extend hello.c functionality without modifying the core file directly.

## Overview

The plugin system allows developers to:
- Transform the output message before it's displayed
- Add pre-processing functionality that runs before the main program
- Add post-processing functionality that runs after the main program

## Building with Plugins

### Standard Build (No Plugins)
```bash
make hello          # Standard build - plugins disabled
./hello             # Outputs: "Hello world!" + exit code
```

### Plugin-Enabled Build
```bash  
make hello-plugins  # Plugin-enabled build
./hello-plugins     # Outputs transformed content via plugins
```

### Other Plugin Build Targets
```bash
make strict-plugins    # Strict compilation with plugins
make debug-plugins     # Debug build with plugins  
make clang-plugins     # Clang build with plugins
make test-plugins      # Test plugin functionality
```

## Plugin Development

### Plugin Types

The system supports three types of plugins:

1. **Output Transformers** (`PLUGIN_TYPE_OUTPUT_TRANSFORMER`)
   - Modify the main output message
   - Applied in registration order
   - Must return transformed string

2. **Pre-processors** (`PLUGIN_TYPE_PRE_PROCESSOR`)  
   - Execute before main functionality
   - Can display additional output
   - Return 0 for success, non-zero for error

3. **Post-processors** (`PLUGIN_TYPE_POST_PROCESSOR`)
   - Execute after main functionality
   - Can display additional output  
   - Return 0 for success, non-zero for error

### Creating a Plugin

1. **Include required headers:**
```c
#include "plugin.h"
```

2. **Implement plugin functions:**
```c
// Initialization (optional)
static int my_plugin_init(void) {
    plugin_log("My plugin initialized");
    return 0;
}

// Main functionality (depends on plugin type)
static const char* my_transform_function(const char* original) {
    // Transform and return modified string
    return transformed_string;
}

// Cleanup (optional)
static void my_plugin_cleanup(void) {
    plugin_log("My plugin cleaned up");
}
```

3. **Define plugin descriptor:**
```c
static const plugin_t my_plugin = {
    .name = "My Custom Plugin",
    .version = "1.0.0",
    .author = "Plugin Developer",
    .type = PLUGIN_TYPE_OUTPUT_TRANSFORMER,
    .init = my_plugin_init,
    .cleanup = my_plugin_cleanup,
    .transform_output = my_transform_function,
    .process = NULL  // Not used for output transformers
};
```

4. **Register the plugin:**
```c
int register_my_plugin(void) {
    return plugin_register(&my_plugin);
}
```

### Example: Simple Decoration Plugin

```c
#include "plugin.h"
#include <string.h>

static char decorated_buffer[1024];

static const char* add_decoration(const char* original) {
    snprintf(decorated_buffer, sizeof(decorated_buffer), 
             "*** %s ***", original);
    return decorated_buffer;
}

static const plugin_t decoration_plugin = {
    .name = "Message Decorator",
    .version = "1.0.0",
    .author = "Example Developer", 
    .type = PLUGIN_TYPE_OUTPUT_TRANSFORMER,
    .init = NULL,
    .cleanup = NULL,
    .transform_output = add_decoration,
    .process = NULL
};

int register_decoration_plugin(void) {
    return plugin_register(&decoration_plugin);
}
```

## Plugin API Reference

### Core Functions

- `int plugin_register(const plugin_t* plugin)` - Register a plugin
- `int plugin_load_all(void)` - Initialize all registered plugins  
- `void plugin_cleanup_all(void)` - Cleanup all plugins
- `void plugin_log(const char* message)` - Debug logging for plugins

### Plugin Function Signatures

- `typedef int (*plugin_init_func_t)(void)` - Initialization function
- `typedef void (*plugin_cleanup_func_t)(void)` - Cleanup function  
- `typedef const char* (*output_transformer_func_t)(const char*)` - Output transformer
- `typedef int (*processor_func_t)(void)` - Pre/post processor function

### Constants

- `MAX_PLUGINS` (10) - Maximum number of plugins
- `PLUGIN_TYPE_OUTPUT_TRANSFORMER` (1) - Output transformer type
- `PLUGIN_TYPE_PRE_PROCESSOR` (2) - Pre-processor type
- `PLUGIN_TYPE_POST_PROCESSOR` (3) - Post-processor type

## Integration with Existing Code

The plugin system is designed with minimal impact on the core hello.c:

1. **Backward Compatibility**: Standard builds (`make hello`) work exactly as before
2. **Conditional Compilation**: Plugin code only included when `ENABLE_PLUGINS` is defined
3. **Performance**: No overhead when plugins are disabled
4. **Safety**: Plugin errors don't crash the main program

## Debugging Plugins

Enable plugin debug output:
```bash
export PLUGIN_DEBUG=1
./hello-plugins
```

This will show plugin registration and initialization messages on stderr.

## Sample Plugins

The repository includes sample plugins in `sample_plugins.c`:

1. **Uppercase Transformer** - Converts output to uppercase
2. **Greeting Pre-processor** - Shows plugin activation message  
3. **Statistics Post-processor** - Shows completion message

Study these examples to understand plugin development patterns.

## Best Practices

1. **Error Handling**: Always validate inputs and return appropriate error codes
2. **Memory Safety**: Use safe string functions and bounded buffers
3. **Logging**: Use `plugin_log()` for debug messages to avoid interfering with output
4. **Initialization**: Return 0 from init functions on success, non-zero on error
5. **Cleanup**: Free any allocated resources in cleanup functions
6. **Thread Safety**: Current implementation is not thread-safe

## Limitations

- Maximum 10 plugins per system
- Output buffer limited to 1024 characters
- No dynamic loading of plugins (compile-time linking)
- Single-threaded execution model
- No plugin dependency management

## Validation

The plugin system maintains full compatibility with existing tests:

```bash
make test           # Original tests pass
make test-plugins   # Plugin functionality tests
```

Both the original hello.c behavior and plugin-enhanced behavior are validated to ensure system reliability.