# Plugin System for hello.c

This directory contains plugins that can extend the functionality of hello.c without modifying the core file.

## Plugin Architecture

The plugin system provides a simple and extensible way to transform the greeting message through a chain of plugins. Each plugin implements the `plugin_info_t` interface defined in `plugin.h`.

### Plugin Interface

Each plugin must:
1. Include `../plugin.h`
2. Implement a `plugin_transform` function
3. Export a global `plugin_info` structure

### Plugin Functions

- **init** (optional): Called when the plugin is loaded
- **transform** (required): Transforms the input string to output string
- **cleanup** (optional): Called before the plugin is unloaded

## Example Plugins

### uppercase.c
Converts the greeting message to uppercase.

**Input:** `Hello world!`  
**Output:** `HELLO WORLD!`

### exclaim.c
Adds extra exclamation marks to the greeting.

**Input:** `Hello world!`  
**Output:** `Hello world!!!`

## Creating a New Plugin

1. Create a new `.c` file in the `plugins/` directory
2. Include the plugin header:
   ```c
   #include "../plugin.h"
   ```

3. Implement the transform function:
   ```c
   static int my_transform(const char* input, char* output, size_t output_size) {
       /* Your transformation logic here */
       return PLUGIN_SUCCESS;
   }
   ```

4. Export the plugin info:
   ```c
   plugin_info_t plugin_info = {
       .api_version = PLUGIN_API_VERSION,
       .name = "my_plugin",
       .version = "1.0.0",
       .init = NULL,
       .transform = my_transform,
       .cleanup = NULL
   };
   ```

## Building with Plugins

### Compile the plugin system:
```bash
gcc -c plugin.c -o plugin.o
```

### Compile a plugin:
```bash
gcc -c plugins/uppercase.c -o plugins/uppercase.o
```

### Link with hello program:
```bash
gcc -o hello_with_plugins hello_plugin_demo.c plugin.o plugins/uppercase.o
```

### Run the demo:
```bash
./hello_with_plugins
```

## Plugin Chain

Plugins are applied in the order they are registered. Each plugin receives the output of the previous plugin as its input.

Example chain:
```
Original -> Plugin 1 -> Plugin 2 -> Plugin 3 -> Final Output
```

## Plugin Guidelines

1. **Keep it simple**: Plugins should do one thing well
2. **Check bounds**: Always validate buffer sizes to prevent overflows
3. **Return codes**: Use PLUGIN_SUCCESS (0) or PLUGIN_ERROR (-1)
4. **No side effects**: Avoid modifying global state
5. **Thread safety**: Consider thread safety if used in multi-threaded environments

## API Reference

See `plugin.h` for the complete API documentation.

## Future Enhancements

- Dynamic plugin loading with dlopen/LoadLibrary
- Plugin configuration files
- Plugin dependencies
- Plugin priorities
- Async plugin execution
