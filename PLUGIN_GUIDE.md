# Plugin System Documentation

## Overview

The hello.c plugin system allows external developers to extend functionality without modifying the core hello.c file. Plugins can transform the output message and execute code before and after the message is printed.

## Architecture

The plugin system uses a simple, compile-time registration mechanism:
- Plugins are statically linked at compile time
- Multiple plugins can be registered and will execute in registration order
- No dynamic library loading required (keeps it simple)
- Backward compatible - hello.c works without plugins when compiled normally

## Creating a Plugin

### Basic Plugin Structure

```c
#include "plugin.h"
#include <stdio.h>
#include <string.h>

/* Transform function - modifies the message */
static const char* my_transform(const char* input) {
    static char buffer[256];
    snprintf(buffer, sizeof(buffer), "Modified: %s", input);
    return buffer;
}

/* Before hook - executes before message is printed */
static void my_before(void) {
    printf("Starting...\n");
}

/* After hook - executes after message is printed */
static void my_after(void) {
    printf("\nFinished!\n");
}

/* Register the plugin */
PLUGIN_REGISTER(MyPlugin, my_transform, my_before, my_after);
```

### Plugin Components

1. **Transform Function** (optional)
   - Type: `const char* (*)(const char* input)`
   - Receives the current message and returns a transformed version
   - Must return a pointer to static or persistent storage
   - Can return the input unchanged by returning `input` directly
   - Pass `NULL` if no transformation is needed

2. **Before Hook** (optional)
   - Type: `void (*)(void)`
   - Executes before the message is printed
   - Useful for printing headers, initializing state, etc.
   - Pass `NULL` if not needed

3. **After Hook** (optional)
   - Type: `void (*)(void)`
   - Executes after the message is printed
   - Useful for printing footers, cleanup, etc.
   - Pass `NULL` if not needed

### Registration

Use the `PLUGIN_REGISTER` macro to register your plugin:

```c
PLUGIN_REGISTER(PluginName, transform_fn, before_fn, after_fn);
```

Note: The first parameter is an identifier (not a string), which will be converted to a string name automatically.

The registration happens automatically at program startup using GCC's constructor attribute.

## Compilation

### Compiling with Plugins

```bash
# Compile with a single plugin
gcc -DUSE_PLUGINS -o hello hello.c plugin.c plugin_uppercase.c

# Compile with multiple plugins
gcc -DUSE_PLUGINS -o hello hello.c plugin.c plugin_uppercase.c plugin_decorator.c

# Compile without plugins (original behavior)
gcc -o hello hello.c
```

### Build Flags

- `-DUSE_PLUGINS`: Enable plugin system in hello.c
- Without this flag, hello.c behaves exactly as the original

## Example Plugins

### 1. Uppercase Plugin (`plugin_uppercase.c`)

Transforms the message to uppercase letters.

```bash
gcc -DUSE_PLUGINS -o hello hello.c plugin.c plugin_uppercase.c
./hello
# Output: HELLO WORLD!
```

### 2. Decorator Plugin (`plugin_decorator.c`)

Adds decorative borders and headers/footers.

```bash
gcc -DUSE_PLUGINS -o hello hello.c plugin.c plugin_decorator.c
./hello
# Output:
# === Plugin Output Start ===
# *** Hello world! ***
# === Plugin Output End ===
```

### 3. Multiple Plugins

Plugins execute in the order they are linked:

```bash
gcc -DUSE_PLUGINS -o hello hello.c plugin.c plugin_decorator.c plugin_uppercase.c
./hello
# Output:
# === Plugin Output Start ===
# *** HELLO WORLD! ***
# === Plugin Output End ===
```

Note: The decorator transforms first (adds ***), then uppercase transforms the result.

## Plugin Execution Order

1. All `before` hooks execute in registration order
2. All `transform` functions execute in registration order (each transformation feeds into the next)
3. The final message is printed
4. All `after` hooks execute in registration order

## Best Practices

1. **Static Storage**: Transform functions must return pointers to static or persistent storage, not stack-allocated buffers
2. **Buffer Safety**: Always check buffer sizes to avoid overflows
3. **Minimal Changes**: Keep plugins small and focused on a single task
4. **NULL Checks**: You can pass NULL for any function you don't need
5. **Idempotence**: Transform functions should be safe to call multiple times
6. **Thread Safety**: The current implementation is not thread-safe

## Limitations

- Maximum 10 plugins can be registered (see `MAX_PLUGINS` in plugin.h)
- Plugins are statically linked at compile time (no runtime loading)
- No plugin configuration or parameters beyond compile-time
- Transform functions must use static buffers or return the input pointer
- Not thread-safe in current implementation

## Advanced Usage

### Conditional Plugins

You can create plugins that conditionally transform based on environment:

```c
static const char* conditional_transform(const char* input) {
    const char* env = getenv("MY_PLUGIN_ENABLED");
    if (env && strcmp(env, "1") == 0) {
        static char buffer[256];
        snprintf(buffer, sizeof(buffer), "MODIFIED: %s", input);
        return buffer;
    }
    return input; /* No transformation */
}
```

### Plugin Chains

Plugins naturally form a chain - each transform receives the output of the previous transform:

```
Original message -> Plugin1.transform -> Plugin2.transform -> ... -> Final output
```

This allows for composable transformations without plugins needing to know about each other.

## Troubleshooting

**Problem**: Plugin doesn't register
- **Solution**: Make sure you're compiling with `-DUSE_PLUGINS`
- **Solution**: Verify your plugin file is included in the compilation

**Problem**: Message gets corrupted
- **Solution**: Check that your transform function returns static storage, not a stack variable
- **Solution**: Verify buffer sizes are adequate

**Problem**: "Maximum number of plugins reached" warning
- **Solution**: Increase `MAX_PLUGINS` in plugin.h or reduce the number of plugins

## Future Enhancements

Potential improvements for the plugin system:
- Runtime plugin loading via shared libraries
- Plugin configuration files
- Thread-safe plugin execution
- Plugin dependencies and ordering
- Error handling and recovery
- Plugin statistics and profiling
