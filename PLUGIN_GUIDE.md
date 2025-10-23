# Plugin System for hello.c

## Overview

The plugin system allows external developers to extend the functionality of hello.c without modifying the core file directly. Plugins can transform the output message before it's printed.

This system provides a clean, extensible architecture that maintains backward compatibility with the original hello.c program.

**Key Features:**
- Safe plugin chaining with dual staging buffers
- Comprehensive error handling with specific error codes
- Configurable limits (compile-time override)
- Duplicate detection and validation
- Thread-safety documentation and limitations

**Safety Guarantees:**
- No buffer overflows (all plugins validate buffer sizes)
- Safe chaining even when plugins use static buffers (ping-pong buffering)
- NULL pointer validation at all API boundaries
- Tested with ASAN and UBSAN (no issues found)

## Thread Safety and Limitations

⚠️ **NOT THREAD-SAFE**: This implementation uses static buffers for performance and simplicity.

- Do not call plugin functions from multiple threads concurrently
- Do not call plugin functions reentrantly (e.g., from signal handlers)
- For thread-safe usage, you must implement external synchronization (e.g., mutexes)

## C++ Compatibility

The plugin system is C++ compatible. The header file includes `extern "C"` guards:

```cpp
#include <iostream>
#include "plugin.h"

extern const plugin_t uppercase_plugin;

int main() {
    plugin_init();
    
    if (plugin_register(&uppercase_plugin) != PLUGIN_SUCCESS) {
        std::cerr << "Failed to register plugin" << std::endl;
        return 1;
    }
    
    const char* result = plugin_apply_all("Hello from C++!");
    if (result == NULL) {
        std::cerr << "Error: " << plugin_get_last_error() << std::endl;
        return 1;
    }
    
    std::cout << result << std::endl;
    return 0;
}
```

Build with: `g++ -o app app.cpp plugin.c plugins/*.c`

## Architecture

### Core Components

1. **plugin.h** - Plugin interface definition with error codes and documentation
2. **plugin.c** - Plugin management with dual staging buffers for safe chaining
3. **plugins/** - Directory containing plugin implementations

### How It Works

- Plugins implement a transform function: `const char* transform(const char* input)`
- Plugins are registered at runtime via `plugin_register()`
- When a message is processed, it's passed through all registered plugins in sequence
- Each plugin's output is copied to a staging buffer before calling the next plugin
- Uses ping-pong buffering to prevent aliasing issues with static buffers

### Buffer Ownership and Lifetime

**Plugins:**
- Plugins typically return pointers to their own static buffers
- The returned pointer must remain valid at least until the plugin is called again
- Plugins must not return pointers that the caller should free

**plugin_apply_all():**
- Returns a pointer to an internal static buffer
- The returned pointer remains valid until the next call to plugin_apply_all()
- If you need the result to persist, copy it to your own buffer:
  ```c
  const char* result = plugin_apply_all("input");
  char my_copy[256];
  if (result != NULL) {
      strncpy(my_copy, result, sizeof(my_copy) - 1);
      my_copy[sizeof(my_copy) - 1] = '\0';
  }
  ```

## Error Handling

### Error Codes

The plugin system defines these error codes:

**Registration Error Codes:**
```c
#define PLUGIN_SUCCESS          0   /* Operation succeeded */
#define PLUGIN_ERROR_FULL      -1   /* Plugin registry is full */
#define PLUGIN_ERROR_NULL      -2   /* NULL plugin or invalid plugin descriptor */
#define PLUGIN_ERROR_DUPLICATE -3   /* Plugin already registered */
```

**Execution Error Codes:**
```c
#define PLUGIN_ERROR_INPUT_NULL     -4   /* Input to plugin_apply_all was NULL */
#define PLUGIN_ERROR_PLUGIN_FAILED  -5   /* A plugin returned NULL during transformation */
#define PLUGIN_ERROR_OUTPUT_TOO_LARGE -6 /* Plugin output exceeded buffer size */
#define PLUGIN_ERROR_INVALID_PLUGIN -7   /* Plugin in registry has NULL transform function */
```

### Querying Last Error

After any plugin operation fails, you can query the specific error:

```c
const char* result = plugin_apply_all("Hello world!");
if (result == NULL) {
    int error = plugin_get_last_error();
    switch (error) {
        case PLUGIN_ERROR_INPUT_NULL:
            fprintf(stderr, "Error: Input was NULL\n");
            break;
        case PLUGIN_ERROR_PLUGIN_FAILED:
            fprintf(stderr, "Error: A plugin failed during transformation\n");
            break;
        case PLUGIN_ERROR_OUTPUT_TOO_LARGE:
            fprintf(stderr, "Error: Output exceeded buffer size\n");
            break;
        case PLUGIN_ERROR_INVALID_PLUGIN:
            fprintf(stderr, "Error: Invalid plugin in registry\n");
            break;
        default:
            fprintf(stderr, "Error: Unknown error %d\n", error);
            break;
    }
    return 1;
}
```

### When Functions Return NULL

**`plugin_apply_all()` returns NULL when:**
- Input is NULL (sets PLUGIN_ERROR_INPUT_NULL)
- Any plugin returns NULL (sets PLUGIN_ERROR_PLUGIN_FAILED)
- Result would exceed PLUGIN_BUFFER_SIZE - 1 bytes (sets PLUGIN_ERROR_OUTPUT_TOO_LARGE)
- A plugin in the registry has a NULL transform function (sets PLUGIN_ERROR_INVALID_PLUGIN)

**Always check the return value and query error if needed:**
```c
const char* result = plugin_apply_all("Hello world!");
if (result == NULL) {
    int error = plugin_get_last_error();
    fprintf(stderr, "Error: plugin transformation failed (code %d)\n", error);
    return 1;
}
```

## Configuration

You can override the default limits at compile time:

```bash
# Increase maximum plugins to 20
gcc -DPLUGIN_MAX_PLUGINS=20 -o app app.c plugin.c plugins/*.c

# Increase buffer size to 512 bytes
gcc -DPLUGIN_BUFFER_SIZE=512 -o app app.c plugin.c plugins/*.c

# Both
gcc -DPLUGIN_MAX_PLUGINS=20 -DPLUGIN_BUFFER_SIZE=512 -o app app.c plugin.c plugins/*.c
```

**Defaults:**
- `PLUGIN_MAX_PLUGINS`: 10
- `PLUGIN_BUFFER_SIZE`: 256 bytes

## Creating a Plugin

### Step 1: Create Your Plugin File

Create a new C file in the `plugins/` directory:

```c
/* my_plugin.c - Example custom plugin */
#include "../plugin.h"
#include <string.h>
#include <stdio.h>

static char my_buffer[PLUGIN_BUFFER_SIZE];

const char* my_transform(const char* input) {
    size_t len;
    
    /* ALWAYS validate input */
    if (input == NULL) {
        return NULL;
    }
    
    /* Your transformation logic here */
    len = strlen(input);
    
    /* ALWAYS check buffer size */
    if (len + 8 >= PLUGIN_BUFFER_SIZE) {
        return NULL;  /* Not enough space */
    }
    
    /* Safe transformation */
    snprintf(my_buffer, PLUGIN_BUFFER_SIZE, "*** %s ***", input);
    
    return my_buffer;
}

/* Plugin descriptor */
const plugin_t my_plugin = {
    .name = "my_plugin",
    .transform = my_transform
};
```

**Best Practices:**
1. ✅ Always validate input (check for NULL)
2. ✅ Always check buffer sizes before writing
3. ✅ Always null-terminate your output
4. ✅ Return NULL on error
5. ✅ Use static buffers (not heap allocation)
6. ✅ Ensure proper type casting (e.g., `(unsigned char)` for ctype functions)

### Step 2: Register Your Plugin

In your application code:

```c
#include <stdio.h>
#include "plugin.h"

extern const plugin_t my_plugin;

int main(void) {
    int result;
    const char* output;
    
    /* Initialize the plugin system */
    plugin_init();
    
    /* Register plugin with error checking */
    result = plugin_register(&my_plugin);
    if (result != PLUGIN_SUCCESS) {
        fprintf(stderr, "Error: failed to register plugin: %d\n", result);
        return 1;
    }
    
    /* Apply plugins with error checking */
    output = plugin_apply_all("Hello world!");
    if (output == NULL) {
        fprintf(stderr, "Error: plugin transformation failed\n");
        return 1;
    }
    
    printf("%s\n", output);
    
    return 0;
}
```

### Step 3: Build with Your Plugin

```bash
gcc -Wall -Wextra -o my_app my_app.c plugin.c plugins/my_plugin.c
```

## Example Plugins

### Uppercase Plugin

Converts the message to uppercase.

**File:** `plugins/uppercase.c`

**Usage:**
```c
extern const plugin_t uppercase_plugin;
plugin_register(&uppercase_plugin);
```

**Example:**
- Input: `"Hello world!"`
- Output: `"HELLO WORLD!"`

### Reverse Plugin

Reverses the message.

**File:** `plugins/reverse.c`

**Usage:**
```c
extern const plugin_t reverse_plugin;
plugin_register(&reverse_plugin);
```

**Example:**
- Input: `"Hello world!"`
- Output: `"!dlrow olleH"`

### Prefix Plugin

Adds a prefix to the message.

**File:** `plugins/prefix.c`

**Usage:**
```c
extern const plugin_t prefix_plugin;
plugin_register(&prefix_plugin);
```

**Example:**
- Input: `"Hello world!"`
- Output: `">>> Hello world!"`

## Plugin Chaining

Plugins can be chained together. The output of one plugin becomes the input of the next:

```c
plugin_init();
plugin_register(&prefix_plugin);    /* First: adds prefix */
plugin_register(&uppercase_plugin); /* Second: converts to uppercase */

const char* result = plugin_apply_all("Hello world!");
/* Result: ">>> HELLO WORLD!" */
```

## API Reference

### Functions

#### `void plugin_init(void)`

Initializes the plugin system. Must be called before using any other plugin functions.

- Resets the plugin registry (clears all registered plugins)
- Clears internal staging buffers
- Can be called multiple times to reset the system

**Thread Safety:** Not thread-safe

#### `int plugin_register(const plugin_t* plugin)`

Registers a plugin with the system.

**Parameters:**
- `plugin` - Pointer to plugin descriptor (must not be NULL)
  - `plugin->name` must not be NULL
  - `plugin->transform` must not be NULL

**Returns:**
- `PLUGIN_SUCCESS` (0) on success
- `PLUGIN_ERROR_FULL` (-1) if plugin registry is full
- `PLUGIN_ERROR_NULL` (-2) if plugin, name, or transform is NULL
- `PLUGIN_ERROR_DUPLICATE` (-3) if plugin is already registered

**Notes:**
- Duplicate detection is based on pointer equality (same plugin_t* address)
- Registration order determines execution order in `plugin_apply_all()`

**Thread Safety:** Not thread-safe

**Example:**
```c
int result = plugin_register(&my_plugin);
if (result != PLUGIN_SUCCESS) {
    switch (result) {
        case PLUGIN_ERROR_FULL:
            fprintf(stderr, "Error: plugin registry full\n");
            break;
        case PLUGIN_ERROR_NULL:
            fprintf(stderr, "Error: invalid plugin descriptor\n");
            break;
        case PLUGIN_ERROR_DUPLICATE:
            fprintf(stderr, "Error: plugin already registered\n");
            break;
    }
    return 1;
}
```

#### `const char* plugin_apply_all(const char* input)`

Applies all registered plugins to an input string in the order they were registered.

**Parameters:**
- `input` - The input string to transform (must not be NULL)

**Returns:**
- Pointer to the final transformed string, or NULL on error
- The returned pointer points to an internal static buffer
- The returned string remains valid until the next call to `plugin_apply_all()`
- Returns NULL if:
  - input is NULL
  - any plugin returns NULL (error in transformation)
  - result would exceed PLUGIN_BUFFER_SIZE - 1 bytes
  - any plugin in the registry has a NULL transform function

**Implementation:**
- Uses dual staging buffers (ping-pong buffering) to safely chain plugins
- Each plugin's output is copied to a staging buffer before calling the next plugin
- This ensures safe chaining even when plugins return pointers to static buffers

**Thread Safety:** Not thread-safe

**Example:**
```c
const char* result = plugin_apply_all("Hello world!");
if (result == NULL) {
    fprintf(stderr, "Error: transformation failed\n");
    return 1;
}

/* Use result immediately or copy it */
char my_copy[256];
strncpy(my_copy, result, sizeof(my_copy) - 1);
my_copy[sizeof(my_copy) - 1] = '\0';
```

#### `int plugin_count(void)`

Returns the number of currently registered plugins.

**Returns:**
- Number of registered plugins (0 to PLUGIN_MAX_PLUGINS)

**Thread Safety:** Not thread-safe

### Data Types

#### `plugin_t`

Plugin descriptor structure:

```c
typedef struct {
    const char* name;              /* Plugin name (must not be NULL) */
    plugin_transform_fn transform;  /* Transform function (must not be NULL) */
} plugin_t;
```

Both fields must be non-NULL for successful registration.

#### `plugin_transform_fn`

Plugin transform function signature:

```c
typedef const char* (*plugin_transform_fn)(const char* input);
```

**Contract:**
- Input: pointer to null-terminated string (may be NULL, plugins should check)
- Output: pointer to transformed null-terminated string, or NULL on error
- The returned pointer typically points to a plugin-internal static buffer
- The returned string must remain valid at least until the function is called again
- The returned string should not exceed PLUGIN_BUFFER_SIZE - 1 bytes

## Testing

The plugin system includes comprehensive tests:

```bash
# Run all plugin tests (unit tests + edge cases)
./test/run_plugin_tests.sh

# Run with Address Sanitizer and Undefined Behavior Sanitizer
gcc -fsanitize=address -fsanitize=undefined -g -I. -o test_asan \
    test/test_plugin.c plugin.c plugins/*.c
./test_asan
```

**Test Coverage:**
- 26 total tests (11 unit tests + 15 edge case tests)
- NULL pointer handling
- Buffer boundary conditions
- Error code validation
- Duplicate registration
- Registry overflow
- Plugin chaining safety
- Repeated calls
- Empty and long inputs

## Building the Demo

To see the plugin system in action:

```bash
# Build the demo
gcc -o plugin_demo plugin_demo.c plugin.c plugins/uppercase.c plugins/reverse.c plugins/prefix.c

# Run the demo
./plugin_demo
```

The demo shows various combinations of plugins and their effects.

## Best Practices

1. **Always Validate Input:** Check for NULL before processing
   ```c
   if (input == NULL) return NULL;
   ```

2. **Always Check Buffer Sizes:** Prevent buffer overflows
   ```c
   size_t len = strlen(input);
   if (len >= PLUGIN_BUFFER_SIZE) {
       len = PLUGIN_BUFFER_SIZE - 1;
   }
   ```

3. **Always Null-Terminate:** Ensure output is properly terminated
   ```c
   buffer[len] = '\0';
   ```

4. **Return NULL on Error:** Signal failures explicitly
   ```c
   if (error_condition) return NULL;
   ```

5. **Use Static Buffers:** Avoid heap allocation for simplicity
   ```c
   static char my_buffer[PLUGIN_BUFFER_SIZE];
   ```

6. **Check Return Values:** Always check plugin_register() and plugin_apply_all()
   ```c
   if (result != PLUGIN_SUCCESS) { /* handle error */ }
   if (output == NULL) { /* handle error */ }
   ```

7. **Order Matters:** Register plugins in execution order
   
8. **Initialize First:** Call plugin_init() before any operations

9. **Copy Results If Needed:** The returned pointer is only valid until the next call
   ```c
   const char* result = plugin_apply_all(input);
   char my_copy[256];
   if (result) strncpy(my_copy, result, 255);
   ```

## Plugin Lifecycle Management

### Resetting the Plugin System

You can reset the plugin system by calling `plugin_init()` again:

```c
/* Initial setup */
plugin_init();
plugin_register(&plugin1);
plugin_register(&plugin2);

/* Use plugins... */
const char* result = plugin_apply_all("test");

/* Reset and re-register different plugins */
plugin_init();  /* Clears all registered plugins */
plugin_register(&plugin3);
plugin_register(&plugin4);

/* Use new plugin configuration... */
result = plugin_apply_all("test");
```

**Note:** There is no `plugin_unregister()` function. To remove plugins, call `plugin_init()` to reset the entire system, then re-register the desired plugins.

### Duplicate Registration

**Pointer-based detection:** The same `plugin_t` pointer cannot be registered twice:

```c
plugin_t my_plugin = {.name = "foo", .transform = my_fn};

plugin_register(&my_plugin);  // OK
plugin_register(&my_plugin);  // Returns PLUGIN_ERROR_DUPLICATE
```

**Name-based allowance:** Different pointers with the same name are allowed:

```c
plugin_t plugin1 = {.name = "foo", .transform = fn1};
plugin_t plugin2 = {.name = "foo", .transform = fn2};

plugin_register(&plugin1);  // OK
plugin_register(&plugin2);  // OK (different pointer, even though same name)
```

This allows multiple instances of similarly-named plugins as long as they are different objects.

## Limitations

- Maximum 10 plugins can be registered simultaneously
- Output buffer size is limited to 256 bytes
- Not thread-safe (uses static buffers)
- Plugins must be compiled together with the main application

## Integration with hello.c

The hello.c program can optionally be compiled with plugin support:

```bash
# Build hello.c with plugin support
gcc -DUSE_PLUGINS -o hello hello.c plugin.c plugins/uppercase.c

# Or without plugins (default behavior)
gcc -o hello hello.c
```

This allows hello.c to maintain its simple core functionality while enabling optional extensions through plugins.
