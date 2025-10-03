# Hello World

A simple, extensible C program demonstrating a plugin architecture.

## Overview

This is a "Hello World" program with a twist - it features a powerful plugin system that allows developers to extend functionality without modifying the core code. External developers can create plugins that transform or modify the greeting message in any way they choose.

## Features

- **Plugin Architecture**: Extensible design supporting compile-time plugins
- **Zero Core Modifications**: Add new features without changing `hello.c`
- **Plugin Chaining**: Multiple plugins can be combined and applied sequentially
- **Simple API**: Easy-to-use interface for plugin development
- **Type-Safe**: C-based with strong typing
- **Strict Compilation**: Compiles cleanly with `-Werror -Wpedantic`

## Quick Start

### Basic Usage (No Plugins)

```bash
gcc -o hello hello.c plugin.c
./hello
```

**Output:** `Hello world!`

### With Example Plugin (Uppercase)

```bash
gcc -o hello hello.c plugin.c plugins/example_plugin.c
./hello
```

**Output:** `HELLO WORLD!`

### With Multiple Plugins (Chaining)

```bash
gcc -o hello hello.c plugin.c plugins/prefix_plugin.c plugins/example_plugin.c
./hello
```

**Output:** `*** HELLO WORLD!`

## Plugin System

The plugin system allows external developers to extend the program's functionality without modifying the core files. Plugins are:

- **Compile-time linked**: No runtime dynamic loading overhead
- **Type-safe**: Strongly typed C interface
- **Chainable**: Multiple plugins can be combined
- **Independent**: Each plugin is a separate file
- **Simple**: Easy to create and understand

### Available Example Plugins

1. **example_plugin.c** - Converts message to uppercase
2. **prefix_plugin.c** - Adds "*** " prefix to the message

### Creating Your Own Plugin

See the comprehensive [Plugin Development Guide](PLUGIN_GUIDE.md) for detailed instructions, examples, and best practices.

**Quick Template:**

```c
#include "../plugin.h"
#include <string.h>

static char my_buffer[256];

static char* my_transform(const char* input) {
    snprintf(my_buffer, sizeof(my_buffer), "Custom: %s", input);
    return my_buffer;
}

static plugin_interface my_plugin = {
    .process_message = my_transform,
    .plugin_name = "MyPlugin",
    .plugin_version = "1.0.0"
};

REGISTER_PLUGIN(my_plugin);
```

## Building

### Standard Build

```bash
gcc -Wall -Wextra -o hello hello.c plugin.c
```

### Strict Build (Production)

```bash
gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -o hello hello.c plugin.c
```

### With Plugins

```bash
gcc -Wall -Wextra -o hello hello.c plugin.c plugins/your_plugin.c
```

## Repository Structure

```
.
├── hello.c                 # Main program
├── plugin.h                # Plugin interface definition
├── plugin.c                # Plugin system implementation
├── plugins/                # Example plugins directory
│   ├── README.md          # Plugin examples overview
│   ├── example_plugin.c   # Uppercase transformation
│   └── prefix_plugin.c    # Prefix transformation
├── PLUGIN_GUIDE.md        # Comprehensive plugin development guide
├── test/                  # Test framework and validation
│   ├── simple_test.h      # Testing framework
│   ├── test_hello.c       # Unit tests
│   └── validate.sh        # Validation script
└── README.md              # This file
```

## Documentation

- **[PLUGIN_GUIDE.md](PLUGIN_GUIDE.md)** - Complete guide to creating plugins
- **[plugins/README.md](plugins/README.md)** - Example plugins documentation

## Contributing

Contributions are welcome! To contribute a new plugin:

1. Create your plugin in the `plugins/` directory
2. Follow the plugin interface defined in `plugin.h`
3. Test with strict compilation flags
4. Add documentation to your plugin file
5. Submit a pull request

## License

Test repo for JediMaster