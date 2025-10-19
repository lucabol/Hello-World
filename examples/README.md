# Plugin System Examples

This directory contains working examples demonstrating the plugin system.

## demo.c

A comprehensive demonstration of the plugin system showing:
- Base greeting without plugins
- Transformation with multiple plugins
- Order of plugin application

### Building and Running

```bash
gcc -Wall -Wextra -o examples/demo examples/demo.c hello_lib.c plugin.c
./examples/demo
```

### Expected Output

```
=== Hello World Plugin System Demo ===

1. Base greeting: Hello world!
2. With plugins:  HELLO WORLD!!!

Plugins applied (in order):
  - exclamation_plugin: adds extra '!!' 
  - uppercase_plugin: converts to uppercase

Result: exclamation first, then uppercase
  'Hello world!' -> 'Hello world!!!' -> 'HELLO WORLD!!!'
```

## Creating Your Own Example

1. Create a new .c file in this directory
2. Include the core headers: `hello.h`, `plugin.h`
3. Include the plugin headers you want to use
4. Build with: `gcc -o example example.c hello_lib.c plugin.c`

See [Plugin Guide](../PLUGIN_GUIDE.md) for more details.
