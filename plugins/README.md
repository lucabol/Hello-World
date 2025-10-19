# Hello World Plugins

This directory contains example plugins for the Hello World program.

## Available Plugins

### uppercase.c
Converts the message to uppercase letters.

**Usage:**
```c
register_plugin(uppercase_plugin);
```

**Example:**
- Input: `Hello world!`
- Output: `HELLO WORLD!`

### reverse.c
Reverses the message string.

**Usage:**
```c
register_plugin(reverse_plugin);
```

**Example:**
- Input: `Hello world!`
- Output: `!dlrow olleH`

### exclaim.c
Adds excitement to the message.

**Usage:**
```c
register_plugin(exclaim_plugin);
```

**Example:**
- Input: `Hello world!`
- Output: `Hello world!!!!`

## Creating Your Own Plugin

See [PLUGINS.md](../PLUGINS.md) in the root directory for detailed documentation on creating custom plugins.

### Quick Template

```c
/* plugins/my_plugin.c */
#include <stdlib.h>
#include <string.h>
#include "../plugin.h"

char* my_plugin(const char* input) {
    if (input == NULL) {
        return NULL;
    }
    
    size_t len = strlen(input);
    char* result = (char*)malloc(len + 1);
    
    if (result == NULL) {
        return (char*)input;
    }
    
    /* Your transformation logic here */
    strcpy(result, input);
    
    return result;
}
```

## Building with Plugins

```bash
# Build example with uppercase plugin
make example
./hello_example

# Build comprehensive demo with all plugins
make demo
./demo_plugins

# Custom build
gcc -o my_hello main.c plugin.c plugins/my_plugin.c
```
