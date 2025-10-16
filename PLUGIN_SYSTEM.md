# Plugin System for hello.c

This plugin system allows external developers to extend the functionality of hello.c without modifying the core file directly.

## Architecture Overview

The plugin system consists of:

1. **plugin.h** - Plugin API interface
2. **plugin.c** - Plugin management implementation
3. **plugins/** - Directory for plugin implementations
4. **hello.c** - Core application (supports plugins via conditional compilation)
5. **hello_plugin_enabled.c** - Alternative main that demonstrates plugin usage

## Quick Start

### Basic Usage (No Plugins)

The default hello.c works without any plugins:

```bash
gcc -o hello hello.c
./hello
# Output: Hello world!
```

### Using the Plugin System

#### Option 1: Using hello_plugin_enabled.c

```bash
# Compile without plugins (default behavior)
gcc -o hello hello_plugin_enabled.c plugin.c
./hello
# Output: Hello world!

# Compile with uppercase plugin
gcc -o hello hello_plugin_enabled.c plugin.c plugins/example_uppercase.c -DLOAD_UPPERCASE
./hello
# Output: HELLO WORLD!

# Compile with reverse plugin
gcc -o hello hello_plugin_enabled.c plugin.c plugins/example_reverse.c -DLOAD_REVERSE
./hello
# Output: !dlrow olleH

# Compile with both plugins (chained transformation)
gcc -o hello hello_plugin_enabled.c plugin.c \
    plugins/example_uppercase.c plugins/example_reverse.c \
    -DLOAD_UPPERCASE -DLOAD_REVERSE
./hello
# Output: !DLROW OLLEH
```

#### Option 2: Using hello.c with ENABLE_PLUGINS flag

```bash
# Compile with plugin support enabled
gcc -DENABLE_PLUGINS -o hello hello.c plugin.c
./hello
# Output: Hello world! (no plugins loaded)
```

### Running the Demo

```bash
# Compile and run the plugin demo
gcc -o plugin_demo plugin_demo.c plugin.c \
    plugins/example_uppercase.c plugins/example_reverse.c
./plugin_demo
```

## Creating Custom Plugins

### Plugin Structure

A plugin consists of:
1. A transformation function that processes the message
2. An initialization function that registers the plugin

### Example Plugin

```c
/* plugins/my_plugin.c */
#include "../plugin.h"
#include <string.h>

/* Transform function */
static char* my_transform(const char* input, char* output, size_t output_size) {
    /* Your transformation logic here */
    strncpy(output, input, output_size - 1);
    output[output_size - 1] = '\0';
    
    /* Example: add prefix */
    char temp[256];
    snprintf(temp, sizeof(temp), "PREFIX: %s", output);
    strncpy(output, temp, output_size - 1);
    
    return output;
}

/* Initialization function */
void plugin_my_plugin_init(void) {
    plugin_register("my_plugin", "Adds a prefix to the message", my_transform);
}
```

### Using Your Plugin

```c
/* In your main program */
#include "plugin.h"

void plugin_my_plugin_init(void);  /* Declare plugin init function */

int main(void) {
    char output[256];
    const char* message = "Hello world!";
    
    /* Initialize your plugin */
    plugin_my_plugin_init();
    
    /* Apply transformations */
    plugin_apply_all(message, output, sizeof(output));
    printf("%s", output);
    
    return 0;
}
```

### Compile with Your Plugin

```bash
gcc -o hello your_main.c plugin.c plugins/my_plugin.c
```

## Plugin API Reference

### Data Types

```c
/* Plugin callback function type */
typedef char* (*plugin_transform_func)(const char* input, char* output, size_t output_size);

/* Plugin structure */
typedef struct {
    const char* name;                    /* Plugin name */
    const char* description;             /* Plugin description */
    plugin_transform_func transform;     /* Transformation function */
} plugin_t;
```

### Functions

#### `int plugin_register(const char* name, const char* description, plugin_transform_func transform)`

Register a new plugin.

- **Parameters:**
  - `name`: Plugin name (must be unique)
  - `description`: Brief description of what the plugin does
  - `transform`: Callback function for transforming messages
- **Returns:** 0 on success, -1 on failure (too many plugins)

#### `char* plugin_apply_all(const char* input, char* output, size_t output_size)`

Apply all registered plugins to a message.

- **Parameters:**
  - `input`: Original message to transform
  - `output`: Buffer for transformed message (at least 256 bytes)
  - `output_size`: Size of output buffer
- **Returns:** Pointer to output buffer

#### `int plugin_count(void)`

Get the number of registered plugins.

- **Returns:** Number of currently registered plugins

#### `void plugin_init(void)`

Initialize the plugin system (called automatically).

## Plugin Chaining

Plugins are applied in the order they are registered. The output of one plugin becomes the input to the next.

Example:
```
Original: "Hello world!"
After uppercase plugin: "HELLO WORLD!"
After reverse plugin: "!DLROW OLLEH"
```

## Included Example Plugins

### 1. Uppercase Plugin (plugins/example_uppercase.c)

Converts the message to uppercase.

```bash
gcc -o hello hello_plugin_enabled.c plugin.c plugins/example_uppercase.c -DLOAD_UPPERCASE
./hello
# Output: HELLO WORLD!
```

### 2. Reverse Plugin (plugins/example_reverse.c)

Reverses the message.

```bash
gcc -o hello hello_plugin_enabled.c plugin.c plugins/example_reverse.c -DLOAD_REVERSE
./hello
# Output: !dlrow olleH
```

## Design Principles

1. **Backward Compatibility**: The core hello.c maintains its original behavior when compiled without plugins
2. **Minimal Changes**: Plugins don't require modifying the core code
3. **Simple API**: Easy-to-use interface for plugin developers
4. **Flexibility**: Support for multiple plugins with chaining
5. **Safety**: Buffer overflow protection and error handling

## Limitations

- Maximum of 10 plugins can be registered (configurable via MAX_PLUGINS)
- Output buffer is limited to 256 bytes
- Plugins must be compiled together with the main program (no dynamic loading)
- Plugin order matters (first registered, first applied)

## Testing

The core functionality is validated with:

```bash
# Run validation tests
./test/validate.sh

# The default build must still pass all tests
gcc -o hello hello.c
./hello  # Must output exactly: Hello world!
```

## Advanced Usage

### Conditional Plugin Loading

You can create different builds with different plugin combinations:

```bash
# Build 1: No plugins
gcc -o hello_plain hello_plugin_enabled.c plugin.c

# Build 2: With uppercase only
gcc -o hello_upper hello_plugin_enabled.c plugin.c \
    plugins/example_uppercase.c -DLOAD_UPPERCASE

# Build 3: With all plugins
gcc -o hello_all hello_plugin_enabled.c plugin.c \
    plugins/example_uppercase.c plugins/example_reverse.c \
    -DLOAD_UPPERCASE -DLOAD_REVERSE
```

### Dynamic Plugin Selection

While the current implementation doesn't support runtime plugin loading, you can implement conditional initialization:

```c
int main(int argc, char* argv[]) {
    if (argc > 1 && strcmp(argv[1], "--uppercase") == 0) {
        plugin_uppercase_init();
    }
    /* ... rest of code ... */
}
```

## Contributing Plugins

To contribute a new plugin:

1. Create a new file in the `plugins/` directory
2. Implement the transformation function following the API
3. Implement the initialization function (name: `plugin_<name>_init`)
4. Document your plugin's behavior
5. Test with various inputs

Example naming convention:
- File: `plugins/example_<feature>.c`
- Init function: `void plugin_<feature>_init(void)`
- Transform function: `static char* <feature>_transform(...)`
