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

static char myname_buffer[PLUGIN_BUFFER_SIZE];

const char* myname_transform(const char* input) {
    // Add your name to the message
    snprintf(myname_buffer, PLUGIN_BUFFER_SIZE, "%s - by YourName", input);
    return myname_buffer;
}

const plugin_t myname_plugin = {
    .name = "myname",
    .transform = myname_transform
};
```

### Step 2: Use your plugin

Create `my_app.c`:

```c
#include <stdio.h>
#include "plugin.h"

extern const plugin_t myname_plugin;

int main(void) {
    plugin_init();
    plugin_register(&myname_plugin);
    
    const char* result = plugin_apply_all("Hello world!");
    printf("%s\n", result);
    
    return 0;
}
```

### Step 3: Build and run

```bash
gcc -o my_app my_app.c plugin.c plugins/myname.c
./my_app
```

Output: `Hello world! - by YourName`

## Testing Your Plugin

Add a test to `test/test_plugin.c`:

```c
void test_myname_plugin(void) {
    const char* input = "Hello world!";
    const char* expected = "Hello world! - by YourName";
    const char* result;
    
    plugin_init();
    plugin_register(&myname_plugin);
    result = plugin_apply_all(input);
    
    TEST_ASSERT_EQUAL_STRING(expected, result);
}
```

Then run: `./test/run_plugin_tests.sh`

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
