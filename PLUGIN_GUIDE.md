# Plugin System Guide

The Hello World program now supports a plugin architecture that allows external developers to extend functionality without modifying the core `hello.c` file.

## Overview

The plugin system provides a simple interface for creating transformations that modify the greeting message before it is displayed. Plugins are applied in the order they are included.

## Compiler Compatibility

### Supported Compilers

- **GCC / Clang**: Full automatic plugin registration support using constructor attributes
- **MSVC**: Requires manual registration (see below)
- **Other compilers**: May require manual registration

### Automatic vs Manual Registration

**GCC/Clang (Automatic):**
```c
#include "plugin.h"
#include "plugins/my_plugin.h"  /* Auto-registers via constructor */

int main() {
    /* Plugins already registered */
    const char* result = apply_plugins(get_greeting());
}
```

**MSVC (Manual):**
```c
#include "plugin.h"

/* Declare plugin functions */
const char* my_transform(const char*);

int main() {
    /* Manually register plugins */
    INIT_PLUGIN(my_transform);
    
    /* Now use plugins */
    const char* result = apply_plugins(get_greeting());
}
```

## Architecture

### Core Components

- **plugin.h**: Plugin interface definition
- **plugin.c**: Plugin system implementation  
- **hello.h**: Core API exposing `get_greeting()`
- **hello.c**: Main program that uses plugins
- **hello_lib.c**: Library version of core functionality (without main)

### Plugin Function Signature

```c
const char* plugin_function(const char* input)
```

Each plugin receives a string and returns a transformed string.

### Transform Function Contract

**Memory and Lifetime:**
- Plugin functions MUST return a pointer to memory that remains valid until the next plugin call
- Return value must point to static storage, thread-local storage, or long-lived heap memory
- Caller does NOT take ownership and must NOT free the returned pointer
- Returned string must be properly null-terminated

**Thread Safety:**
- Plugin registration is NOT thread-safe
- Plugin application is NOT thread-safe unless explicitly designed for it
- Register all plugins during single-threaded startup before concurrent access
- If thread-safety is needed, use thread-local buffers or synchronization primitives

**Buffer Sizes:**
- Recommended maximum buffer size: 256 bytes
- Plugins must handle truncation gracefully
- Input strings may be arbitrarily long; plugins should validate and truncate if needed

**Example Safe Implementation:**
```c
static char PLUGIN_BUFFER(myplugin)[256];

static const char* my_transform(const char* message) {
    int i = 0;
    /* Safely copy with bounds checking */
    while (message[i] != '\0' && i < 255) {
        PLUGIN_BUFFER(myplugin)[i] = transform_char(message[i]);
        i++;
    }
    PLUGIN_BUFFER(myplugin)[i] = '\0';  /* Always null-terminate */
    return PLUGIN_BUFFER(myplugin);
}
```

## Creating a Plugin

### Option 1: Header-Only Plugin (Recommended)

Create a header file in the `plugins/` directory:

```c
/* plugins/my_plugin.h */
#ifndef MY_PLUGIN_H
#define MY_PLUGIN_H

#include "plugin.h"

/* Use PLUGIN_BUFFER macro to create unique buffer name */
static char PLUGIN_BUFFER(myplugin)[256];

static const char* my_transform(const char* message) {
    /* Your transformation logic here */
    /* Always bounds-check and null-terminate */
    int i = 0;
    while (message[i] != '\0' && i < 255) {
        PLUGIN_BUFFER(myplugin)[i] = message[i];
        i++;
    }
    PLUGIN_BUFFER(myplugin)[i] = '\0';
    return PLUGIN_BUFFER(myplugin);
}

DEFINE_PLUGIN(my_transform)

#endif
```

**Important Notes:**
- Use `PLUGIN_BUFFER(uniquename)` macro to avoid buffer name collisions
- Each plugin should have a unique buffer name
- Include plugin headers in ONLY ONE translation unit to avoid duplicate registration
- Always null-terminate your output strings
- Perform bounds checking to prevent buffer overflows

### Option 2: Separate Source File

For more complex plugins, you can use separate .c files:

1. Create your plugin function in a .c file
2. Declare it with `DECLARE_PLUGIN(func_name)` in a .h file
3. Call `INIT_PLUGIN(func_name)` in your main() before using plugins

## Using Plugins

Include plugin headers in your program before the plugin system is used:

```c
#include <stdio.h>
#include "hello.h"
#include "plugin.h"

/* Include plugins in the order you want them applied */
#include "plugins/plugin1.h"
#include "plugins/plugin2.h"

int main() {
    const char* greeting = get_greeting();
    const char* transformed = apply_plugins(greeting);
    printf("%s", transformed);
    return 0;
}
```

**Important:** Include each plugin header in only ONE .c file to prevent duplicate registration.

## Building with Plugins

### Standard Build (No Plugins)

```bash
gcc -o hello hello.c plugin.c
./hello
# Output: Hello world!
```

### Build with Plugins

```bash
# Use -I. to allow "plugin.h" includes
gcc -I. -o hello_with_plugins your_main.c hello_lib.c plugin.c
./hello_with_plugins
# Output: (transformed by your plugins)
```

### Custom MAX_PLUGINS

The default maximum is 10 plugins. To increase:

```bash
gcc -DMAX_PLUGINS=20 -o hello hello.c plugin.c
```

Or in your code before including plugin.h:
```c
#define MAX_PLUGINS 20
#include "plugin.h"
```

## Example Plugins

### Uppercase Plugin

Location: `plugins/uppercase_plugin.h`

Transforms the greeting to uppercase:

```bash
gcc -I. -o demo demo.c hello_lib.c plugin.c
# Include plugins/uppercase_plugin.h in demo.c
# Output: HELLO WORLD!
```

### Exclamation Plugin

Location: `plugins/exclamation_plugin.h`

Adds extra exclamation marks:

```bash
# Include plugins/exclamation_plugin.h in demo.c  
# Output: Hello world!!!
```

### Combining Plugins

Plugins are applied in the order they are included:

```c
#include "plugins/exclamation_plugin.h"
#include "plugins/uppercase_plugin.h"
// Result: HELLO WORLD!!!

#include "plugins/uppercase_plugin.h"
#include "plugins/exclamation_plugin.h"
// Result: HELLO WORLD!!! (same result - uppercase doesn't affect "!")
```

## Plugin Development Guidelines

1. **Buffer Naming**: Use `PLUGIN_BUFFER(uniquename)` to avoid collisions
2. **Bounds Checking**: Always validate input length and prevent overflows
3. **Null Termination**: Always null-terminate output strings
4. **Thread Safety**: Use thread-local storage if concurrent access is needed
5. **Immutability**: Don't modify the input string
6. **Error Handling**: Return input unchanged if transformation fails
7. **Single Include**: Include plugin headers in only ONE translation unit
8. **Memory Management**: Use static buffers, avoid dynamic allocation in hot paths

## Error Handling

### Registration Errors

`register_plugin()` returns:
- `0`: Success
- `-1`: Registry full (MAX_PLUGINS exceeded) or NULL function pointer
- `-2`: Already registered (duplicate)

```c
int result = register_plugin(my_transform);
if (result == -1) {
    fprintf(stderr, "Failed to register plugin\n");
} else if (result == -2) {
    /* Already registered - this is OK, plugin will work */
}
```

### Registry Overflow

If you exceed MAX_PLUGINS:
- Warning printed to stderr
- Plugin not registered
- Remaining plugins continue to work
- Consider increasing MAX_PLUGINS via -DMAX_PLUGINS=N

## Limitations

- Maximum 10 plugins by default (configurable via MAX_PLUGINS)
- Plugins must return static or long-lived strings
- Plugin execution order matches include order
- No dynamic loading at runtime (compile-time only)
- Not thread-safe by default
- Each plugin header should be included in only one .c file

## API Reference

### Core Functions

```c
/* Register a plugin function manually
 * Returns: 0 on success, -1 if full/NULL, -2 if duplicate
 */
int register_plugin(plugin_func_t func);

/* Apply all registered plugins to a message */
const char* apply_plugins(const char* message);

/* Get the base greeting message */
const char* get_greeting(void);
```

### Macros

```c
/* Define a plugin in a header file (auto-registration on GCC/Clang) */
DEFINE_PLUGIN(function_name)

/* Declare a plugin that will be initialized manually */
DECLARE_PLUGIN(function_name)

/* Initialize a manually declared plugin */
INIT_PLUGIN(function_name)

/* Create a unique buffer name for your plugin */
PLUGIN_BUFFER(unique_name)
```

## Testing

Run the validation script to ensure your changes don't break core functionality:

```bash
./test/validate.sh
```

Run plugin system tests:

```bash
gcc -o test_plugin_system test/test_plugin_system.c plugin.c hello_lib.c
./test_plugin_system
```

## Contributing

When contributing plugins:

1. Place plugin headers in the `plugins/` directory
2. Use descriptive names (e.g., `reverse_plugin.h`)
3. Use `PLUGIN_BUFFER(pluginname)` for unique buffer names
4. Document what your plugin does in the header comment
5. Provide usage examples
6. Test with the core hello.c program
7. Ensure proper bounds checking and null termination
8. Test for long inputs (>256 characters)

## Examples Directory

See the `plugins/` directory for complete working examples:

- `uppercase_plugin.h` - Converts to uppercase
- `exclamation_plugin.h` - Adds exclamation marks

## Troubleshooting

### Plugin not registered

- **GCC/Clang**: Check that you're including the plugin header
- **MSVC**: Ensure you're calling `INIT_PLUGIN(function_name)` manually
- **Check return value**: `register_plugin()` returns error codes

### Buffer overflows or truncation

- Increase buffer size in your plugin
- Add proper bounds checking
- Test with long input strings

### Duplicate registration

- Include plugin headers in only ONE .c file
- If you get -2 return code, the plugin is already registered (this is OK)

### Crashes or corruption

- Ensure null termination of strings
- Check for buffer overflows
- Verify you're not returning pointers to local (stack) variables
- Use static buffers with `PLUGIN_BUFFER` macro

## Support

For issues or questions about the plugin system, please open an issue on the repository.
