# Plugin System for hello.c

This document explains how to use the plugin system to extend hello.c functionality without modifying the core file.

## Overview

The plugin system allows external developers to create plugins that transform the greeting message. Plugins are simple C functions that take an input string and return a transformed string.

## Architecture

The plugin system consists of:

1. **Plugin Interface** (`plugin.h`) - Defines the plugin function signature and metadata structure
2. **Plugin Loader** (`plugin_loader.c`) - Applies plugins in sequence to transform the message
3. **Example Plugins** (`example_plugins.c/h`) - Demonstrates plugin implementation
4. **Core Program** (`hello.c`) - The main program with inline plugin support

## Quick Start

### Using the Default Program (No Plugins)

```bash
# Build and run (outputs "Hello world!")
make
./hello
```

### Using Example Plugins

```bash
# Build with uppercase plugin
make hello-with-uppercase
./hello-with-uppercase
# Output: HELLO WORLD!
```

## Creating Your Own Plugin

### Step 1: Write the Plugin Function

Create a new file `my_plugin.c`:

```c
#include <string.h>

/* Buffer for plugin output - must be static */
static char my_buffer[256];

/* My custom plugin function */
const char* my_custom_plugin(const char* input) {
    /* Transform the input and store in buffer */
    int len = strlen(input);
    
    /* Example: add prefix */
    snprintf(my_buffer, sizeof(my_buffer), ">>> %s <<<", input);
    
    return my_buffer;
}
```

### Step 2: Create a Plugin Header

Create `my_plugin.h`:

```c
#ifndef MY_PLUGIN_H
#define MY_PLUGIN_H

const char* my_custom_plugin(const char* input);

#endif
```

### Step 3: Create Your Program File

Create `hello_custom.c`:

```c
#include <stdio.h>
#include "plugin.h"
#include "my_plugin.h"

static const char* greeting = "Hello world!";

static plugin_t plugins[] = {
    {"custom", "My custom transformation", my_custom_plugin}
};
static int plugin_count = sizeof(plugins) / sizeof(plugins[0]);

int main() {
    const char* final_message = apply_plugins(greeting, plugins, plugin_count);
    printf("%s\n", final_message);
    return 0;
}
```

### Step 4: Compile Your Program

```bash
gcc -Wall -Wextra -std=c99 -o hello-custom hello_custom.c plugin_loader.c my_plugin.c
./hello-custom
# Output: >>> Hello world! <<<
```

## Plugin Examples

### Uppercase Plugin

Converts the message to uppercase:

```bash
make hello-with-uppercase
./hello-with-uppercase
# Output: HELLO WORLD!
```

### Multiple Plugins (Chaining)

You can chain multiple plugins together:

```c
static plugin_t plugins[] = {
    {"reverse", "Reverse the message", reverse_plugin},
    {"uppercase", "Convert to uppercase", uppercase_plugin}
};
static int plugin_count = 2;
```

Plugins are applied in order, so the output of one plugin becomes the input to the next.

## Plugin Function Contract

### Requirements

1. **Function Signature**: `const char* plugin_name(const char* input)`
2. **Return Value**: Must return a pointer to static or heap memory (not stack memory)
3. **Input**: Must NOT modify the input string
4. **Thread Safety**: Plugins use static buffers, so they are not thread-safe

### Example Pattern

```c
static char output_buffer[256];  /* Static buffer for output */

const char* my_plugin(const char* input) {
    /* 1. Validate input */
    if (input == NULL) return "";
    
    /* 2. Transform input and store in static buffer */
    int len = strlen(input);
    if (len >= 256) len = 255;
    
    /* Your transformation logic here */
    strcpy(output_buffer, input);
    
    /* 3. Return pointer to static buffer */
    return output_buffer;
}
```

## Available Example Plugins

### 1. Uppercase Plugin

**File**: `example_plugins.c`  
**Function**: `uppercase_plugin()`  
**Description**: Converts all characters to uppercase

```c
const char* uppercase_plugin(const char* input);
```

### 2. Reverse Plugin

**File**: `example_plugins.c`  
**Function**: `reverse_plugin()`  
**Description**: Reverses the message string

```c
const char* reverse_plugin(const char* input);
```

### 3. Exclaim Plugin

**File**: `example_plugins.c`  
**Function**: `exclaim_plugin()`  
**Description**: Adds extra exclamation marks

```c
const char* exclaim_plugin(const char* input);
```

## Best Practices

1. **Keep Plugins Simple**: Each plugin should do one thing well
2. **Use Static Buffers**: Return pointers to static memory for simplicity
3. **Check Buffer Sizes**: Always validate buffer bounds to prevent overflows
4. **Document Your Plugin**: Include clear comments about what your plugin does
5. **Test Independently**: Test your plugin with various inputs before integrating

## Makefile Targets

```bash
make               # Build default hello (no plugins)
make test          # Run validation tests
make hello-with-uppercase  # Build with uppercase plugin example
make clean         # Remove build artifacts
make debug         # Build with debug symbols
make strict        # Build with strict compiler flags
```

## Integration with Existing Code

The core `hello.c` file includes the plugin system inline, so it can be compiled standalone:

```bash
gcc -o hello hello.c
./hello
# Output: Hello world!
```

This ensures backward compatibility - the program works exactly as before when no plugins are active.

## Limitations

1. **Static Buffers**: Plugins use static buffers, limiting output size to 256 characters
2. **Not Thread-Safe**: Static buffers make plugins unsuitable for multi-threaded use
3. **Compile-Time Only**: Plugins must be compiled with the program (no dynamic loading)
4. **Sequential Processing**: Plugins are applied in order, one after another

## Future Enhancements

Possible improvements to the plugin system:

- Dynamic plugin loading (using `dlopen()`)
- Plugin configuration files
- Plugin dependency management
- Thread-safe plugin execution
- Plugin error handling and validation
- Plugin hooks for different stages (pre-print, post-print, etc.)

## Troubleshooting

### "undefined reference to apply_plugins"

**Solution**: Make sure to compile with `plugin_loader.c`:

```bash
gcc -o hello hello_custom.c plugin_loader.c my_plugin.c
```

### Buffer Overflow

**Solution**: Check your buffer sizes and use `strncpy()` or `snprintf()`:

```c
snprintf(output_buffer, sizeof(output_buffer), "%s", input);
```

### Output Corrupted

**Solution**: Ensure your plugin returns a pointer to static or heap memory, not stack memory:

```c
/* WRONG - returns stack memory */
char buffer[256];
return buffer;

/* CORRECT - returns static memory */
static char buffer[256];
return buffer;
```

## Contributing

To contribute new plugins:

1. Create your plugin following the patterns in `example_plugins.c`
2. Test your plugin thoroughly
3. Document your plugin's behavior
4. Submit a pull request with your plugin file and documentation

## License

This plugin system is part of the Hello-World project and follows the same license.
