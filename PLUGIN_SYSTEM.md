# Plugin System Documentation

## Overview

The Hello World program now supports a plugin system that allows external developers to extend functionality without modifying the core `hello.c` file. The plugin system is optional and maintains full backward compatibility.

## Building

### Default (No Plugins)
```bash
make            # or make all - builds standard hello (no plugin support)
make strict     # builds with strict warnings for testing
```

### With Plugin Support
```bash
make plugin         # builds hello_plugin with plugin support
make plugin-strict  # builds with strict warnings for testing plugins
```

### Sample Plugins
```bash
make plugins        # builds sample plugins in plugins/ directory
make test-plugins   # builds and runs with sample plugins
```

## Plugin API

### Plugin Types

1. **Output Filter Plugins** (`PLUGIN_TYPE_OUTPUT_FILTER`)
   - Modify the main output text
   - Must implement `plugin_filter(const char* input) -> char*`
   - Return modified string (caller frees) or NULL for no change

2. **Preprocessor Plugins** (`PLUGIN_TYPE_PRE_PROCESSOR`) 
   - Run before main output
   - Must implement `plugin_process(void)`

3. **Postprocessor Plugins** (`PLUGIN_TYPE_POST_PROCESSOR`)
   - Run after main output  
   - Must implement `plugin_process(void)`

### Required Plugin Functions

All plugins must implement:
- `const plugin_info_t* plugin_get_info(void)` - Returns plugin information
- Type-specific function (`plugin_filter` or `plugin_process`)

Optional functions:
- `int plugin_init(void)` - Called when plugin is loaded (return 0 for success)
- `void plugin_cleanup(void)` - Called when plugin is unloaded

### Plugin Structure

```c
#include "plugin_api.h"

static const plugin_info_t plugin_info = {
    .name = "my-plugin",
    .version = "1.0.0",
    .description = "Description of what the plugin does",
    .type = PLUGIN_TYPE_OUTPUT_FILTER,  // or PRE_PROCESSOR, POST_PROCESSOR
    .api_version = PLUGIN_API_VERSION
};

const plugin_info_t* plugin_get_info(void) {
    return &plugin_info;
}

// Implement type-specific function...
```

## Sample Plugins

The repository includes three sample plugins in `plugins/examples/`:

1. **uppercase_filter.c** - Converts output to uppercase (OUTPUT_FILTER)
2. **banner_preprocessor.c** - Prints a banner before output (PRE_PROCESSOR)  
3. **timestamp_postprocessor.c** - Adds timestamp after output (POST_PROCESSOR)

## Plugin Directory Structure

```
plugins/
├── Makefile                    # Builds sample plugins
├── examples/
│   ├── uppercase_filter.c
│   ├── banner_preprocessor.c
│   └── timestamp_postprocessor.c
├── uppercase_filter.so         # Built plugins (*.so files)
├── banner_preprocessor.so
└── timestamp_postprocessor.so
```

## Usage Examples

### Without Plugins
```bash
$ ./hello
Hello world!
```

### With Plugins
```bash 
$ ./hello_plugin
Plugin loader: Scanning ./plugins for plugins...
Plugin loader: Found plugin candidate: uppercase_filter.so
[uppercase-filter] Plugin initialized
Plugin loader: Loaded uppercase-filter v1.0.0 - Converts output text to uppercase
Plugin loader: Found plugin candidate: timestamp_postprocessor.so
[timestamp] Plugin initialized
Plugin loader: Loaded timestamp-postprocessor v1.0.0 - Adds a timestamp after main output
Plugin loader: Found plugin candidate: banner_preprocessor.so
[banner] Plugin initialized
Plugin loader: Loaded banner-preprocessor v1.0.0 - Prints a banner before main output
Plugin loader: Successfully loaded 3 plugin(s)
*** Welcome to Hello World with Plugins! ***
[uppercase-filter] Converted 'Hello world!' to 'HELLO WORLD!'
HELLO WORLD!
[Generated at: 2025-09-24 07:54:30][uppercase-filter] Plugin cleaned up

[timestamp] Plugin cleaned up
[banner] Plugin cleaned up
```

## Creating Custom Plugins

1. Create a `.c` file implementing the plugin API
2. Compile as a shared library: `gcc -Wall -Wextra -std=c99 -fPIC -shared -I. -o myplugin.so myplugin.c`
3. Place the `.so` file in the `./plugins/` directory
4. Run `./hello_plugin` to load automatically

## Backward Compatibility

- The default `hello` binary has **no** plugin support and maintains exact original behavior
- Output format is preserved: "Hello world!" without trailing newline
- All existing build targets and validation continue to work
- Plugin system is completely optional

## Build Targets

- `make` / `make all` - Standard hello (no plugins)
- `make plugin` - Plugin-enabled hello
- `make plugins` - Build sample plugins  
- `make test-plugins` - Test with plugins loaded
- `make validate` - Validate standard build
- `make validate-plugin` - Validate plugin build
- `make clean` - Clean all build artifacts
- `make help` - Show all available targets