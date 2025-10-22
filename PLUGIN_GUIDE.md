# Plugin System for hello.c

## Overview

The plugin system allows external developers to extend the functionality of hello.c without modifying the core file directly. Plugins can transform the output message before it's printed.

## Architecture

### Core Components

1. **plugin.h** - Plugin interface definition
2. **plugin.c** - Plugin management implementation
3. **plugins/** - Directory containing plugin implementations

### How It Works

- Plugins implement a simple transform function: `const char* transform(const char* input)`
- Plugins are registered at compile time via the plugin registry
- When a message is processed, it's passed through all registered plugins in sequence
- Each plugin's output becomes the next plugin's input (chaining)

## Creating a Plugin

### Step 1: Create Your Plugin File

Create a new C file in the `plugins/` directory:

```c
/* my_plugin.c - Example custom plugin */
#include "../plugin.h"
#include <string.h>

static char my_buffer[PLUGIN_BUFFER_SIZE];

const char* my_transform(const char* input) {
    /* Your transformation logic here */
    /* For example, add stars around the message */
    snprintf(my_buffer, PLUGIN_BUFFER_SIZE, "*** %s ***", input);
    return my_buffer;
}

/* Plugin descriptor */
const plugin_t my_plugin = {
    .name = "my_plugin",
    .transform = my_transform
};
```

### Step 2: Register Your Plugin

In your application code:

```c
#include "plugin.h"

extern const plugin_t my_plugin;

int main(void) {
    plugin_init();
    plugin_register(&my_plugin);
    
    const char* result = plugin_apply_all("Hello world!");
    printf("%s\n", result);
    
    return 0;
}
```

### Step 3: Build with Your Plugin

```bash
gcc -o my_app my_app.c plugin.c plugins/my_plugin.c
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

#### `int plugin_register(const plugin_t* plugin)`

Registers a plugin with the system.

**Parameters:**
- `plugin` - Pointer to plugin descriptor

**Returns:**
- `0` on success
- `-1` if plugin registry is full (max 10 plugins)

#### `const char* plugin_apply_all(const char* input)`

Applies all registered plugins to an input string in the order they were registered.

**Parameters:**
- `input` - The input string to transform

**Returns:**
- Pointer to the final transformed string

#### `int plugin_count(void)`

Returns the number of currently registered plugins.

### Data Types

#### `plugin_t`

Plugin descriptor structure:

```c
typedef struct {
    const char* name;              /* Plugin name */
    plugin_transform_fn transform;  /* Transform function */
} plugin_t;
```

#### `plugin_transform_fn`

Plugin transform function signature:

```c
typedef const char* (*plugin_transform_fn)(const char* input);
```

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

1. **Use Static Buffers:** Each plugin should use its own static buffer for output
2. **Buffer Size:** Never exceed `PLUGIN_BUFFER_SIZE` (256 bytes)
3. **Return Values:** Always return a valid pointer to a null-terminated string
4. **Order Matters:** Register plugins in the order you want them applied
5. **Initialize First:** Always call `plugin_init()` before registering plugins
6. **Error Handling:** Check return value of `plugin_register()` for errors

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
