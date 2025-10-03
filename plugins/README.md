# Example Plugins

This directory contains example plugins demonstrating the Hello World plugin system.

## Available Plugins

### example_plugin.c - Uppercase Transformer
Converts the greeting message to uppercase.

**Build:**
```bash
gcc -o hello hello.c plugin.c plugins/example_plugin.c
```

**Output:**
```
HELLO WORLD!
```

### prefix_plugin.c - Prefix Transformer
Adds "*** " prefix to the greeting message.

**Build:**
```bash
gcc -o hello hello.c plugin.c plugins/prefix_plugin.c
```

**Output:**
```
*** Hello world!
```

## Plugin Chaining

Multiple plugins can be combined. They are applied in the order specified during compilation.

**Example - Prefix then Uppercase:**
```bash
gcc -o hello hello.c plugin.c plugins/prefix_plugin.c plugins/example_plugin.c
./hello
```

**Output:**
```
*** HELLO WORLD!
```

## Creating Your Own Plugin

See the [Plugin Development Guide](../PLUGIN_GUIDE.md) for detailed instructions on creating custom plugins.

## Plugin Template

```c
#include "../plugin.h"
#include <string.h>

static char my_buffer[256];

static char* my_transform(const char* input) {
    /* Your transformation logic */
    snprintf(my_buffer, sizeof(my_buffer), "Modified: %s", input);
    return my_buffer;
}

static plugin_interface my_plugin = {
    .process_message = my_transform,
    .plugin_name = "MyPlugin",
    .plugin_version = "1.0.0"
};

REGISTER_PLUGIN(my_plugin);
```
