# Plugin Development Guide

## Overview

The Hello World program supports a plugin architecture that allows external developers to extend its functionality without modifying the core `hello.c` file. Plugins can transform or modify the greeting message through a simple, well-defined interface.

## Plugin Architecture

### Design Principles

- **Non-invasive**: Plugins do not require modifications to `hello.c`
- **Compile-time**: Plugins are statically linked at compilation
- **Sequential processing**: Multiple plugins can be chained together
- **Simple interface**: Easy to implement and understand

### How It Works

1. Plugins implement the `plugin_interface` structure defined in `plugin.h`
2. Plugins register themselves using the `REGISTER_PLUGIN` macro
3. At runtime, registered plugins are automatically called in registration order
4. Each plugin can transform the message passed to it
5. The final transformed message is output by the program

## Creating a Plugin

### Basic Plugin Structure

```c
#include "../plugin.h"
#include <string.h>

/* Static buffer for transformed message */
static char my_buffer[256];

/* Transformation function */
static char* my_transform(const char* input) {
    /* Your transformation logic here */
    snprintf(my_buffer, sizeof(my_buffer), "Transformed: %s", input);
    return my_buffer;
}

/* Plugin definition */
static plugin_interface my_plugin = {
    .process_message = my_transform,
    .plugin_name = "MyPlugin",
    .plugin_version = "1.0.0"
};

/* Register the plugin */
REGISTER_PLUGIN(my_plugin);
```

### Plugin Interface Reference

```c
typedef struct {
    char* (*process_message)(const char* input);
    const char* plugin_name;
    const char* plugin_version;
} plugin_interface;
```

**Fields:**

- `process_message`: Function pointer that transforms the input message
  - **Input**: Pointer to the message to transform
  - **Output**: Pointer to the transformed message (must be static storage)
  - **Return NULL**: To skip transformation (pass through)
  
- `plugin_name`: String identifying the plugin (for debugging)

- `plugin_version`: Version string for the plugin

### Important Considerations

1. **Memory Management**
   - Return pointers to static storage or global buffers
   - Do NOT return pointers to stack variables
   - Do NOT use malloc() without ensuring proper cleanup

2. **Thread Safety**
   - Static buffers are not thread-safe
   - Consider using thread-local storage for concurrent applications

3. **Buffer Sizes**
   - Ensure your buffer is large enough for transformed messages
   - Check bounds to prevent buffer overflows

4. **Return Value**
   - Return NULL to indicate "no transformation" (pass through)
   - Return a valid pointer to indicate transformation

## Building with Plugins

### Without Plugins (Default)

```bash
gcc -o hello hello.c plugin.c
./hello
# Output: Hello world!
```

### With a Single Plugin

```bash
gcc -o hello hello.c plugin.c plugins/example_plugin.c
./hello
# Output: HELLO WORLD! (uppercase transformation)
```

### With Multiple Plugins (Chaining)

```bash
gcc -o hello hello.c plugin.c plugins/prefix_plugin.c plugins/example_plugin.c
./hello
# Output: *** HELLO WORLD! (prefix added, then uppercased)
```

Plugins are applied in the order they appear in the compilation command.

### Compilation Flags

For development:
```bash
gcc -Wall -Wextra -o hello hello.c plugin.c plugins/your_plugin.c
```

For production (strict):
```bash
gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -o hello hello.c plugin.c plugins/your_plugin.c
```

## Example Plugins

### Example 1: Uppercase Transformer

See `plugins/example_plugin.c` - Converts the message to uppercase.

### Example 2: Prefix Transformer

See `plugins/prefix_plugin.c` - Adds a prefix to the message.

### Example 3: Reverse Plugin (DIY Exercise)

Create a plugin that reverses the greeting message:

```c
#include "../plugin.h"
#include <string.h>

static char reverse_buffer[256];

static char* reverse_transform(const char* input) {
    size_t len = strlen(input);
    size_t i;
    
    if (len >= sizeof(reverse_buffer)) {
        len = sizeof(reverse_buffer) - 1;
    }
    
    for (i = 0; i < len; i++) {
        reverse_buffer[i] = input[len - 1 - i];
    }
    reverse_buffer[len] = '\0';
    
    return reverse_buffer;
}

static plugin_interface reverse_plugin = {
    .process_message = reverse_transform,
    .plugin_name = "ReversePlugin",
    .plugin_version = "1.0.0"
};

REGISTER_PLUGIN(reverse_plugin);
```

## Advanced Topics

### Conditional Transformation

Plugins can conditionally apply transformations:

```c
static char* conditional_transform(const char* input) {
    /* Only transform if message starts with "Hello" */
    if (strncmp(input, "Hello", 5) == 0) {
        /* Apply transformation */
        snprintf(buffer, sizeof(buffer), "Greetings: %s", input);
        return buffer;
    }
    /* Return NULL to skip transformation */
    return NULL;
}
```

### Disabling Plugin Debug Output

By default, plugins print registration messages to stderr. To disable:

```bash
gcc -DSILENT_PLUGINS -o hello hello.c plugin.c plugins/your_plugin.c
```

### Maximum Plugins

The system supports up to 16 plugins by default (defined by `MAX_PLUGINS` in `plugin.h`). To change this limit, modify the `MAX_PLUGINS` constant.

## Testing Your Plugin

1. **Compile with your plugin**:
   ```bash
   gcc -Wall -Wextra -o hello_test hello.c plugin.c plugins/your_plugin.c
   ```

2. **Run and verify output**:
   ```bash
   ./hello_test
   ```

3. **Test with strict flags**:
   ```bash
   gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -o hello_test hello.c plugin.c plugins/your_plugin.c
   ```

4. **Test plugin chaining**:
   ```bash
   gcc -o hello_test hello.c plugin.c plugins/plugin1.c plugins/your_plugin.c plugins/plugin3.c
   ```

## Best Practices

1. **Keep it simple**: Plugins should do one thing well
2. **Use descriptive names**: Make plugin names self-documenting
3. **Handle edge cases**: Check for NULL input, empty strings, etc.
4. **Use static buffers**: Avoid dynamic allocation for simplicity
5. **Version your plugins**: Use semantic versioning
6. **Document your plugin**: Add comments explaining the transformation
7. **Test thoroughly**: Test with various input messages

## Troubleshooting

### Plugin Not Registering

- Ensure you used `REGISTER_PLUGIN` macro
- Check that the plugin is being compiled and linked
- Verify the constructor attribute is supported by your compiler

### Buffer Overflow

- Check buffer sizes in your transformation function
- Use `snprintf` instead of `sprintf`
- Validate input length before transforming

### Unexpected Output

- Verify plugin order in compilation command
- Check if any plugin returns NULL (skips transformation)
- Enable debug output (remove `-DSILENT_PLUGINS` flag)

## Contributing

When contributing plugins:

1. Place your plugin in the `plugins/` directory
2. Name your file descriptively (e.g., `rot13_plugin.c`)
3. Include documentation in the file header
4. Test with strict compilation flags
5. Provide usage examples

## API Stability

The plugin interface is designed to be stable. Future versions will maintain backward compatibility with existing plugins where possible. Any breaking changes will be clearly documented in release notes.
