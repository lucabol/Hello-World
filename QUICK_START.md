# Quick Start Guide - Plugin System

## For Users: Running the Demo

```bash
# Build the demo
gcc -o plugin_demo plugin_demo.c plugin.c plugins/uppercase.c plugins/reverse.c plugins/prefix.c

# Run it
./plugin_demo
```

You'll see examples of:
- No plugins (original message)
- Individual plugins (uppercase, reverse, prefix)
- Plugin chaining (combining multiple plugins)

## For Developers: Creating Your First Plugin

### Step 1: Create your plugin file

Create `plugins/myname.c`:

```c
#include "../plugin.h"
#include <string.h>
#include <stdio.h>

static char myname_buffer[PLUGIN_BUFFER_SIZE];

const char* myname_transform(const char* input) {
    size_t input_len, total_len;
    const char* suffix = " - by YourName";
    size_t suffix_len;
    
    /* ALWAYS validate input */
    if (input == NULL) {
        return NULL;
    }
    
    input_len = strlen(input);
    suffix_len = strlen(suffix);
    total_len = input_len + suffix_len;
    
    /* ALWAYS check buffer size */
    if (total_len >= PLUGIN_BUFFER_SIZE) {
        return NULL;  /* Not enough space */
    }
    
    /* Safe transformation */
    snprintf(myname_buffer, PLUGIN_BUFFER_SIZE, "%s%s", input, suffix);
    
    return myname_buffer;
}

const plugin_t myname_plugin = {
    .name = "myname",
    .transform = myname_transform
};
```

**Key Points:**
- ✅ Always check if input is NULL
- ✅ Always validate buffer sizes before writing
- ✅ Return NULL on error
- ✅ Use snprintf() to prevent buffer overflows

### Step 2: Use your plugin

Create `my_app.c`:

```c
#include <stdio.h>
#include "plugin.h"

extern const plugin_t myname_plugin;

int main(void) {
    int result;
    const char* output;
    
    /* Initialize */
    plugin_init();
    
    /* Register with error checking */
    result = plugin_register(&myname_plugin);
    if (result != PLUGIN_SUCCESS) {
        fprintf(stderr, "Failed to register plugin: error code %d\n", result);
        return 1;
    }
    
    /* Apply with error checking */
    output = plugin_apply_all("Hello world!");
    if (output == NULL) {
        fprintf(stderr, "Plugin transformation failed\n");
        return 1;
    }
    
    printf("%s\n", output);
    
    return 0;
}
```

**Key Points:**
- ✅ Always check return value of plugin_register()
- ✅ Always check if plugin_apply_all() returns NULL
- ✅ Handle errors gracefully

### Step 3: Build and run

```bash
gcc -Wall -Wextra -o my_app my_app.c plugin.c plugins/myname.c
./my_app
```

Output: `Hello world! - by YourName`

## Testing Your Plugin

Add a test to `test/test_plugin.c`:

```c
extern const plugin_t myname_plugin;

void test_myname_plugin(void) {
    const char* input = "Hello world!";
    const char* expected = "Hello world! - by YourName";
    const char* result;
    
    plugin_init();
    
    /* Test registration */
    int reg_result = plugin_register(&myname_plugin);
    if (reg_result != PLUGIN_SUCCESS) {
        tests_failed++;
        print_red("  ✗ ");
        printf("Plugin registration failed\n");
        return;
    }
    
    /* Test transformation */
    result = plugin_apply_all(input);
    
    if (result == NULL) {
        tests_failed++;
        print_red("  ✗ ");
        printf("Plugin returned NULL\n");
        return;
    }
    
    TEST_ASSERT_EQUAL_STRING(expected, result);
}
```

Then run: `./test/run_plugin_tests.sh`

## Common Errors and Solutions

### Error: "Failed to register plugin: error code -2"

**Problem:** NULL plugin descriptor or NULL name/transform

**Solution:** Ensure your plugin descriptor is properly initialized:
```c
const plugin_t my_plugin = {
    .name = "my_plugin",        // Must not be NULL
    .transform = my_transform   // Must not be NULL
};
```

### Error: "Failed to register plugin: error code -3"

**Problem:** Plugin already registered (duplicate)

**Solution:** Each plugin can only be registered once. Call `plugin_init()` to reset if needed.

### Error: "Plugin transformation failed" (NULL return)

**Problem:** Plugin returned NULL or result too large

**Possible causes:**
1. Plugin received NULL input and returned NULL
2. Transformation result exceeds PLUGIN_BUFFER_SIZE - 1 bytes
3. Plugin encountered an error and returned NULL

**Solution:** 
- Check your plugin validates input
- Ensure output fits in buffer
- Add error handling in your plugin

### Build Error: "implicit declaration of function 'snprintf'"

**Problem:** Missing include

**Solution:** Add `#include <stdio.h>` to your plugin file

## Error Codes Reference

```c
PLUGIN_SUCCESS (0)          // Operation succeeded
PLUGIN_ERROR_FULL (-1)      // Registry full (max 10 plugins)
PLUGIN_ERROR_NULL (-2)      // NULL plugin or invalid descriptor
PLUGIN_ERROR_DUPLICATE (-3) // Plugin already registered
```

## Thread Safety Warning

⚠️ **The plugin system is NOT thread-safe**

Do not:
- Call plugin functions from multiple threads concurrently
- Use plugins in signal handlers
- Call plugins reentrantly

For multi-threaded applications:
- Use a mutex to serialize access
- Or create separate plugin instances per thread

## Combining Plugins

Plugins are applied in the order registered:

```c
plugin_init();
plugin_register(&uppercase_plugin);  // First: HELLO WORLD!
plugin_register(&myname_plugin);     // Second: HELLO WORLD! - by YourName
result = plugin_apply_all("Hello world!");
```

## Important Rules

1. ✓ Use buffers <= 256 bytes (PLUGIN_BUFFER_SIZE)
2. ✓ Always null-terminate your strings
3. ✓ Check buffer sizes before writing
4. ✓ Return a valid pointer (usually your static buffer)
5. ✓ Don't modify the input string

## More Information

- Full documentation: See `PLUGIN_GUIDE.md`
- Implementation details: See `IMPLEMENTATION_SUMMARY.md`
- Plugin examples: See `plugins/` directory
