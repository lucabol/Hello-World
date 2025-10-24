# Plugin System for hello.c

This document describes the plugin architecture that allows external developers to extend hello.c functionality without modifying the core file directly.

## Overview

The plugin system provides a clean, callback-based architecture for transforming the greeting message. Plugins can be:
- Created independently of hello.c
- Registered at runtime
- Chained together for complex transformations
- Easily distributed and shared

## Architecture

### Core Components

1. **plugin.h** - Plugin interface definition
   - Defines the plugin API contract
   - Declares plugin types and functions
   - Specifies constants (max message length, max plugin count)

2. **plugin.c** - Plugin system implementation
   - Plugin registration and management
   - Plugin execution engine
   - Chaining and transformation logic

3. **plugins_examples.c** - Example plugin implementations
   - Demonstrates how to create plugins
   - Includes ready-to-use transformation plugins

### Plugin Interface

A plugin consists of:
```c
typedef struct {
    const char* name;              /* Plugin identifier */
    plugin_transform_fn transform; /* Transformation function */
} plugin_t;
```

The transformation function signature:
```c
typedef int (*plugin_transform_fn)(const char* input, char* output, size_t output_size);
```

## Creating a Plugin

### Step 1: Implement the Transform Function

```c
static int my_transform(const char* input, char* output, size_t output_size) {
    if (input == NULL || output == NULL) {
        return -1;
    }
    
    /* Your transformation logic here */
    /* Example: copy input to output with modification */
    
    return 0;  /* Return 0 on success, -1 on error */
}
```

### Step 2: Define the Plugin Structure

```c
plugin_t my_plugin = {
    .name = "my_plugin",
    .transform = my_transform
};
```

### Step 3: Register and Use

```c
#include "plugin.h"

extern plugin_t my_plugin;

int main(void) {
    register_plugin(&my_plugin);
    
    char output[PLUGIN_MAX_MESSAGE_LEN];
    apply_plugins("Hello world!", output, sizeof(output));
    printf("%s\n", output);
    
    return 0;
}
```

## Example Plugins Provided

The repository includes several example plugins in `plugins_examples.c`:

### 1. Uppercase Plugin
Converts the message to uppercase.
```
Input:  "Hello world!"
Output: "HELLO WORLD!"
```

### 2. Reverse Plugin
Reverses the character order.
```
Input:  "Hello world!"
Output: "!dlrow olleH"
```

### 3. Exclaim Plugin
Adds extra exclamation marks.
```
Input:  "Hello world!"
Output: "Hello world!!!!"
```

### 4. Prefix Plugin
Adds a prefix tag.
```
Input:  "Hello world!"
Output: "[Plugin] Hello world!"
```

## Plugin Chaining

Multiple plugins can be chained together. They are applied in registration order:

```c
register_plugin(&uppercase_plugin);
register_plugin(&exclaim_plugin);

apply_plugins("Hello world!", output, sizeof(output));
/* Result: "HELLO WORLD!!!!" */
```

The output of one plugin becomes the input of the next.

## Building and Testing

### Build hello.c with plugin support:
```bash
gcc -o hello hello.c plugin.c
./hello
# Output: Hello world!
```

### Build with example plugins:
```bash
gcc -o hello_with_plugins hello_with_plugins.c plugin.c plugins_examples.c
./hello_with_plugins
```

### Run the full plugin demo:
```bash
gcc -o plugin_demo plugin_demo.c plugin.c plugins_examples.c
./plugin_demo
```

## API Reference

### Functions

#### `int register_plugin(plugin_t* plugin)`
Registers a plugin with the system.
- **Parameters**: Pointer to plugin structure
- **Returns**: 0 on success, -1 if registry is full
- **Max plugins**: 10 (PLUGIN_MAX_COUNT)

#### `int apply_plugins(const char* input, char* output, size_t output_size)`
Applies all registered plugins to a message.
- **Parameters**: 
  - `input`: Input message
  - `output`: Output buffer
  - `output_size`: Size of output buffer
- **Returns**: Number of plugins applied, or -1 on error

#### `int get_plugin_count(void)`
Returns the number of currently registered plugins.

#### `void clear_plugins(void)`
Clears all registered plugins from the system.

### Constants

- `PLUGIN_MAX_MESSAGE_LEN` (256): Maximum message length
- `PLUGIN_MAX_COUNT` (10): Maximum number of plugins

## Design Principles

1. **Minimal Core Changes**: hello.c was modified minimally to integrate the plugin system
2. **Zero Dependencies**: The plugin system has no external dependencies
3. **Type Safety**: Uses strong typing with function pointers
4. **Error Handling**: All functions return error codes
5. **Buffer Safety**: All operations respect buffer boundaries
6. **Composability**: Plugins can be freely combined and reordered

## Advanced Use Cases

### Creating Domain-Specific Plugins

Plugins can implement business logic:
- Language translation
- Formatting (colors, styles)
- Filtering (profanity, content moderation)
- Logging and monitoring
- Custom greetings based on time/context

### Plugin Distribution

Plugins can be:
- Compiled into separate object files
- Linked at compile time
- Distributed as source code
- Shared across projects

### Testing Plugins

Each plugin should:
1. Handle NULL inputs gracefully
2. Respect buffer size limits
3. Return appropriate error codes
4. Be tested in isolation
5. Be tested in combination with other plugins

## Limitations

- Maximum 10 plugins can be registered simultaneously
- Message length limited to 256 characters
- Plugins are applied sequentially (not parallel)
- No dynamic loading (compile-time linking only)

## Future Enhancements

Possible improvements:
- Dynamic plugin loading (dlopen/LoadLibrary)
- Plugin configuration system
- Plugin dependencies and requirements
- Async/parallel plugin execution
- Plugin versioning and compatibility checking

## Examples

See the following files for complete examples:
- `plugin_demo.c` - Comprehensive plugin demonstration
- `hello_with_plugins.c` - Integration example
- `plugins_examples.c` - Plugin implementation examples

## Security Considerations

When creating plugins:
- Validate all inputs
- Prevent buffer overflows
- Sanitize user data
- Handle errors gracefully
- Document security assumptions

## License

The plugin system follows the same license as hello.c.
