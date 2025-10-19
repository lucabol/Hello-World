# Plugin System Quick Start Guide

## What is this?

The Hello World program now supports a plugin architecture that allows you to extend its functionality without modifying the core `hello.c` file.

## Quick Examples

### 1. Run Original Hello (No Plugins)

```bash
make
./hello
# Output: Hello world!
```

### 2. Run with Uppercase Plugin

```bash
make example
./hello_example
# Output: HELLO WORLD!
```

### 3. See All Plugin Examples

```bash
make demo
./demo_plugins
# Shows comprehensive examples of all available plugins
```

## Available Plugins

| Plugin | Description | Example Output |
|--------|-------------|----------------|
| `uppercase.c` | Converts to uppercase | `HELLO WORLD!` |
| `reverse.c` | Reverses the string | `!dlrow olleH` |
| `exclaim.c` | Adds excitement | `Hello world!!!!` |

## Creating Your Own Plugin

### Step 1: Create Plugin File

Create `plugins/my_plugin.c`:

```c
#include <stdlib.h>
#include <string.h>
#include "../plugin.h"

char* my_plugin(const char* input) {
    if (input == NULL) return NULL;
    
    size_t len = strlen(input);
    char* result = malloc(len + 1);
    if (result == NULL) return (char*)input;
    
    // Your transformation here
    strcpy(result, input);
    // ... modify result ...
    
    return result;
}
```

### Step 2: Create Main Program

Create `my_main.c`:

```c
#include <stdio.h>
#include <stdlib.h>
#include "plugin.h"

extern char* my_plugin(const char* input);

int main() {
    const char* message = "Hello world!";
    
    register_plugin(my_plugin);
    
    char* result = apply_plugins(message);
    printf("%s", result);
    
    if (result != message) free(result);
    return 0;
}
```

### Step 3: Build and Run

```bash
gcc -o my_hello my_main.c plugin.c plugins/my_plugin.c
./my_hello
```

## Plugin Chaining

You can combine multiple plugins:

```c
register_plugin(uppercase_plugin);  // Applied first
register_plugin(exclaim_plugin);    // Applied second
register_plugin(reverse_plugin);    // Applied third

// Input:  Hello world!
// After uppercase: HELLO WORLD!
// After exclaim: HELLO WORLD!!!!
// After reverse: !!!!DLROW OLLEH
```

## Need More Information?

- **Full Documentation**: See [PLUGINS.md](PLUGINS.md)
- **Plugin Examples**: See [plugins/README.md](plugins/README.md)
- **API Reference**: See [plugin.h](plugin.h)

## Build Commands Reference

| Command | Description |
|---------|-------------|
| `make` | Build original hello (no plugins) |
| `make example` | Build with uppercase plugin |
| `make demo` | Build comprehensive demo |
| `make clean` | Remove all build artifacts |
| `make test` | Run validation tests |
| `make strict` | Build with strict compiler flags |

## Troubleshooting

**Q: Does the plugin system change the original hello.c behavior?**  
A: No! When built with just `make`, hello.c behaves exactly as before. Plugins are only enabled when built with the `USE_PLUGINS` flag.

**Q: How many plugins can I register?**  
A: Up to 10 plugins (configurable in `plugin.h` via `MAX_PLUGINS`).

**Q: What if my plugin doesn't need to modify the string?**  
A: Just return the input pointer directly - no need to allocate memory.

**Q: Memory leaks?**  
A: The plugin system handles intermediate memory cleanup when chaining. Just make sure to free the final result if it's different from the input.
