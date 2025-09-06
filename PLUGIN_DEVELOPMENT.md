# Plugin System Documentation

## Overview

The hello.c program now supports a plugin architecture that allows external developers to extend functionality without modifying the core program. Plugins are dynamically loaded shared libraries that can transform messages or format output.

## Quick Start

### Building the System
```bash
make all          # Build main program and all plugins
./hello           # Run with plugins
make run-no-plugins  # Test backward compatibility
```

### Plugin Types

1. **Message Transformers** (`PLUGIN_TYPE_MESSAGE_TRANSFORMER`)
   - Modify the "Hello world!" message before output
   - Example: uppercase conversion, text replacement, language translation

2. **Output Formatters** (`PLUGIN_TYPE_OUTPUT_FORMATTER`)
   - Control how the final message is displayed
   - Example: add borders, colors, different layouts

## Creating Plugins

### Plugin Structure

Every plugin must:
1. Include `plugin.h`
2. Implement `get_plugin_interface()` function
3. Provide plugin metadata via `get_info()` function
4. Be compiled as a shared library (`.so` file)

### Basic Plugin Template

```c
#include "../plugin.h"

static plugin_info_t plugin_info = {
    .name = "my_plugin",
    .version = "1.0.0",
    .description = "Description of what this plugin does",
    .type = PLUGIN_TYPE_MESSAGE_TRANSFORMER, /* or PLUGIN_TYPE_OUTPUT_FORMATTER */
    .api_version = PLUGIN_API_VERSION
};

static plugin_info_t* get_info(void) {
    return &plugin_info;
}

static int init(void) {
    /* Optional: Initialize plugin resources */
    return 0; /* Return 0 for success, non-zero for failure */
}

static char* transform_message(const char* input) {
    /* For MESSAGE_TRANSFORMER plugins only */
    /* Return newly allocated string, or NULL to leave unchanged */
    /* The returned string will be freed by the main program */
}

static void format_output(const char* message) {
    /* For OUTPUT_FORMATTER plugins only */
    /* Print the message however you want */
    /* If no formatter handles output, default printf() is used */
}

static void cleanup(void) {
    /* Optional: Cleanup plugin resources */
}

static plugin_interface_t interface = {
    .get_info = get_info,
    .init = init,
    .transform_message = transform_message, /* NULL if not used */
    .format_output = format_output,         /* NULL if not used */
    .cleanup = cleanup
};

plugin_interface_t* get_plugin_interface(void) {
    return &interface;
}
```

### Building Plugins

```bash
# Individual plugin
gcc -Wall -Wextra -fPIC -shared -o plugins/my_plugin.so plugins/my_plugin.c

# All plugins (using Makefile)
make plugins
```

### Plugin Installation

1. Place your compiled `.so` file in the `plugins/` directory
2. The main program will automatically discover and load it

## Example Plugins

### 1. Uppercase Transformer (`plugins/uppercase_transformer.c`)
- Converts the message to uppercase
- Type: `PLUGIN_TYPE_MESSAGE_TRANSFORMER`
- Transforms "Hello world!" → "HELLO WORLD!"

### 2. Fancy Formatter (`plugins/fancy_formatter.c`)
- Adds decorative borders around the output
- Type: `PLUGIN_TYPE_OUTPUT_FORMATTER`
- Displays message in a box made of asterisks

## Plugin Execution Order

1. **Load Phase**: All plugins in `plugins/*.so` are loaded at startup
2. **Transform Phase**: Message transformers are applied in load order
3. **Format Phase**: The first output formatter found handles display
4. **Cleanup Phase**: All plugins are cleaned up at program exit

## Error Handling

- Plugin loading failures are silently ignored (allows optional plugins)
- Invalid plugins (wrong API version, missing functions) are skipped
- Plugin initialization failures prevent that plugin from being used
- The program continues to work even if all plugins fail to load

## API Compatibility

- Current API version: `PLUGIN_API_VERSION = 1`
- Plugins must match the exact API version to be loaded
- Future API changes will increment the version number

## Development Tips

1. **Memory Management**: 
   - Message transformers must return malloc()'d strings
   - The main program will free() the returned strings
   - Clean up your own resources in cleanup()

2. **Error Handling**:
   - Return NULL from transform_message() to leave message unchanged
   - Return non-zero from init() to prevent plugin loading
   - Use fprintf(stderr, ...) for error messages if needed

3. **Testing**:
   - Test your plugin with `make run` 
   - Test without plugins using `make run-no-plugins`
   - Use `rm plugins/other_plugin.so` to test individual plugins

4. **Debugging**:
   - Compile with `-g` flag for debugging symbols
   - Use `gdb ./hello` to debug the main program with plugins

## Backward Compatibility

The plugin system maintains full backward compatibility:
- Without plugins directory: works exactly like original hello.c
- With empty plugins directory: works exactly like original hello.c  
- Plugins are purely additive and optional

## Advanced Use Cases

- **Chain Transformers**: Multiple message transformers apply in sequence
- **Conditional Logic**: Plugins can check environment variables or files
- **State Management**: Use init()/cleanup() for persistent plugin state
- **External Dependencies**: Plugins can link to additional libraries