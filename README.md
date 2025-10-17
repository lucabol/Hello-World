# Hello World with Plugin System

A simple C "Hello World" program with an extensible plugin architecture that allows external developers to add new features or transformations without modifying the core code.

## Quick Start

```bash
# Build the program
make

# Run normally
./hello
# Output: Hello world!

# Run with uppercase plugin
HELLO_PLUGIN=./plugins/uppercase.so ./hello
# Output: HELLO WORLD!

# Run with reverse plugin
HELLO_PLUGIN=./plugins/reverse.so ./hello
# Output: !dlrow olleH
```

## Features

- **Simple Core**: Maintains the original "Hello world!" functionality
- **Plugin Architecture**: Dynamically load transformations at runtime
- **Zero Modifications**: Add features without changing hello.c
- **Example Plugins**: Uppercase and reverse transformations included
- **Full Test Suite**: Unit tests and validation tests ensure quality

## Building

### Build Everything
```bash
make all          # Build hello + plugins
make              # Same as 'make all'
```

### Build Components
```bash
make hello        # Build main program only
make plugins      # Build all plugins only
```

### Testing
```bash
make test         # Run validation tests
make unit-test    # Run unit tests
```

### Demonstrations
```bash
make demo-uppercase   # Demo with uppercase plugin
make demo-reverse     # Demo with reverse plugin
```

## Plugin Development

### Creating a Plugin

1. Create a new `.c` file in the `plugins/` directory
2. Include the plugin API header: `#include "../plugin.h"`
3. Implement required functions:

```c
#include "../plugin.h"
#include <string.h>

static char buffer[256];

/* Required: Transform function */
const char* plugin_transform(const char* message) {
    // Your transformation logic here
    // Return transformed message (or NULL on error)
    return buffer;
}

/* Required: Plugin metadata */
PluginInfo plugin_info(void) {
    PluginInfo info;
    info.name = "My Plugin";
    info.version = "1.0.0";
    info.description = "Does something cool";
    info.api_version = PLUGIN_API_VERSION;
    return info;
}

/* Optional: Initialization */
int plugin_init(void) {
    return 0;  // Return 0 on success
}

/* Optional: Cleanup */
void plugin_cleanup(void) {
    // Cleanup resources
}
```

### Building Your Plugin

```bash
gcc -shared -fPIC -Wall -Wextra -o plugins/myplugin.so plugins/myplugin.c
```

Or add to Makefile:
```makefile
PLUGINS = $(PLUGIN_DIR)/myplugin.so $(PLUGIN_DIR)/uppercase.so ...
```

### Using Your Plugin

```bash
HELLO_PLUGIN=./plugins/myplugin.so ./hello
```

## Plugin API Reference

### Required Functions

#### `plugin_transform(const char* message)`
- **Purpose**: Transform the greeting message
- **Parameters**: Original message (read-only, do not free)
- **Returns**: Transformed message (static or heap-allocated)
- **Note**: Main transformation function called for each message

#### `plugin_info(void)`
- **Purpose**: Provide plugin metadata
- **Returns**: PluginInfo struct with name, version, description, API version

### Optional Functions

#### `plugin_init(void)`
- **Purpose**: Initialize plugin resources
- **Returns**: 0 on success, non-zero on failure
- **Note**: Called once when plugin is loaded

#### `plugin_cleanup(void)`
- **Purpose**: Clean up plugin resources
- **Note**: Called once before plugin is unloaded

## Example Plugins

### Uppercase Plugin
Converts greeting to uppercase letters.
```bash
HELLO_PLUGIN=./plugins/uppercase.so ./hello
# Output: HELLO WORLD!
```

### Reverse Plugin
Reverses the character order.
```bash
HELLO_PLUGIN=./plugins/reverse.so ./hello
# Output: !dlrow olleH
```

## Architecture

### Core Components

- **hello.c**: Main program with plugin loading capability
- **hello.h**: Public API for greeting function (used by unit tests)
- **plugin.h**: Plugin API definitions and types

### Plugin System

The plugin system uses dynamic loading (dlopen/dlsym) to load plugins at runtime:

1. Check `HELLO_PLUGIN` environment variable
2. Load plugin shared library if specified
3. Look up `plugin_transform` function
4. Apply transformation to greeting message
5. Output transformed message

### Design Principles

- **Minimal Changes**: Core hello.c remains simple and readable
- **Backward Compatible**: Works exactly as before without plugins
- **Safe**: Validates plugin loading and handles errors gracefully
- **Testable**: Unit tests verify core functionality independently

## Testing

### Validation Tests
```bash
make test
# or
./test/validate.sh
```

Validates:
- Strict compilation with GCC
- Correct output format ("Hello world!" without trailing newline)
- Exit code (0)
- No runtime errors

### Unit Tests
```bash
make unit-test
# or
./test/run_unit_tests.sh
```

Tests:
- `get_greeting()` returns non-null
- Returns "Hello world!"
- Correct string length
- No trailing newline
- Static storage (pointer stability)

## Requirements

- **Compiler**: GCC or Clang with C99 support
- **OS**: Linux/Unix (uses dlopen for dynamic loading)
- **Build Tools**: Make (optional, can use gcc directly)

## Project Structure

```
.
├── hello.c              # Main program with plugin support
├── hello.h              # Public greeting API
├── plugin.h             # Plugin API definition
├── Makefile             # Build system
├── plugins/             # Plugin directory
│   ├── uppercase.c      # Uppercase transformation plugin
│   └── reverse.c        # Reverse transformation plugin
├── test/                # Test suite
│   ├── validate.sh      # Validation tests
│   ├── run_unit_tests.sh # Unit test runner
│   ├── test_hello.c     # Unit test code
│   └── simple_test.h    # Test framework
└── README.md            # This file
```

## License

Test repo for JediMaster

## Contributing

To add new plugins:
1. Create plugin source in `plugins/`
2. Follow the plugin API (see plugin.h)
3. Add build rule to Makefile
4. Test with `HELLO_PLUGIN=./plugins/yourplugin.so ./hello`
