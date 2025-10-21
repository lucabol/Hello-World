# Plugin System Quick Start

## 30-Second Overview

The hello.c program now supports plugins that can transform the greeting message.

## Compiler Support

- **GCC/Clang**: Automatic plugin registration ✓
- **MSVC**: Manual registration required (use `INIT_PLUGIN()`)

## Create a Plugin (60 seconds)

```c
/* my_plugin.h */
#ifndef MY_PLUGIN_H
#define MY_PLUGIN_H

#include "plugin.h"

/* Use PLUGIN_BUFFER macro for unique buffer names */
static char PLUGIN_BUFFER(myplugin)[256];

static const char* my_transform(const char* msg) {
    /* Transform msg and store in buffer */
    /* Always bounds-check and null-terminate! */
    int i = 0;
    while (msg[i] != '\0' && i < 255) {
        PLUGIN_BUFFER(myplugin)[i] = msg[i];
        i++;
    }
    PLUGIN_BUFFER(myplugin)[i] = '\0';
    return PLUGIN_BUFFER(myplugin);
}

DEFINE_PLUGIN(my_transform)
#endif
```

## Use Your Plugin (30 seconds)

```c
#include <stdio.h>
#include "hello.h"
#include "plugin.h"
#include "my_plugin.h"  /* Include ONCE in only ONE .c file */

int main() {
    const char* greeting = get_greeting();
    const char* result = apply_plugins(greeting);
    printf("%s", result);
    return 0;
}
```

## Build and Run (10 seconds)

```bash
# Use -I. for include paths
gcc -I. -o my_program my_program.c hello_lib.c plugin.c
./my_program
```

## Try the Examples

```bash
# Run the demo
gcc -I. -o demo examples/demo.c hello_lib.c plugin.c
./demo

# Output: HELLO WORLD!!!
```

## Important Guidelines

✅ **Use `PLUGIN_BUFFER(uniquename)`** for buffer names to avoid collisions  
✅ **Include plugin headers in ONLY ONE .c file** to prevent duplicates  
✅ **Always null-terminate** your output strings  
✅ **Bounds-check** all string operations (prevent overflows)  
✅ **Use `-I.`** when compiling to find headers  

## Learn More

- [Full Plugin Guide](PLUGIN_GUIDE.md) - Complete documentation with memory/thread-safety details
- [Plugin Examples](plugins/) - Working examples
- [Demo Programs](examples/) - See plugins in action

## Key Points

✅ Plugins are header files - easy to create and share  
✅ Multiple plugins can be chained together  
✅ No modifications to hello.c needed  
✅ Compile-time plugin registration (no runtime overhead)  
✅ Configurable MAX_PLUGINS (default 10, use -DMAX_PLUGINS=N)  
✅ GCC/Clang auto-register, MSVC needs manual `INIT_PLUGIN()`  

## MSVC Example

```c
#include "plugin.h"

/* Declare plugin function */
const char* my_transform(const char*);

int main() {
    /* Manually register on MSVC */
    INIT_PLUGIN(my_transform);
    
    /* Use plugins normally */
    printf("%s", apply_plugins(get_greeting()));
}
```

## Need Help?

See the [Plugin Guide](PLUGIN_GUIDE.md) or check the [examples](examples/) directory.
