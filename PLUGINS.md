# Plugin Development Guide

This guide provides detailed information for developers who want to create plugins for the hello.c program.

## Table of Contents

1. [Plugin Architecture](#plugin-architecture)
2. [API Reference](#api-reference)
3. [Creating Your First Plugin](#creating-your-first-plugin)
4. [Best Practices](#best-practices)
5. [Advanced Techniques](#advanced-techniques)
6. [Troubleshooting](#troubleshooting)

## Plugin Architecture

### Overview

The hello.c plugin system uses dynamic loading to allow runtime extension of functionality without modifying the core program. Plugins are shared libraries (.so files) that export specific functions following the plugin API.

### Loading Mechanism

1. Program starts and calls `get_greeting()` to get the base message
2. Checks `HELLO_PLUGIN` environment variable
3. If set, loads the specified shared library using `dlopen()`
4. Looks up the `plugin_transform` function using `dlsym()`
5. Calls the transform function with the original message
6. Outputs the transformed message

### Design Goals

- **Non-invasive**: No changes to core hello.c required for new plugins
- **Safe**: Graceful error handling if plugin fails to load
- **Simple**: Minimal API surface for easy plugin development
- **Flexible**: Plugins can do arbitrary transformations

## API Reference

### Data Types

#### PluginInfo
```c
typedef struct {
    const char* name;           /* Plugin name */
    const char* version;        /* Plugin version (e.g., "1.0.0") */
    const char* description;    /* Brief description */
    int api_version;            /* Must be PLUGIN_API_VERSION */
} PluginInfo;
```

### Required Functions

#### plugin_transform
```c
const char* plugin_transform(const char* message);
```
**Purpose**: Transform the input message

**Parameters**:
- `message`: Original greeting message (read-only, do not free)

**Returns**:
- Transformed message string
- NULL on error (original message will be used)

**Notes**:
- Return value can be static buffer or heap-allocated
- Must not modify input message
- Must be thread-safe if concurrent calls are possible

#### plugin_info
```c
PluginInfo plugin_info(void);
```
**Purpose**: Provide plugin metadata

**Returns**: PluginInfo struct with plugin details

**Notes**:
- Set `api_version` to `PLUGIN_API_VERSION` for compatibility
- Returned strings should be static/constant

### Optional Functions

#### plugin_init
```c
int plugin_init(void);
```
**Purpose**: Initialize plugin resources

**Returns**:
- 0 on success
- Non-zero on failure (plugin will not be used)

**Use Cases**:
- Allocate resources
- Initialize state
- Validate environment

#### plugin_cleanup
```c
void plugin_cleanup(void);
```
**Purpose**: Clean up plugin resources

**Notes**:
- Called before plugin is unloaded
- Free any allocated resources
- Current implementation may not call this (see note in hello.c)

## Creating Your First Plugin

### Step 1: Create Plugin Source File

Create `plugins/shout.c`:

```c
#include <stdio.h>
#include <string.h>
#include "../plugin.h"

static char buffer[256];

PluginInfo plugin_info(void) {
    PluginInfo info;
    info.name = "Shout Plugin";
    info.version = "1.0.0";
    info.description = "Adds exclamation marks for emphasis";
    info.api_version = PLUGIN_API_VERSION;
    return info;
}

const char* plugin_transform(const char* message) {
    if (!message) return NULL;
    
    size_t len = strlen(message);
    if (len + 3 >= sizeof(buffer)) {
        return message; /* Too long, return original */
    }
    
    snprintf(buffer, sizeof(buffer), "%s!!!", message);
    return buffer;
}
```

### Step 2: Build the Plugin

```bash
gcc -shared -fPIC -Wall -Wextra -o plugins/shout.so plugins/shout.c
```

### Step 3: Test the Plugin

```bash
HELLO_PLUGIN=./plugins/shout.so ./hello
```

Expected output:
```
Hello world!!!!
```

### Step 4: Add to Makefile (Optional)

Edit Makefile to add your plugin to the build:

```makefile
PLUGINS = $(PLUGIN_DIR)/uppercase.so $(PLUGIN_DIR)/reverse.so $(PLUGIN_DIR)/shout.so

$(PLUGIN_DIR)/shout.so: $(PLUGIN_DIR)/shout.c plugin.h
	$(CC) $(PLUGIN_CFLAGS) -o $@ $<
```

Now `make plugins` will build your plugin automatically.

## Best Practices

### Memory Management

1. **Use Static Buffers**: Simplest approach for most plugins
   ```c
   static char buffer[256];
   // Use buffer for transformation
   return buffer;
   ```

2. **Check Buffer Sizes**: Always validate before writing
   ```c
   if (len >= sizeof(buffer)) {
       return message; /* Or return NULL */
   }
   ```

3. **Avoid Memory Leaks**: If using malloc, track allocations
   ```c
   static char* allocated = NULL;
   free(allocated);
   allocated = malloc(new_size);
   return allocated;
   ```

### Error Handling

1. **Validate Input**:
   ```c
   if (!message || !*message) {
       return NULL; /* Or return default */
   }
   ```

2. **Handle Errors Gracefully**:
   ```c
   if (transformation_failed) {
       fprintf(stderr, "Plugin error: ...\n");
       return message; /* Return original on error */
   }
   ```

3. **Use plugin_init for Validation**:
   ```c
   int plugin_init(void) {
       if (!check_prerequisites()) {
           fprintf(stderr, "Plugin init failed\n");
           return 1;
       }
       return 0;
   }
   ```

### API Compatibility

Always set the correct API version:
```c
info.api_version = PLUGIN_API_VERSION;
```

This ensures your plugin is compatible with the loader.

### Thread Safety

If plugins might be called concurrently:

1. **Use Thread-Local Storage**:
   ```c
   __thread static char buffer[256];
   ```

2. **Use Mutexes for Shared State**:
   ```c
   #include <pthread.h>
   static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
   ```

## Advanced Techniques

### Stateful Plugins

Track state across invocations:

```c
#include <stdio.h>
#include "../plugin.h"

static int call_count = 0;
static char buffer[256];

int plugin_init(void) {
    call_count = 0;
    return 0;
}

const char* plugin_transform(const char* message) {
    call_count++;
    snprintf(buffer, sizeof(buffer), "[Call %d] %s", call_count, message);
    return buffer;
}
```

### Multi-Transform Plugins

Apply multiple transformations:

```c
static char buffer[256];

static void uppercase_transform(char* str) {
    for (char* p = str; *p; p++) {
        *p = toupper((unsigned char)*p);
    }
}

static void reverse_transform(char* str) {
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char tmp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = tmp;
    }
}

const char* plugin_transform(const char* message) {
    strncpy(buffer, message, sizeof(buffer) - 1);
    uppercase_transform(buffer);
    reverse_transform(buffer);
    return buffer;
}
```

### Configuration via Environment

Read configuration from environment variables:

```c
int plugin_init(void) {
    const char* config = getenv("PLUGIN_CONFIG");
    if (config) {
        /* Parse configuration */
    }
    return 0;
}
```

### Conditional Transformations

Apply transformations based on conditions:

```c
const char* plugin_transform(const char* message) {
    if (strstr(message, "world")) {
        /* Transform only if message contains "world" */
        return transformed_version;
    }
    return message; /* No transformation */
}
```

## Troubleshooting

### Plugin Not Loading

**Error**: "Failed to load plugin: cannot open shared object file"

**Solutions**:
- Check file path is correct (use absolute path or ./ prefix)
- Verify plugin file exists: `ls -l plugins/yourplugin.so`
- Check permissions: `chmod +x plugins/yourplugin.so`

### Missing Symbol Error

**Error**: "Plugin missing plugin_transform function"

**Solutions**:
- Ensure function is not static: `const char* plugin_transform(...)`
- Check function signature matches API
- Verify with: `nm -D plugins/yourplugin.so | grep plugin_transform`

### Crashes or Segfaults

**Common Causes**:
1. Buffer overflow - always check sizes
2. Returning pointer to stack memory - use static/heap
3. Null pointer dereference - check input

**Debugging**:
```bash
# Build with debug symbols
gcc -shared -fPIC -g -Wall -Wextra -o plugins/debug.so plugins/debug.c

# Run with gdb
gdb --args ./hello
(gdb) run
```

### Compilation Warnings

Build with strict flags to catch issues:
```bash
gcc -shared -fPIC -Wall -Wextra -Wpedantic -Werror -o plugin.so plugin.c
```

## Example Plugins

### Prefix Plugin
```c
#include "../plugin.h"
#include <stdio.h>
#include <string.h>

static char buffer[256];

PluginInfo plugin_info(void) {
    PluginInfo info = {
        .name = "Prefix Plugin",
        .version = "1.0.0",
        .description = "Adds a prefix to the message",
        .api_version = PLUGIN_API_VERSION
    };
    return info;
}

const char* plugin_transform(const char* message) {
    snprintf(buffer, sizeof(buffer), "Message: %s", message);
    return buffer;
}
```

### ROT13 Plugin
```c
#include "../plugin.h"
#include <ctype.h>
#include <string.h>

static char buffer[256];

static char rot13_char(char c) {
    if (c >= 'a' && c <= 'z') {
        return 'a' + (c - 'a' + 13) % 26;
    } else if (c >= 'A' && c <= 'Z') {
        return 'A' + (c - 'A' + 13) % 26;
    }
    return c;
}

PluginInfo plugin_info(void) {
    PluginInfo info = {
        .name = "ROT13 Plugin",
        .version = "1.0.0",
        .description = "Applies ROT13 cipher",
        .api_version = PLUGIN_API_VERSION
    };
    return info;
}

const char* plugin_transform(const char* message) {
    size_t len = strlen(message);
    if (len >= sizeof(buffer)) return message;
    
    for (size_t i = 0; i < len; i++) {
        buffer[i] = rot13_char(message[i]);
    }
    buffer[len] = '\0';
    return buffer;
}
```

## Further Reading

- **plugin.h**: Full API documentation in header comments
- **plugins/uppercase.c**: Simple transformation example
- **plugins/reverse.c**: String manipulation example
- **hello.c**: Reference implementation of plugin loading

## Support

For questions or issues with plugin development:
1. Check this guide for solutions
2. Review example plugins in `plugins/` directory
3. Verify plugin API version compatibility
4. Test with minimal plugin first
