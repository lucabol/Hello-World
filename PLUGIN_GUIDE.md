# Plugin System Guide

The Hello World program now supports a plugin architecture that allows external developers to extend functionality without modifying the core `hello.c` file.

## Overview

The plugin system provides a simple interface for creating transformations that modify the greeting message before it is displayed. Plugins are applied in the order they are included.

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

## Creating a Plugin

### Option 1: Header-Only Plugin (Recommended)

Create a header file in the `plugins/` directory:

```c
/* plugins/my_plugin.h */
#ifndef MY_PLUGIN_H
#define MY_PLUGIN_H

#include "../plugin.h"

static char my_buffer[256];

static const char* my_transform(const char* message) {
    /* Your transformation logic here */
    return my_buffer;
}

DEFINE_PLUGIN(my_transform)

#endif
```

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

## Building with Plugins

### Standard Build (No Plugins)

```bash
gcc -o hello hello.c plugin.c
./hello
# Output: Hello world!
```

### Build with Plugins

```bash
gcc -o hello_with_plugins your_main.c hello_lib.c plugin.c
./hello_with_plugins
# Output: (transformed by your plugins)
```

## Example Plugins

### Uppercase Plugin

Location: `plugins/uppercase_plugin.h`

Transforms the greeting to uppercase:

```bash
gcc -o demo demo.c hello_lib.c plugin.c
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
// Result: HELLO WORLD!!! (different result order)
```

## Plugin Development Guidelines

1. **Thread Safety**: Plugins should use static buffers or ensure thread safety
2. **Buffer Size**: Limit transformed strings to 256 characters
3. **Null Termination**: Always null-terminate your output strings
4. **No Memory Allocation**: Avoid dynamic memory allocation in plugins
5. **Immutability**: Don't modify the input string
6. **Error Handling**: Return the input unchanged if transformation fails

## Limitations

- Maximum 10 plugins can be registered (configurable in plugin.h)
- Plugins must return static or constant strings
- Plugin execution order matches include order
- No dynamic loading at runtime (compile-time only)

## API Reference

### Core Functions

```c
/* Register a plugin function manually */
void register_plugin(plugin_func_t func);

/* Apply all registered plugins to a message */
const char* apply_plugins(const char* message);

/* Get the base greeting message */
const char* get_greeting(void);
```

### Macros

```c
/* Define a plugin in a header file (auto-registration) */
DEFINE_PLUGIN(function_name)

/* Declare a plugin that will be initialized manually */
DECLARE_PLUGIN(function_name)

/* Initialize a manually declared plugin */
INIT_PLUGIN(function_name)
```

## Testing

Run the validation script to ensure your changes don't break core functionality:

```bash
./test/validate.sh
```

## Contributing

When contributing plugins:

1. Place plugin headers in the `plugins/` directory
2. Use descriptive names (e.g., `reverse_plugin.h`)
3. Document what your plugin does in the header comment
4. Provide usage examples
5. Test with the core hello.c program

## Examples Directory

See the `plugins/` directory for complete working examples:

- `uppercase_plugin.h` - Converts to uppercase
- `exclamation_plugin.h` - Adds exclamation marks

## Support

For issues or questions about the plugin system, please open an issue on the repository.
