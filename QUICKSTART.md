# Plugin System Quick Start

## 30-Second Overview

The hello.c program now supports plugins that can transform the greeting message.

## Create a Plugin (60 seconds)

```c
/* my_plugin.h */
#ifndef MY_PLUGIN_H
#define MY_PLUGIN_H

#include "plugin.h"

static char my_buffer[256];

static const char* my_transform(const char* msg) {
    /* Transform msg and store in my_buffer */
    return my_buffer;
}

DEFINE_PLUGIN(my_transform)
#endif
```

## Use Your Plugin (30 seconds)

```c
#include <stdio.h>
#include "hello.h"
#include "plugin.h"
#include "my_plugin.h"  /* Include your plugin */

int main() {
    const char* greeting = get_greeting();
    const char* result = apply_plugins(greeting);
    printf("%s", result);
    return 0;
}
```

## Build and Run (10 seconds)

```bash
gcc -o my_program my_program.c hello_lib.c plugin.c
./my_program
```

## Try the Examples

```bash
# Run the demo
gcc -o demo examples/demo.c hello_lib.c plugin.c
./demo

# Output: HELLO WORLD!!!
```

## Learn More

- [Full Plugin Guide](PLUGIN_GUIDE.md) - Complete documentation
- [Plugin Examples](plugins/) - Working examples
- [Demo Programs](examples/) - See plugins in action

## Key Points

✅ Plugins are header files - easy to create and share  
✅ Multiple plugins can be chained together  
✅ No modifications to hello.c needed  
✅ Compile-time plugin registration (no runtime overhead)  

## Need Help?

See the [Plugin Guide](PLUGIN_GUIDE.md) or check the [examples](examples/) directory.
