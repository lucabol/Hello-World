# Plugins Directory

This directory contains example plugins for the Hello World plugin system.

## Available Plugins

### uppercase_plugin.h
Transforms the greeting message to uppercase.

**Example:**
```c
#include "plugins/uppercase_plugin.h"
// Transforms "Hello world!" to "HELLO WORLD!"
```

**Note:** Uses `PLUGIN_BUFFER(uppercase)` to avoid name collisions.

### exclamation_plugin.h
Adds extra exclamation marks to the greeting.

**Example:**
```c
#include "plugins/exclamation_plugin.h"
// Transforms "Hello world!" to "Hello world!!!"
```

**Note:** Uses `PLUGIN_BUFFER(exclamation)` for unique buffer naming.

## Creating Your Own Plugin

1. Create a new `.h` file in this directory
2. Follow the pattern in the example plugins
3. **Use `PLUGIN_BUFFER(uniquename)` macro** for buffer names
4. **Always bounds-check** and null-terminate strings
5. Include your plugin header in your main program (in ONLY ONE .c file)
6. Build and test

See the [Plugin Guide](../PLUGIN_GUIDE.md) for detailed instructions.

## Best Practices

✅ **Unique Buffer Names**: Use `PLUGIN_BUFFER(pluginname)` macro  
✅ **Bounds Checking**: Prevent buffer overflows  
✅ **Null Termination**: Always terminate strings properly  
✅ **Single Include**: Include each plugin header in only ONE .c file  
✅ **Thread Safety**: Use thread-local storage if needed  

## Usage Example

```c
#include "hello.h"
#include "plugin.h"
#include "plugins/your_plugin.h"  /* Include in ONE .c file only */

int main() {
    const char* greeting = get_greeting();
    const char* result = apply_plugins(greeting);
    printf("%s", result);
    return 0;
}
```

Build:
```bash
gcc -I. -o my_program my_program.c hello_lib.c plugin.c
```
