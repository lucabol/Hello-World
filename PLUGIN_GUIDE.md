# Plugin System Documentation

## Overview

The hello.c plugin system allows external developers to extend functionality without modifying the core hello.c file. Plugins can transform the output message and execute code before and after the message is printed.

The system uses a **safe out-buffer API** to prevent buffer overflow and memory corruption issues.

## Architecture

The plugin system uses a simple, compile-time registration mechanism:
- Plugins are statically linked at compile time
- Multiple plugins can be registered and chain together safely
- **Double-buffering** prevents buffer clobbering during chaining
- No dynamic library loading required (keeps it simple)
- Backward compatible - hello.c works without plugins when compiled normally

### Safety Features

- Out-buffer API prevents static buffer issues
- Buffer overflow detection
- Error handling for transform failures
- Truncation warnings
- Double-buffering for safe plugin chaining

## Creating a Plugin

### Basic Plugin Structure (New Safe API)

```c
#include "plugin.h"
#include <stdio.h>
#include <string.h>

/* Transform function - modifies the message safely */
static int my_transform(const char* input, char* output, size_t output_size) {
    int result = snprintf(output, output_size, "Modified: %s", input);
    
    /* IMPORTANT: Always check for buffer overflow */
    if (result < 0 || (size_t)result >= output_size) {
        return -1;  /* Error: buffer too small */
    }
    
    return 0;  /* Success */
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
   - **Type**: `int (*)(const char* input, char* output, size_t output_size)`
   - **Parameters**:
     - `input`: The input message (null-terminated, read-only)
     - `output`: Buffer to write transformed message (must null-terminate)
     - `output_size`: Size of the output buffer in bytes
   - **Returns**:
     - `0` on success
     - Non-zero on error (e.g., buffer too small)
   - **CRITICAL**: Always check that output fits in the buffer
   - Pass `NULL` if no transformation is needed

2. **Before Hook** (optional)
   - **Type**: `void (*)(void)`
   - Executes before the message is printed
   - Useful for headers, logging, initialization
   - Pass `NULL` if not needed

3. **After Hook** (optional)
   - **Type**: `void (*)(void)`
   - Executes after the message is printed
   - Useful for footers, cleanup, logging
   - Pass `NULL` if not needed

### Registration

**Automatic (GCC/Clang only):**
```c
PLUGIN_REGISTER(PluginName, transform_fn, before_fn, after_fn);
```

**Manual (All compilers):**
```c
void init_plugins(void) {
    plugin_register("PluginName", transform_fn, before_fn, after_fn);
}
```

## Example Plugins with New API

### Example 1: Uppercase Transformer

```c
#include "plugin.h"
#include <ctype.h>
#include <string.h>

static int uppercase_transform(const char* input, char* output, size_t output_size) {
    size_t len = strlen(input);
    
    /* Check buffer size */
    if (len >= output_size) {
        return -1;  /* Buffer too small */
    }
    
    /* Transform to uppercase */
    for (size_t i = 0; i < len; i++) {
        output[i] = (char)toupper((unsigned char)input[i]);
    }
    output[len] = '\0';
    
    return 0;  /* Success */
}

PLUGIN_REGISTER(UppercasePlugin, uppercase_transform, NULL, NULL);
```

### Example 2: Decorator with Hooks

```c
#include "plugin.h"
#include <stdio.h>

static int decorator_transform(const char* input, char* output, size_t output_size) {
    int result = snprintf(output, output_size, "*** %s ***", input);
    
    /* Check for overflow */
    if (result < 0 || (size_t)result >= output_size) {
        return -1;  /* Buffer too small */
    }
    
    return 0;
}

static void decorator_before(void) {
    printf("=== Plugin Output Start ===\n");
}

static void decorator_after(void) {
    printf("\n=== Plugin Output End ===\n");
}

PLUGIN_REGISTER(DecoratorPlugin, decorator_transform, decorator_before, decorator_after);
```

### Example 3: Hooks-Only Logger

```c
#include "plugin.h"
#include <stdio.h>
#include <time.h>

static void logger_before(void) {
    time_t now = time(NULL);
    printf("[%s] Starting\n", ctime(&now));
}

static void logger_after(void) {
    printf("\n[INFO] Complete\n");
}

/* No transform - just hooks */
PLUGIN_REGISTER(LoggerPlugin, NULL, logger_before, logger_after);
```

## Safe Plugin Chaining

Multiple plugins chain safely using double-buffering:

```bash
gcc -DUSE_PLUGINS -o hello hello.c plugin.c plugin_decorator.c plugin_uppercase.c
./hello
```

**Output:**
```
=== Plugin Output Start ===
*** HELLO WORLD! ***
=== Plugin Output End ===
```

**How it works:**
1. System starts with input: "Hello world!"
2. Decorator transforms into Buffer A: "*** Hello world! ***"
3. Uppercase reads Buffer A, transforms into Buffer B: "*** HELLO WORLD! ***"
4. System prints final result from Buffer B

**No buffer clobbering** - each transform has its own input and output buffer.

## Buffer Safety - Critical Best Practices

### ✅ CORRECT: Always Check Buffer Size

```c
static int safe_transform(const char* input, char* output, size_t output_size) {
    /* Use snprintf and check return value */
    int result = snprintf(output, output_size, "Prefix: %s", input);
    
    /* Critical: check for overflow */
    if (result < 0 || (size_t)result >= output_size) {
        return -1;  /* Signal error */
    }
    
    return 0;  /* Success */
}
```

### ❌ WRONG: Common Mistakes

```c
/* WRONG - No buffer size check */
static int bad1(const char* input, char* output, size_t output_size) {
    sprintf(output, "Prefix: %s", input);  /* BUFFER OVERFLOW! */
    return 0;
}

/* WRONG - Doesn't check snprintf return */
static int bad2(const char* input, char* output, size_t output_size) {
    snprintf(output, output_size, "Prefix: %s", input);
    return 0;  /* Might have truncated without detecting */
}

/* WRONG - Returns static buffer (old API, deprecated) */
static const char* bad3(const char* input) {
    static char buffer[256];
    snprintf(buffer, sizeof(buffer), "Prefix: %s", input);
    return buffer;  /* WRONG API! */
}
```

## Portability and Limitations

### Compiler Support

| Compiler | Support | Notes |
|----------|---------|-------|
| GCC | ✅ Full | Auto-registration works |
| Clang | ✅ Full | Auto-registration works |
| MSVC | ⚠️ Manual | Use explicit `plugin_register()` |
| Others | ❓ Maybe | If `__attribute__((constructor))` supported |

### MSVC and Manual Registration

For compilers that don't support `__attribute__((constructor))` (like MSVC), you must use explicit registration:

```c
// my_plugin.c - MSVC-compatible plugin
#include "plugin.h"
#include <stdio.h>

static int my_transform(const char* input, char* output, size_t output_size) {
    int result = snprintf(output, output_size, "MSVC: %s", input);
    if (result < 0 || (size_t)result >= output_size) {
        return PLUGIN_ERROR_BUFFER_TOO_SMALL;
    }
    return PLUGIN_SUCCESS;
}

// Explicit registration function - call from main() or init function
void my_plugin_init(void) {
    plugin_register("MyPlugin", my_transform, NULL, NULL);
}
```

Then in your main application:

```c
// main.c or hello.c with MSVC
#include "plugin.h"

// Declare plugin init functions
extern void my_plugin_init(void);

int main(void) {
    #ifdef USE_PLUGINS
    // Manually register plugins
    my_plugin_init();
    
    // Now execute plugins as normal
    char output_buffer[1024];
    plugin_execute_before_hooks();
    plugin_execute_transforms("Hello world!", output_buffer, sizeof(output_buffer));
    printf("%s", output_buffer);
    plugin_execute_after_hooks();
    #else
    printf("Hello world!");
    #endif
    return 0;
}
```

**Note**: When using manual registration, define `PLUGIN_MANUAL_REGISTRATION` to suppress the compiler error:
```bash
cl /DUSE_PLUGINS /DPLUGIN_MANUAL_REGISTRATION hello.c plugin.c my_plugin.c
```

### Error Handling Behavior

**What happens when a transform returns an error?**

The plugin execution engine handles errors as follows:

1. **Transform returns `PLUGIN_SUCCESS` (0)**: 
   - Output is passed to the next plugin in the chain
   - Execution continues normally

2. **Transform returns error code (non-zero)**:
   - Error is logged to stderr with plugin name and error code
   - **The last successful transform output is used** (NOT the original input)
   - Error code is returned from `plugin_execute_transforms()`
   - Subsequent transforms are **skipped** (chain stops)
   - `after` hooks still execute

3. **hello.c error handling**:
   - If `plugin_execute_transforms()` returns non-zero, hello.c falls back to printing the original message
   - After hooks still execute even on error

**Example error scenario:**
```
Plugin chain: uppercase -> decorator -> failing_plugin

1. uppercase: "Hello" -> "HELLO" (success)
2. decorator: "HELLO" -> "***HELLO***" (success)  
3. failing_plugin: returns PLUGIN_ERROR_BUFFER_TOO_SMALL

Result: hello.c prints original "Hello world!" (fallback)
stderr: "ERROR: Plugin 'failing_plugin' transform failed (code -1)"
```

**Testing error behavior**: See test_plugins.sh for examples of error handling tests.

### Registration Order

⚠️ **Important Limitation**: Constructor execution order across translation units is **NOT guaranteed** by C standard.

- Plugin order depends on link order
- Order is deterministic for a given build
- Don't rely on specific order for correctness
- Test plugins in different orders

### Thread Safety

⚠️ **Not Thread-Safe**:
- All registration must complete before execution
- Transform functions receive independent buffers
- Avoid mutable global state in transforms
- Provide your own synchronization if needed

### Buffer Limits

- Default: 1024 bytes (configurable with `-DPLUGIN_BUFFER_SIZE=N`)
- Input and output limited to buffer size
- Exceeding limit causes errors/warnings

## Compilation

```bash
# Without plugins (default)
gcc -o hello hello.c
./hello
# Output: Hello world!

# With single plugin
gcc -DUSE_PLUGINS -o hello hello.c plugin.c plugin_uppercase.c
./hello
# Output: HELLO WORLD!

# With multiple plugins
gcc -DUSE_PLUGINS -o hello hello.c plugin.c plugin_decorator.c plugin_uppercase.c
./hello
# Output:
# === Plugin Output Start ===
# *** HELLO WORLD! ***
# === Plugin Output End ===

# Using Makefile
make uppercase      # Build with uppercase only
make decorator      # Build with decorator only
make logger         # Build with logger only
make with-plugins   # Build with all examples
```

### Build Options

```bash
# Custom max plugins
gcc -DMAX_PLUGINS=20 -DUSE_PLUGINS -o hello hello.c plugin.c my_plugin.c

# Custom buffer size
gcc -DPLUGIN_BUFFER_SIZE=4096 -DUSE_PLUGINS -o hello hello.c plugin.c my_plugin.c

# Strict compilation (recommended)
gcc -Wall -Wextra -Wpedantic -Werror -DUSE_PLUGINS -o hello hello.c plugin.c my_plugin.c
```

## Testing

### Run Test Suite

```bash
./test_plugins.sh
```

Tests include:
- Core functionality without plugins ✓
- Individual plugin transforms ✓
- Multi-plugin chaining (2-3 plugins) ✓
- Hooks-only plugins ✓
- Hooks + transforms combined ✓
- Strict compilation (-Werror) ✓
- Buffer safety verification ✓

### Unit Testing Your Plugin

```c
#include "plugin.h"
#include <assert.h>
#include <string.h>

void test_my_transform(void) {
    char output[256];
    int result = my_transform("input", output, sizeof(output));
    
    assert(result == 0);  /* Should succeed */
    assert(strcmp(output, "expected") == 0);
}

void test_overflow(void) {
    char tiny[5];
    int result = my_transform("long input", tiny, sizeof(tiny));
    
    assert(result != 0);  /* Should fail gracefully */
}
```

##Troubleshooting

| Problem | Solution |
|---------|----------|
| Plugin not registering | Compile with `-DUSE_PLUGINS`, include plugin file |
| "Max plugins reached" | Increase with `-DMAX_PLUGINS=N` |
| Output truncated | Increase with `-DPLUGIN_BUFFER_SIZE=N` |
| Segfault | Check buffer handling, null termination, bounds |
| Chaining broken | Verify return values, test individually first |
| Compilation error | Check API signature matches new out-buffer model |
| Stack overflow with large buffers | Use `-DPLUGIN_USE_HEAP` for heap allocation |

## Configuration Options

### Stack vs Heap Allocation

**Default (Stack Allocation):**
- Uses 2 × PLUGIN_BUFFER_SIZE bytes on the stack (default: 2KB)
- Fast, no allocation overhead
- **Recommended** for typical desktop/server applications
- Safe up to PLUGIN_BUFFER_SIZE=4096 (compiler warning if exceeded)

**Heap Allocation Mode:**
Use when:
- Running on embedded systems with small stack limits
- Using large PLUGIN_BUFFER_SIZE values (>4KB)
- Thread stacks are limited (e.g., <16KB)

**Example:**
```bash
# Enable heap allocation
gcc -DUSE_PLUGINS -DPLUGIN_USE_HEAP -o hello hello.c plugin.c my_plugin.c

# Large buffer with heap allocation
gcc -DUSE_PLUGINS -DPLUGIN_USE_HEAP -DPLUGIN_BUFFER_SIZE=16384 -o hello hello.c plugin.c my_plugin.c
```

**Performance Notes:**
- Stack allocation: ~0 overhead
- Heap allocation: Small malloc/free overhead per transform call
- Difference is negligible for typical use cases

### Thread-Safety Considerations

**CRITICAL: Registration must happen before threads are spawned!**

The plugin system is NOT thread-safe during registration. Follow these rules:

1. **Single-threaded registration**: All `plugin_register()` calls (automatic or manual) must complete before spawning threads
2. **No concurrent registration**: Never call `plugin_register()` from multiple threads
3. **Transform functions**: Should be reentrant (no mutable global state)

**Safe Pattern:**
```c
int main(void) {
    /* Plugin registration happens here (constructors or explicit calls) */
    
    /* Spawn threads after registration complete */
    pthread_create(...);
    
    /* NOW safe to call plugin_execute_* from multiple threads
       (if transforms themselves are reentrant) */
}
```

**Unsafe Pattern (DON'T DO THIS):**
```c
void* thread_func(void* arg) {
    plugin_register(...);  /* ❌ RACE CONDITION! */
    return NULL;
}
```

### Link-Time Optimization (LTO) and Static Libraries

When using LTO or building as a static library, linkers may strip constructor functions:

**Solution 1: Explicit Registration (Recommended)**
```c
/* In my_plugin.c */
void my_plugin_init(void) {
    plugin_register("MyPlugin", transform_fn, before_fn, after_fn);
}

/* In main.c */
extern void my_plugin_init(void);

int main(void) {
    my_plugin_init();  /* Explicit call */
    /* ... */
}
```

**Solution 2: Linker Flags**
```bash
# GNU ld
gcc -Wl,--whole-archive libmy_plugins.a -Wl,--no-whole-archive

# Or use explicit symbol references
gcc -Wl,--undefined=__plugin_init_MyPlugin
```

**Solution 3: Reference Plugin Symbols**
```c
/* Force linker to keep plugin */
extern void __plugin_init_MyPlugin(void) __attribute__((weak));

int main(void) {
    if (__plugin_init_MyPlugin) {
        /* Constructor will have run */
    }
}
```

## Troubleshooting

## Migration from Old API (if applicable)

If you have plugins written for an earlier draft:

**Old API (deprecated):**
```c
const char* transform(const char* input) {
    static char buffer[256];
    snprintf(buffer, sizeof(buffer), "...%s", input);
    return buffer;  /* Returns static pointer */
}
```

**New API (current):**
```c
int transform(const char* input, char* output, size_t output_size) {
    int result = snprintf(output, output_size, "...%s", input);
    if (result < 0 || (size_t)result >= output_size) {
        return -1;  /* Error */
    }
    return 0;  /* Success */
}
```

**Changes:**
1. Remove static buffer declaration
2. Write to `output` parameter instead of static buffer
3. Check `snprintf` return value
4. Return 0 on success, -1 on error
5. Update `PLUGIN_REGISTER` call if needed

## Advanced Topics

### Error Handling

```c
static int robust_transform(const char* input, char* output, size_t output_size) {
    size_t len = strlen(input);
    
    /* Pre-check required size */
    if (len + 10 >= output_size) {  /* 10 = overhead */
        fprintf(stderr, "ERROR: Input too long: %zu bytes\n", len);
        return -1;
    }
    
    /* Transform */
    int result = snprintf(output, output_size, "<<<%s>>>", input);
    
    /* Verify */
    if (result < 0 || (size_t)result >= output_size) {
        return -1;
    }
    
    return 0;
}
```

### Conditional Behavior

```c
#include <stdlib.h>

static int conditional_transform(const char* input, char* output, size_t output_size) {
    const char* enabled = getenv("MY_PLUGIN_ENABLED");
    
    if (enabled && strcmp(enabled, "1") == 0) {
        /* Transform */
        int result = snprintf(output, output_size, "[ON] %s", input);
        return (result < 0 || (size_t)result >= output_size) ? -1 : 0;
    }
    
    /* Pass through */
    if (strlen(input) >= output_size) {
        return -1;
    }
    strcpy(output, input);
    return 0;
}
```

## Complete Template

See `plugin_repeat_example.c` for a complete, documented template showing:
- Safe buffer handling with new API
- Error checking best practices
- Before/after hooks
- Inline documentation

Copy this file as a starting point for your own plugins.

## See Also

- `PLUGIN_SUMMARY.md` - Implementation overview
- `CHANGELOG.md` - Version history and breaking changes
- `README.md` - Quick start
- `plugin.h` - API reference with detailed comments
- `test_plugins.sh` - Test suite source

## References

- Buffer safety: Always use `snprintf` and check return values
- GCC constructor attribute: `__attribute__((constructor))`
- C standard: Registration order not guaranteed across translation units

