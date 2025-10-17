# Plugin System Architecture

## Overview

The hello.c program has been extended with a plugin architecture that allows external developers to add new features or transformations without modifying the core file.

## Design Principles

1. **Backward Compatible**: Without plugins, the program works exactly as before
2. **Non-Invasive**: Core hello.c remains simple and maintainable
3. **Runtime Loading**: Plugins are loaded dynamically via dlopen()
4. **Safe**: Graceful error handling if plugins fail to load
5. **Testable**: Unit tests work without plugin system interference

## Architecture Diagram

```
┌─────────────────────────────────────────────────────────┐
│                    hello (executable)                    │
├─────────────────────────────────────────────────────────┤
│                                                           │
│  ┌──────────────┐         ┌───────────────────────┐    │
│  │ get_greeting │         │   apply_plugin()      │    │
│  │   returns    │────────>│  - Check HELLO_PLUGIN │    │
│  │"Hello world!"│         │  - dlopen() if set    │    │
│  └──────────────┘         │  - dlsym() transform  │    │
│                           │  - Apply transform    │    │
│                           └───────────────────────┘    │
│                                      │                   │
│                                      v                   │
│                             ┌────────────────┐          │
│                             │  printf("%s")  │          │
│                             └────────────────┘          │
└─────────────────────────────────────────────────────────┘
                                      │
                                      v
              ┌───────────────────────────────────────┐
              │   Plugin (.so shared library)         │
              ├───────────────────────────────────────┤
              │ plugin_info()      - metadata         │
              │ plugin_init()      - initialization   │
              │ plugin_transform() - transformation   │
              │ plugin_cleanup()   - cleanup          │
              └───────────────────────────────────────┘
```

## Component Details

### Core Files

#### hello.c
- Main program file
- Contains `get_greeting()` function that returns base message
- Contains `apply_plugin()` function for plugin loading (conditionally compiled)
- Uses `#ifndef UNIT_TEST` to exclude plugin system from unit tests

#### hello.h
- Public API header
- Declares `get_greeting()` for use by unit tests
- Simple interface with clear contract

#### plugin.h
- Plugin API definition
- Defines `PluginInfo` struct for metadata
- Defines function pointer types for plugin functions
- Documents plugin contract and requirements

### Plugin System

#### Loading Mechanism (apply_plugin)
1. Check `HELLO_PLUGIN` environment variable
2. If not set, return original message (no transformation)
3. If set, attempt to load plugin with `dlopen()`
4. Look up `plugin_transform` function with `dlsym()`
5. Call transform function with original message
6. Return transformed message (or original on error)

#### Error Handling
- Missing plugin file: Warning printed, original message used
- Missing transform function: Warning printed, original message used  
- Transform returns NULL: Original message used
- All errors are non-fatal

### Plugin Interface

#### Required Functions

**plugin_transform(const char* message)**
- Core transformation function
- Input: original message (read-only)
- Output: transformed message
- Return NULL on error

**plugin_info(void)**
- Returns plugin metadata
- Name, version, description, API version

#### Optional Functions

**plugin_init(void)**
- Called once when plugin loads
- Return 0 for success, non-zero for failure

**plugin_cleanup(void)**
- Called before plugin unloads
- Used for resource cleanup

## Build System

### Makefile Targets

```makefile
make              # Build hello + plugins
make plugins      # Build plugins only
make test         # Run validation tests
make unit-test    # Run unit tests
make clean        # Remove build artifacts
```

### Compilation

**Main Program:**
```bash
gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -o hello hello.c -ldl
```

**Plugins:**
```bash
gcc -shared -fPIC -Wall -Wextra -o plugin.so plugin.c
```

**Unit Tests:**
```bash
gcc -DUNIT_TEST -c -o hello_lib.o hello.c
gcc -o test_runner test/test_hello.c hello_lib.o
```

## Data Flow

### Without Plugin (Default)

```
main() -> get_greeting() -> printf() -> "Hello world!"
```

### With Plugin

```
main() -> get_greeting() -> apply_plugin() -> dlopen() -> dlsym()
       -> plugin_transform() -> printf() -> "TRANSFORMED MESSAGE"
```

## Key Design Decisions

### 1. Environment Variable Configuration
- **Why**: Simple, no command-line parsing needed
- **Benefit**: Works without modifying hello.c interface
- **Usage**: `HELLO_PLUGIN=./plugin.so ./hello`

### 2. Conditional Compilation for Tests
- **Why**: Keep unit tests pure and fast
- **Benefit**: Tests verify core logic without plugin overhead
- **Implementation**: `#ifndef UNIT_TEST` guards

### 3. Union for Function Pointer Cast
- **Why**: Avoid ISO C pedantic warnings
- **Benefit**: Compiles with strictest warning levels
- **Standard**: POSIX-compliant approach

### 4. Static Buffers in Plugins
- **Why**: Simple memory management
- **Benefit**: No malloc/free complexity for basic plugins
- **Tradeoff**: Size limitations, but adequate for most use cases

### 5. No Plugin Cleanup in Current Implementation
- **Why**: Transformed message might reference plugin memory
- **Benefit**: Simplicity for basic use cases
- **Note**: Production systems should implement proper lifecycle

## Extension Points

### Adding New Features

1. **New Plugin Types**: Create additional plugin interfaces
2. **Plugin Chain**: Support multiple plugins in sequence
3. **Plugin Configuration**: Add config file or env var support
4. **Plugin Discovery**: Auto-load plugins from directory
5. **Plugin Metadata Query**: Tool to inspect plugin info

### Example: Plugin Chain

```c
const char* apply_plugin_chain(const char* message) {
    const char* result = message;
    
    // Apply each plugin in sequence
    char* plugins = getenv("HELLO_PLUGINS");
    if (plugins) {
        // Parse comma-separated list
        // Load and apply each plugin
    }
    
    return result;
}
```

## Security Considerations

1. **Untrusted Plugins**: Only load plugins from trusted sources
2. **Buffer Overflows**: Plugins must validate buffer sizes
3. **Resource Leaks**: Plugins should implement cleanup
4. **Error Handling**: Plugins should fail gracefully

## Performance

- **Without Plugin**: Zero overhead (code not compiled in)
- **With Plugin**: 
  - dlopen: ~100-500 microseconds (one-time)
  - Transform: Depends on plugin implementation
  - Total: Usually < 1ms for simple transformations

## Compatibility

- **OS**: Linux/Unix (uses dlopen, dlfcn.h)
- **Compiler**: GCC 4.8+, Clang 3.5+
- **Standard**: C99
- **Dependencies**: libdl (dynamic linking library)

## Testing

### Unit Tests
- Test `get_greeting()` function directly
- Compiled with `-DUNIT_TEST` to exclude plugin system
- Fast, no external dependencies

### Validation Tests
- Test full program with strict compilation
- Verify exact output format
- Test with and without plugins
- Validate error handling

### Plugin Tests
- Each plugin includes usage example
- Demo targets in Makefile for quick testing
- Manual verification of transformations

## Future Enhancements

1. **Plugin Registry**: Central database of available plugins
2. **Hot Reload**: Reload plugins without restarting
3. **Plugin Versioning**: Check API compatibility at runtime
4. **Plugin Configuration**: Per-plugin config files
5. **Multiple Plugins**: Support plugin chains/pipelines
6. **Plugin Sandboxing**: Isolate plugins for security
7. **Performance Profiling**: Measure plugin overhead

## References

- **plugin.h**: Complete API documentation
- **PLUGINS.md**: Plugin development guide
- **README.md**: User documentation
- **examples**: plugins/uppercase.c, plugins/reverse.c
