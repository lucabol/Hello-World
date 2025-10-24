# Plugin System Quick Start Guide

This guide shows how to create, register, and use plugins to extend hello.c functionality.

## Creating a Plugin

A plugin is a simple C function that transforms a message:

```c
#include "plugin.h"

/* Transform function - converts to uppercase */
static int my_uppercase(const char* input, char* output, size_t output_size) {
    size_t i, len = strlen(input);
    
    /* Check buffer size */
    if (len >= output_size) {
        return PLUGIN_ERR_BUFFER_TOO_SMALL;
    }
    
    /* Transform each character */
    for (i = 0; i < len; i++) {
        output[i] = toupper((unsigned char)input[i]);
    }
    output[len] = '\0';
    
    return PLUGIN_SUCCESS;
}

/* Plugin definition (must be static or global) */
plugin_t my_plugin = {
    .name = "uppercase",
    .transform = my_uppercase
};
```

## Registering and Using Plugins

```c
#include "plugin.h"
#include <stdio.h>

int main(void) {
    char output[PLUGIN_MAX_MESSAGE_LEN];
    int result;
    
    /* Register your plugin */
    result = plugin_register(&my_plugin);
    if (result != PLUGIN_SUCCESS) {
        fprintf(stderr, "Failed to register plugin: %d\n", result);
        return 1;
    }
    
    /* Apply all registered plugins */
    result = plugin_apply_all("Hello world!", output, sizeof(output));
    if (result >= 0) {
        printf("%s\n", output);  /* Output: HELLO WORLD! */
    } else {
        fprintf(stderr, "Plugin error: %d\n", result);
        return 1;
    }
    
    /* Cleanup (optional) */
    plugin_unregister("uppercase");
    
    return 0;
}
```

## Plugin Rules

### Memory & Ownership
- **Declare plugins as static or global** - they must remain valid while registered
- **Plugin names** must be string literals or static storage
- **Call `plugin_unregister()`** before freeing/unloading plugin code

### Transform Function Contract
- **Always NUL-terminate** output on success
- **Check buffer size** before writing
- **Return PLUGIN_SUCCESS (0)** on success
- **Return error code** (negative) on failure
- **Never write** beyond `output_size` bytes

### Thread Safety
- **⚠️ NOT THREAD-SAFE** - The plugin system is not thread-safe. If you need to use plugins from multiple threads, you must provide external synchronization.

**Recommended pattern for multi-threaded use:**

```c
#include <pthread.h>
#include "plugin.h"

/* Global mutex to protect plugin operations */
static pthread_mutex_t plugin_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Thread-safe plugin registration */
int safe_plugin_register(plugin_t* plugin) {
    pthread_mutex_lock(&plugin_mutex);
    int result = plugin_register(plugin);
    pthread_mutex_unlock(&plugin_mutex);
    return result;
}

/* Thread-safe plugin application */
int safe_plugin_apply_all(const char* input, char* output, size_t size) {
    pthread_mutex_lock(&plugin_mutex);
    int result = plugin_apply_all(input, output, size);
    pthread_mutex_unlock(&plugin_mutex);
    return result;
}

/* Thread-safe plugin unregistration */
int safe_plugin_unregister(const char* name) {
    pthread_mutex_lock(&plugin_mutex);
    int result = plugin_unregister(name);
    pthread_mutex_unlock(&plugin_mutex);
    return result;
}
```

**Important:** Never call `plugin_unregister()` or `plugin_clear()` while `plugin_apply_all()` is running - this causes undefined behavior even with external synchronization

## Error Codes

| Code | Value | Meaning |
|------|-------|---------|
| `PLUGIN_SUCCESS` | 0 | Operation succeeded |
| `PLUGIN_ERR_NULL_POINTER` | -1 | NULL pointer argument |
| `PLUGIN_ERR_INVALID_PLUGIN` | -2 | Invalid plugin structure |
| `PLUGIN_ERR_REGISTRY_FULL` | -3 | Maximum plugins registered |
| `PLUGIN_ERR_BUFFER_TOO_SMALL` | -4 | Output buffer too small |
| `PLUGIN_ERR_TRANSFORM_FAILED` | -5 | Plugin transform failed |
| `PLUGIN_ERR_PLUGIN_NOT_FOUND` | -6 | Plugin not found |

## API Functions

### Registration
- `int plugin_register(plugin_t* plugin)` - Register a plugin
- `int plugin_unregister(const char* name)` - Unregister by name
- `void plugin_clear(void)` - Clear all plugins

### Query
- `int plugin_get_count(void)` - Get number of registered plugins
- `int plugin_get_capacity(void)` - Get maximum capacity (default 10)

### Execution
- `int plugin_apply_all(const char* input, char* output, size_t size)` - Apply all plugins

## Plugin Chaining

Plugins are applied in registration order:

```c
plugin_register(&uppercase_plugin);  /* First: "hello" → "HELLO" */
plugin_register(&exclaim_plugin);     /* Second: "HELLO" → "HELLO!!!" */

plugin_apply_all("hello", output, sizeof(output));
/* Result: "HELLO!!!" */
```

## Building

```bash
# Compile your plugin with the system
gcc -o myapp myapp.c plugin.c -I. -Wall -Wextra

# Optionally configure maximum plugins at compile time
gcc -DPLUGIN_MAX_COUNT=20 -o myapp myapp.c plugin.c
```

## Examples

See these files for complete examples:
- `plugins_examples.c` - Built-in example plugins (uppercase, reverse, exclaim, prefix)
- `custom_plugin_example.c` - Custom plugins (ROT13, leetspeak)
- `plugin_demo.c` - Comprehensive demonstration

## Best Practices

1. **Always check return values** from plugin functions
2. **Use safe string operations** (avoid strcpy/strcat)
3. **Test buffer boundaries** in your transform functions
4. **Document your plugin's** transformation behavior
5. **Keep transforms fast** - they're called in sequence
6. **Handle errors gracefully** - return proper error codes
7. **Don't unregister during apply** - undefined behavior

For complete API documentation, see `PLUGIN_SYSTEM.md`.
