# Plugin System Documentation

## Overview

The Hello World program now supports a plugin architecture that allows external developers to extend functionality without modifying the core `hello.c` file directly.

## Architecture

The plugin system uses a simple callback-based architecture:

1. **Plugin Interface** (`plugin.h`): Defines the plugin function signature and registration API
2. **Plugin Manager** (`plugin.c`): Manages plugin registration and execution
3. **Core Program** (`hello.c`): Optionally integrates plugins using `USE_PLUGINS` flag

## Plugin API

### Plugin Function Signature

```c
typedef char* (*plugin_func)(const char* input);
```

A plugin function:
- Takes a `const char*` input string
- Returns a `char*` transformed string
- Must allocate new memory (using `malloc`) if modifying the string
- Can return the input pointer directly if no transformation is needed

### Core Functions

#### `int register_plugin(plugin_func func)`
Registers a plugin function to be called during message processing.

**Parameters:**
- `func`: Pointer to a plugin function

**Returns:**
- `0` on success
- `-1` if plugin array is full or func is NULL

**Example:**
```c
register_plugin(uppercase_plugin);
```

#### `char* apply_plugins(const char* input)`
Applies all registered plugins in order to the input string.

**Parameters:**
- `input`: The original message string

**Returns:**
- Transformed string (caller must free if different from input)

**Example:**
```c
char* result = apply_plugins("Hello world!");
printf("%s", result);
if (result != "Hello world!") {
    free(result);
}
```

#### `void clear_plugins(void)`
Clears all registered plugins.

#### `int get_plugin_count(void)`
Returns the number of registered plugins.

## Creating a Plugin

### Step 1: Create Plugin File

Create a new C file in the `plugins/` directory:

```c
/* plugins/my_plugin.c */
#include <stdlib.h>
#include <string.h>
#include "../plugin.h"

char* my_plugin(const char* input) {
    if (input == NULL) {
        return NULL;
    }
    
    /* Allocate new string for transformation */
    size_t len = strlen(input);
    char* result = (char*)malloc(len + 1);
    
    if (result == NULL) {
        return (char*)input; /* Return original on allocation failure */
    }
    
    /* Apply your transformation here */
    strcpy(result, input);
    /* ... modify result ... */
    
    return result;
}
```

### Step 2: Register and Use Plugin

Create a main program that uses your plugin:

```c
#include <stdio.h>
#include <stdlib.h>
#include "plugin.h"

extern char* my_plugin(const char* input);

int main() {
    const char* message = "Hello world!";
    
    /* Register your plugin */
    register_plugin(my_plugin);
    
    /* Apply plugins */
    char* transformed = apply_plugins(message);
    printf("%s", transformed);
    
    /* Cleanup */
    if (transformed != message) {
        free(transformed);
    }
    
    return 0;
}
```

### Step 3: Build

Compile with your plugin:

```bash
gcc -o hello_custom hello_custom.c plugin.c plugins/my_plugin.c
```

## Example Plugins

### Uppercase Plugin (`plugins/uppercase.c`)

Converts the message to uppercase:

```c
char* uppercase_plugin(const char* input);
```

**Example Output:**
```
Input:  Hello world!
Output: HELLO WORLD!
```

### Reverse Plugin (`plugins/reverse.c`)

Reverses the message:

```c
char* reverse_plugin(const char* input);
```

**Example Output:**
```
Input:  Hello world!
Output: !dlrow olleH
```

### Exclaim Plugin (`plugins/exclaim.c`)

Adds excitement to the message:

```c
char* exclaim_plugin(const char* input);
```

**Example Output:**
```
Input:  Hello world!
Output: Hello world!!!!
```

## Building Options

### Original Hello (No Plugins)

```bash
make
./hello
# Output: Hello world!
```

### Hello with Plugins

```bash
make example
./hello_example
# Output: HELLO WORLD! (with uppercase plugin)
```

### Plugin Demo (All Examples)

```bash
make demo
./demo_plugins
# Shows comprehensive examples of single and chained plugins
```

### Custom Build with Specific Plugins

```bash
gcc -o hello_custom main.c plugin.c plugins/uppercase.c plugins/reverse.c
```

## Plugin Chaining

Multiple plugins can be registered and will be applied in order:

```c
register_plugin(uppercase_plugin);
register_plugin(reverse_plugin);

char* result = apply_plugins("Hello world!");
// Result: "!DLROW OLLEH" (uppercase first, then reversed)
```

## Memory Management

**Important:** Plugins that modify the input string must:
1. Allocate new memory using `malloc()`
2. Return the new pointer
3. The caller is responsible for freeing the result

**Plugin does not modify:**
```c
char* passthrough_plugin(const char* input) {
    return (char*)input; // No malloc needed
}
```

**Plugin modifies:**
```c
char* transform_plugin(const char* input) {
    char* result = malloc(strlen(input) + 1);
    strcpy(result, input);
    // ... modify result ...
    return result; // Caller must free
}
```

## Best Practices

1. **Always check for NULL input** in your plugin functions
2. **Handle allocation failures** gracefully by returning the input pointer
3. **Free intermediate results** if chaining plugins
4. **Keep plugins simple** and focused on a single transformation
5. **Test plugins independently** before integrating

## Limitations

- Maximum of 10 plugins can be registered (configurable via `MAX_PLUGINS`)
- Plugins are applied in registration order
- No plugin configuration or parameters beyond the input string
- Memory must be manually managed by callers

## Future Enhancements

Possible extensions to the plugin system:
- Plugin configuration/parameters
- Plugin metadata (name, version, description)
- Dynamic plugin loading from shared libraries (.so/.dll)
- Plugin dependencies and ordering
- Error handling and plugin validation
