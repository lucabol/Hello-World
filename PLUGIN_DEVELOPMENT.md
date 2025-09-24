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
gcc -Wall -Wextra -std=c99 -fPIC -shared -I. -o plugins/my_plugin.so my_plugin.c
```

## API Reference

### Types

- `plugin_info_t`: Contains plugin metadata (name, version, description)
  - `name`: Unique plugin identifier (null-terminated string)
  - `version`: Must equal `PLUGIN_API_VERSION` for compatibility
  - `description`: Human-readable description of plugin functionality

- `message_transformer_t`: Function pointer type for message transformers
  ```c
  typedef int (*message_transformer_t)(const char* input, char* output, size_t output_size);
  ```
  - Returns 0 on success, -1 on error
  - Must null-terminate output string
  - Must not write more than (output_size-1) characters plus null terminator

- `plugin_init_t`: Function pointer type for plugin initialization
  ```c  
  typedef plugin_info_t (*plugin_init_t)(void);
  ```

### Functions

**Core API Functions (called by plugins):**
- `register_transformer(name, transformer)`: Register a message transformer function
  - Returns 0 on success, -1 if registration failed
  - `name`: Unique identifier for debugging/logging
  - `transformer`: Function pointer to transformer implementation

**Internal Functions (called by main program):**
- `apply_transformers(input, output, size)`: Apply all registered transformers to input
- `load_plugins_from_directory(path)`: Load all .so files from directory
  - Returns number of plugins loaded, -1 on directory access error
  - Validates ABI version compatibility
  - Gracefully handles malformed plugins
- `cleanup_plugins()`: Clean up loaded plugins and reset state

### Constants

- `PLUGIN_API_VERSION`: Current plugin API version (1)
- `MAX_MESSAGE_SIZE`: Maximum message buffer size (256 bytes)
- `MAX_PLUGINS`: Maximum number of plugins (32)

### ABI Compatibility and Versioning

The plugin system enforces strict ABI compatibility:

1. **Version Checking**: Plugins must return `PLUGIN_API_VERSION` in their `plugin_info_t`
2. **Function Signatures**: All function signatures must match exactly
3. **Buffer Safety**: Plugins must respect buffer size limits
4. **Symbol Requirements**: Plugins must export `plugin_init` function

**Breaking Changes**: Any change to function signatures, struct layout, or behavior requires incrementing `PLUGIN_API_VERSION`.

### Memory Management and Thread Safety

**Memory Ownership:**
- Input strings: Read-only, valid only during transformer call
- Output buffers: Provided by caller, plugins write to them
- Plugin metadata: Plugins must provide static/persistent strings

**Thread Safety:**
- The plugin system is **NOT thread-safe**
- All operations are single-threaded
- Plugins should be stateless for best practices

**Lifetime:**
- Plugins are loaded once at startup
- Plugin handles remain valid until `cleanup_plugins()` is called
- Transformer functions may be called multiple times

## Example Plugins

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

## Complete Example Plugin

Here's a complete, compilable example plugin:

```c
// File: plugins/example.c
#include "../plugin.h"
#include <string.h>
#include <ctype.h>

/* Example transformer: converts first character to uppercase */
static int capitalize_transform(const char* input, char* output, size_t output_size) {
    size_t len = strlen(input);
    
    /* Check buffer size */
    if (len >= output_size) {
        return -1;  /* Buffer too small */
    }
    
    /* Copy input to output */
    strcpy(output, input);
    
    /* Capitalize first character if it's alphabetic */
    if (len > 0 && isalpha((unsigned char)output[0])) {
        output[0] = (char)toupper((unsigned char)output[0]);
    }
    
    return 0;
}

/* Required plugin initialization function */
plugin_info_t plugin_init(void) {
    plugin_info_t info = {
        .name = "capitalize",
        .version = PLUGIN_API_VERSION,  /* Must match! */
        .description = "Capitalizes the first character of the message"
    };
    
    /* Register our transformer */
    register_transformer("capitalize", capitalize_transform);
    
    return info;
}
```

**Build this example:**
```bash
gcc -Wall -Wextra -std=c99 -fPIC -shared -I. -o plugins/example.so plugins/example.c
```

## Example Combinations

- `uppercase.so` only: "Hello world!" → "HELLO WORLD!"
- `reverse.so` only: "Hello world!" → "!dlrow olleH"  
- `exclamation.so` only: "Hello world!" → "Hello world!!!"
- `uppercase.so` + `exclamation.so`: "Hello world!" → "HELLO WORLD!!!"
- All plugins: "Hello world!" → varies by loading order

## Building and Testing

### Build Everything
```bash
make all plugins
```

### Test Without Plugins
```bash
rm -f plugins/*.so
./hello
# Output: Hello world!
```

### Test With Plugins
```bash
make plugins
./hello
# Output: Transformed based on loaded plugins
```

### Comprehensive Testing
```bash
# Run full plugin test suite
make test-plugins

# Run basic validation
make test

# Run strict validation (with -Werror)
make test-strict
```

### Validation
The core validation tests expect the original "Hello world!" output, so remove plugin files before running tests:
```bash
rm -f plugins/*.so
make test
```

## Plugin Execution Order

Plugins are loaded in the order they appear in the plugins directory (alphabetical order). Transformers are applied sequentially, with each transformer's output becoming the input for the next transformer.

## Error Handling Best Practices

### Plugin Implementation
1. **Always validate input parameters** (null checks, size checks)
2. **Return appropriate error codes** (0 for success, -1 for errors)
3. **Handle edge cases gracefully** (empty strings, very long strings)
4. **Null-terminate all output strings**
5. **Don't rely on plugin load order**

### Error Scenarios Handled by Core
- **Missing plugin_init**: Plugin skipped with error message
- **Wrong ABI version**: Plugin rejected with version mismatch message
- **Transformer failures**: Individual transformers skipped, others continue
- **Buffer overflows**: Core validates transformer output
- **Dynamic loading errors**: Clear dlopen/dlsym error messages
- **Directory access**: Gracefully handles missing plugin directory

## Security Considerations

**⚠️ IMPORTANT SECURITY NOTES:**

1. **Arbitrary Code Execution**: Loading plugins means executing arbitrary shared libraries
2. **No Sandboxing**: Plugins run with full process privileges  
3. **Trust Model**: Only load plugins from trusted sources
4. **Directory Security**: Secure the plugins/ directory against unauthorized writes
5. **ABI Validation**: The system validates versions but cannot prevent malicious code

**Recommended Security Practices:**
- Restrict write access to the plugins directory
- Use file permissions to control plugin loading
- Consider running in a chroot/container environment
- Implement plugin signing/verification for production use
- Regular security audits of plugin code

## Debugging and Diagnostics

**Plugin Loading Debug Output:**
```bash
# Run with plugin loading messages
./hello
# Output includes:
# Loaded plugin: uppercase - Converts message to uppercase
# Warning: transformer 'broken' failed, skipping
```

**Common Error Messages:**
- `Plugin missing plugin_init function`: Plugin lacks required function
- `Plugin has incompatible ABI version`: Version mismatch  
- `Maximum number of transformers reached`: Too many plugins loaded
- `transformer failed, skipping`: Individual transformer returned error

**Debugging Tips:**
1. **Check plugin compilation**: Ensure `-fPIC -shared` flags used
2. **Verify function exports**: Use `nm -D plugin.so | grep plugin_init`
3. **Test individually**: Load one plugin at a time to isolate issues
4. **Check dependencies**: Use `ldd plugin.so` to verify dependencies

## Build System Integration

**Plugin-Specific Compilation Variables:**
- `PLUGIN_CFLAGS`: Includes `-fPIC` and include path
- `PLUGIN_FLAGS`: Shared library linking flags
- `DYNAMIC_FLAGS`: Dynamic linking support (includes conditional -rdynamic)

**Portability Notes:**
- `-rdynamic`: GNU ld specific, automatically detected and conditional
- `-fPIC`: Required for shared libraries on most platforms
- `dlopen/dlsym`: POSIX standard, widely available

## Best Practices Summary

1. **Always check buffer sizes** to prevent overflows
2. **Handle edge cases gracefully** (empty strings, etc.)
3. **Return appropriate error codes** (-1 for errors, 0 for success)
4. **Keep transformations simple and focused**
5. **Test plugins individually and in combination**
6. **Use meaningful names** for plugins and transformers
7. **Follow ABI contract strictly** (version, null termination, etc.)
8. **Be stateless** for thread safety and predictability

## Troubleshooting

### Symbol Not Found Errors
Make sure the main executable is built with `-rdynamic` flag to export symbols to plugins.

### Plugin Not Loading
Check that:
- Plugin file has `.so` extension and ends with `.so` (not just contains it)
- Plugin is in the `plugins/` directory  
- Plugin implements `plugin_init()` function with correct signature
- Plugin compilation succeeded without errors
- Plugin returns correct ABI version

### Compilation Errors
Ensure you're using the correct compiler flags:
```bash
gcc -Wall -Wextra -std=c99 -fPIC -shared -I. -o plugins/plugin.so plugins/plugin.c
```

### Runtime Failures
- Check plugin loading messages for ABI version mismatches
- Verify transformer functions handle all edge cases
- Test with individual plugins to isolate problematic code
- Use debugging tools like `gdb` or `valgrind` for memory issues

This comprehensive documentation covers the complete plugin API and should be sufficient for external developers to create compatible plugins safely and efficiently.