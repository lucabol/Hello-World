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

### exclamation_plugin.h
Adds extra exclamation marks to the greeting.

**Example:**
```c
#include "plugins/exclamation_plugin.h"
// Transforms "Hello world!" to "Hello world!!!"
```

## Creating Your Own Plugin

1. Create a new `.h` file in this directory
2. Follow the pattern in the example plugins
3. Include your plugin header in your main program
4. Build and test

See the [Plugin Guide](../PLUGIN_GUIDE.md) for detailed instructions.

## Usage Example

```c
#include "hello.h"
#include "plugin.h"
#include "plugins/your_plugin.h"

int main() {
    const char* greeting = get_greeting();
    const char* result = apply_plugins(greeting);
    printf("%s", result);
    return 0;
}
```

Build:
```bash
gcc -o my_program my_program.c hello_lib.c plugin.c
```
