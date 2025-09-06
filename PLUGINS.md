# Plugin System for hello.c

This document explains how to create and use plugins to extend the functionality of hello.c without modifying the core file.

## Plugin Interface

Plugins are defined using the `plugin_t` structure in `plugin.h`:

```c
typedef struct {
    const char* name;                              // Plugin name
    char* (*transform_message)(const char* input); // Transform output message
    void (*pre_main_hook)(void);                   // Execute before main logic
    void (*post_main_hook)(void);                  // Execute after main logic
} plugin_t;
```

## Creating a Plugin

1. Create a `.c` file in the `plugins/` directory
2. Include the plugin header: `#include "../plugin.h"`
3. Implement your plugin functions
4. Create a `plugin_t` structure with your functions
5. Use `__attribute__((constructor))` to auto-register your plugin

### Example Plugin

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../plugin.h"

static char* my_transform(const char* input) {
    // Transform the message
    char* result = malloc(strlen(input) + 10);
    sprintf(result, ">> %s <<", input);
    return result;
}

static void my_pre_hook(void) {
    printf("Plugin starting...\n");
}

static plugin_t my_plugin = {
    .name = "My Plugin",
    .transform_message = my_transform,
    .pre_main_hook = my_pre_hook,
    .post_main_hook = NULL
};

__attribute__((constructor))
static void register_my_plugin(void) {
    register_plugin(&my_plugin);
}
```

## Compilation

### Without plugins (original behavior):
```bash
gcc -Wall -Wextra -o hello hello.c
```

### With plugins:
```bash
gcc -Wall -Wextra -o hello_with_plugins hello.c plugins/plugin1.c plugins/plugin2.c
```

### Examples:
```bash
# With excitement plugin
gcc -Wall -Wextra -o hello_excited hello.c plugins/excitement.c

# With uppercase plugin  
gcc -Wall -Wextra -o hello_upper hello.c plugins/uppercase.c

# With both plugins
gcc -Wall -Wextra -o hello_both hello.c plugins/excitement.c plugins/uppercase.c
```

## Plugin Types

### Message Transformers
Use `transform_message` to modify the output text. The function should:
- Take a `const char*` input message
- Return a `char*` result (must be malloc'd)
- Handle memory management properly

### Pre-execution Hooks
Use `pre_main_hook` to execute code before the main message is processed.

### Post-execution Hooks  
Use `post_main_hook` to execute code after the main message is displayed.

## Notes

- Plugins are executed in registration order
- Message transformations are chained (output of one becomes input of next)
- Memory allocated by `transform_message` should use `malloc()` 
- The plugin system handles freeing transformed messages
- Maximum of 10 plugins can be registered simultaneously
- All hook functions are optional (can be NULL)