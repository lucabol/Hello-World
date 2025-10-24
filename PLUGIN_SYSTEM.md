# Plugin System for hello.c

This document describes the plugin architecture that allows external developers to extend hello.c functionality without modifying the core file directly.

## Overview

The plugin system provides a clean, callback-based architecture for transforming the greeting message. Plugins can be:
- Created independently of hello.c
- Registered at runtime
- Chained together for complex transformations
- Easily distributed and shared

## Table of Contents

- [Architecture](#architecture)
- [API Reference](#api-reference)
- [Creating a Plugin](#creating-a-plugin)
- [Error Handling](#error-handling)
- [Thread Safety](#thread-safety)
- [Memory and Ownership](#memory-and-ownership)
- [Plugin Chaining](#plugin-chaining)
- [Example Plugins](#example-plugins)
- [Building and Testing](#building-and-testing)
- [Security Considerations](#security-considerations)

## Architecture

### Core Components

1. **plugin.h** - Plugin interface definition
   - Defines the plugin API contract
   - Declares plugin types and functions
   - Specifies constants (max message length, max plugin count)
   - Documents error codes and return values

2. **plugin.c** - Plugin system implementation
   - Plugin registration and management
   - Plugin execution engine
   - Chaining and transformation logic
   - Error handling and validation

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

### Error Codes

```c
typedef enum {
    PLUGIN_SUCCESS = 0,              /* Operation succeeded */
    PLUGIN_ERR_NULL_POINTER = -1,    /* NULL pointer argument */
    PLUGIN_ERR_INVALID_PLUGIN = -2,  /* Invalid plugin structure */
    PLUGIN_ERR_REGISTRY_FULL = -3,   /* Plugin registry is full */
    PLUGIN_ERR_BUFFER_TOO_SMALL = -4, /* Output buffer too small */
    PLUGIN_ERR_TRANSFORM_FAILED = -5, /* Plugin transform function failed */
    PLUGIN_ERR_PLUGIN_NOT_FOUND = -6  /* Plugin not found in registry */
} plugin_error_t;
```

## API Reference

### plugin_register()

```c
int plugin_register(plugin_t* plugin);
```

Registers a plugin with the system.

**Parameters:**
- `plugin`: Pointer to plugin structure (must be non-NULL)

**Returns:**
- `PLUGIN_SUCCESS` on success
- `PLUGIN_ERR_NULL_POINTER` if plugin is NULL
- `PLUGIN_ERR_INVALID_PLUGIN` if plugin->name or plugin->transform is NULL
- `PLUGIN_ERR_REGISTRY_FULL` if maximum plugins already registered (default: 10)

**Thread Safety:** Not thread-safe

**Ownership:** Caller retains ownership of plugin structure. Plugin must remain valid for the lifetime of registration.

**Example:**
```c
plugin_t my_plugin = {
    .name = "my_plugin",
    .transform = my_transform_function
};

int result = plugin_register(&my_plugin);
if (result != PLUGIN_SUCCESS) {
    fprintf(stderr, "Failed to register plugin: %d\n", result);
}
```

### plugin_unregister()

```c
int plugin_unregister(const char* name);
```

Unregisters a plugin by name.

**Parameters:**
- `name`: Name of the plugin to unregister (must be non-NULL)

**Returns:**
- `PLUGIN_SUCCESS` on success
- `PLUGIN_ERR_NULL_POINTER` if name is NULL
- `PLUGIN_ERR_PLUGIN_NOT_FOUND` if no plugin with that name exists

**Thread Safety:** Not thread-safe

**Example:**
```c
int result = plugin_unregister("my_plugin");
if (result == PLUGIN_SUCCESS) {
    printf("Plugin unregistered successfully\n");
}
```

### plugin_apply_all()

```c
int plugin_apply_all(const char* input, char* output, size_t output_size);
```

Applies all registered plugins to a message in registration order.

**Parameters:**
- `input`: The input message (must be non-NULL, null-terminated)
- `output`: Buffer to write the final transformed message (must be non-NULL)
- `output_size`: Size of the output buffer (must be > 0)

**Returns:**
- Number of plugins successfully applied (>= 0) on success
- `PLUGIN_ERR_NULL_POINTER` if input or output is NULL
- `PLUGIN_ERR_BUFFER_TOO_SMALL` if output_size is 0 or input doesn't fit
- `PLUGIN_ERR_TRANSFORM_FAILED` if a plugin transformation fails

**Behavior on Plugin Failure:**
If a plugin returns non-zero, processing stops immediately. The output buffer will contain the result of the last successful transformation. This allows callers to see partial progress and aids debugging.

**Buffer Behavior:**
- Output is always NUL-terminated on success
- If truncation is needed, output_size - 1 bytes are written plus NUL
- Truncation signals error via return code

**Thread Safety:** Not thread-safe

**Example:**
```c
char output[256];
int result = plugin_apply_all("Hello world!", output, sizeof(output));
if (result >= 0) {
    printf("Applied %d plugins: %s\n", result, output);
} else {
    fprintf(stderr, "Plugin application failed: %d\n", result);
}
```

### plugin_get_count()

```c
int plugin_get_count(void);
```

Returns the number of currently registered plugins.

**Returns:** Number of registered plugins (>= 0)

**Thread Safety:** Not thread-safe (read-only but no synchronization)

### plugin_clear()

```c
void plugin_clear(void);
```

Removes all plugins from the registry. Does not free plugin structures (caller retains ownership).

**Thread Safety:** Not thread-safe

**Use Case:** Primarily for test cleanup and resetting state.

## Creating a Plugin

### Step 1: Implement the Transform Function

```c
static int my_transform(const char* input, char* output, size_t output_size) {
    /* Validate buffer size */
    size_t input_len = strlen(input);
    if (input_len >= output_size) {
        return PLUGIN_ERR_BUFFER_TOO_SMALL;
    }
    
    /* Perform transformation using safe string operations */
    strncpy(output, input, output_size - 1);
    output[output_size - 1] = '\0';
    
    /* Add your transformation here */
    strncat(output, " (modified)", output_size - strlen(output) - 1);
    
    return PLUGIN_SUCCESS;
}
```

**Transform Function Contract:**
- Input is guaranteed non-NULL and NUL-terminated
- Output buffer is guaranteed non-NULL with output_size > 0
- Must not write beyond output_size bytes
- Must NUL-terminate output on success
- Should return `PLUGIN_ERR_BUFFER_TOO_SMALL` if output won't fit
- Should use safe string operations (strncpy, strncat, snprintf)

### Step 2: Define the Plugin Structure

```c
plugin_t my_plugin = {
    .name = "my_plugin",
    .transform = my_transform
};
```

**Plugin Structure Requirements:**
- Must be static or global (not stack-allocated)
- Name must be a string literal or static storage
- Both fields must be non-NULL

### Step 3: Register and Use

```c
#include "plugin.h"

int main(void) {
    /* Register the plugin */
    if (plugin_register(&my_plugin) != PLUGIN_SUCCESS) {
        fprintf(stderr, "Failed to register plugin\n");
        return 1;
    }
    
    /* Apply plugins */
    char output[PLUGIN_MAX_MESSAGE_LEN];
    int result = plugin_apply_all("Hello world!", output, sizeof(output));
    
    if (result >= 0) {
        printf("%s\n", output);
    } else {
        fprintf(stderr, "Error: %d\n", result);
    }
    
    return 0;
}
```

## Error Handling

### Return Code Checking

Always check return codes from plugin API functions:

```c
int result = plugin_register(&my_plugin);
switch (result) {
    case PLUGIN_SUCCESS:
        printf("Plugin registered successfully\n");
        break;
    case PLUGIN_ERR_REGISTRY_FULL:
        fprintf(stderr, "Plugin registry is full\n");
        break;
    case PLUGIN_ERR_INVALID_PLUGIN:
        fprintf(stderr, "Invalid plugin structure\n");
        break;
    default:
        fprintf(stderr, "Unknown error: %d\n", result);
        break;
}
```

### Plugin Failure Behavior

When a plugin fails during `plugin_apply_all()`:

```
Input: "Hello"
  ↓ Plugin 1 (uppercase) - SUCCESS
"HELLO"
  ↓ Plugin 2 (failing plugin) - FAILURE
Returns: PLUGIN_ERR_TRANSFORM_FAILED
Output contains: "HELLO" (last successful result)
```

This allows debugging by seeing where the chain stopped.

## Thread Safety

**The plugin system is NOT thread-safe.** All operations must be:
- Performed from a single thread, OR
- Protected by external synchronization (mutex, etc.)

This includes:
- Plugin registration/unregistration
- Plugin application
- Reading plugin count

**Rationale:** The global plugin registry is not protected by locks to minimize overhead in single-threaded use cases (the most common scenario).

**Multi-threaded Usage:**
```c
pthread_mutex_t plugin_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Registering from multiple threads */
pthread_mutex_lock(&plugin_mutex);
plugin_register(&my_plugin);
pthread_mutex_unlock(&plugin_mutex);

/* Applying from multiple threads */
pthread_mutex_lock(&plugin_mutex);
plugin_apply_all(input, output, size);
pthread_mutex_unlock(&plugin_mutex);
```

## Memory and Ownership

### Plugin Structures

**Ownership:** Caller owns plugin structures
**Lifetime:** Must remain valid while registered
**Storage:** Use static or global variables

```c
/* CORRECT: Static storage */
static plugin_t my_plugin = {
    .name = "my_plugin",
    .transform = my_transform
};

/* CORRECT: Global variable */
plugin_t my_plugin = {
    .name = "my_plugin",
    .transform = my_transform
};

/* WRONG: Stack allocation */
void register_my_plugin(void) {
    plugin_t my_plugin = { /* ... */ };  /* Goes out of scope! */
    plugin_register(&my_plugin);  /* DANGER: Dangling pointer */
}
```

### Plugin Names

Names must be string literals or static storage:

```c
/* CORRECT: String literal */
plugin_t plugin1 = { .name = "plugin1", /* ... */ };

/* CORRECT: Static storage */
static const char name[] = "plugin2";
plugin_t plugin2 = { .name = name, /* ... */ };

/* WRONG: Stack buffer */
void register_plugin(void) {
    char name[20];
    sprintf(name, "plugin%d", 1);  /* Stack-allocated */
    plugin_t p = { .name = name, /* ... */ };  /* DANGER */
    plugin_register(&p);
}
```

## Plugin Chaining

### Execution Order

Plugins are applied in registration order:

```c
plugin_register(&uppercase_plugin);  /* Applied first */
plugin_register(&exclaim_plugin);    /* Applied second */

plugin_apply_all("hello", output, sizeof(output));
/* Result: "HELLO!!!" */
```

### Chaining Diagram

```
Input: "Hello world!"
  ↓
  ├─ Plugin 1: uppercase
  │    ↓
  │  "HELLO WORLD!"
  │    ↓
  ├─ Plugin 2: exclaim
  │    ↓
  │  "HELLO WORLD!!!!"
  ↓
Output: "HELLO WORLD!!!!"
```

### Order Matters

```c
/* Order 1: Reverse then uppercase */
plugin_register(&reverse_plugin);
plugin_register(&uppercase_plugin);
plugin_apply_all("hello", output, sizeof(output));
/* Result: "OLLEH" */

plugin_clear();

/* Order 2: Uppercase then reverse */
plugin_register(&uppercase_plugin);
plugin_register(&reverse_plugin);
plugin_apply_all("hello", output, sizeof(output));
/* Result: "OLLEH" (same in this case) */
```

## Example Plugins

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

See `custom_plugin_example.c` for additional examples including ROT13 and Leetspeak transformations.

## Building and Testing

### Build hello.c with plugin support:
```bash
make hello
./hello  # Output: Hello world!
```

### Build and run all demonstrations:
```bash
make demo
```

### Run comprehensive unit tests:
```bash
make unit-test  # 46 tests covering edge cases and error conditions
```

### Run tests with memory sanitizers:
```bash
make asan  # AddressSanitizer + UndefinedBehaviorSanitizer
```

### Full CI build:
```bash
make ci  # Strict compilation + all tests
```

### Build with custom plugin limit:
```bash
gcc -DPLUGIN_MAX_COUNT=20 -o hello hello.c plugin.c
```

## Security Considerations

### Input Validation

Always validate inputs in transform functions:
```c
static int safe_transform(const char* input, char* output, size_t output_size) {
    /* Validate buffer size */
    if (strlen(input) >= output_size) {
        return PLUGIN_ERR_BUFFER_TOO_SMALL;
    }
    
    /* Use safe string operations */
    strncpy(output, input, output_size - 1);
    output[output_size - 1] = '\0';
    
    return PLUGIN_SUCCESS;
}
```

### Buffer Safety

Use safe string functions:
- ✅ `strncpy(dest, src, n)` instead of `strcpy()`
- ✅ `strncat(dest, src, n)` instead of `strcat()`
- ✅ `snprintf(dest, n, fmt, ...)` instead of `sprintf()`

### Sanitize User Data

If plugins process user input, sanitize it:
```c
/* Remove dangerous characters */
for (i = 0; input[i]; i++) {
    if (!isalnum(input[i]) && !isspace(input[i])) {
        output[i] = '_';  /* Replace with safe character */
    } else {
        output[i] = input[i];
    }
}
```

### Handle Errors Gracefully

Never assume success:
```c
int result = plugin_apply_all(user_input, output, sizeof(output));
if (result < 0) {
    /* Use safe fallback */
    strncpy(output, "Error processing input", sizeof(output) - 1);
    output[sizeof(output) - 1] = '\0';
}
```

## Configuration

### Compile-Time Options

**PLUGIN_MAX_COUNT**: Maximum number of plugins (default: 10)
```bash
gcc -DPLUGIN_MAX_COUNT=20 -o hello hello.c plugin.c
```

## Limitations

- Maximum 10 plugins by default (configurable)
- Message length limited to 256 characters
- Plugins are applied sequentially (not parallel)
- No dynamic loading (compile-time linking only)
- Not thread-safe without external synchronization

## Future Enhancements

Possible improvements:
- Dynamic plugin loading (dlopen/LoadLibrary)
- Plugin configuration system
- Plugin dependencies and requirements
- Thread-safe implementation with mutexes
- Async/parallel plugin execution
- Plugin versioning and compatibility checking

## Examples

See the following files for complete examples:
- `plugin_demo.c` - Comprehensive plugin demonstration
- `hello_with_plugins.c` - Integration example
- `custom_plugin_example.c` - Custom plugin implementations
- `test/test_plugin_unit.c` - Unit test examples

## License

The plugin system follows the same license as hello.c.
