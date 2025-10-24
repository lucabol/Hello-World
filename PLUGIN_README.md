# Plugin System Quick Start

## What is the Plugin System?

The plugin system allows external developers to extend hello.c functionality without modifying the core file. Plugins can transform the greeting message in any way - uppercase, reverse, add prefixes, etc.

## Quick Examples

### Build and Run Basic Hello
```bash
make
./hello
# Output: Hello world!
```

### See Plugin System in Action
```bash
make demo
```

This will show:
- How plugins transform messages
- How plugins can be chained
- Multiple transformation examples

### Build Hello with Plugins
```bash
make hello-plugins
./hello_with_plugins
```

## Creating Your Own Plugin

1. Create a transform function:
```c
static int my_transform(const char* input, char* output, size_t output_size) {
    // Your transformation logic here
    strcpy(output, input);
    strcat(output, " - Modified!");
    return 0;
}
```

2. Define the plugin:
```c
plugin_t my_plugin = {
    .name = "my_plugin",
    .transform = my_transform
};
```

3. Use it:
```c
register_plugin(&my_plugin);
apply_plugins("Hello world!", output, sizeof(output));
```

## Files

- `plugin.h` - Plugin interface
- `plugin.c` - Plugin system implementation
- `plugins_examples.c` - Example plugins (uppercase, reverse, exclaim, prefix)
- `plugin_demo.c` - Demonstration program
- `hello_with_plugins.c` - Integration example
- `PLUGIN_SYSTEM.md` - Complete documentation

## Make Targets

- `make` - Build hello
- `make demo` - Build and run plugin demonstrations
- `make hello-plugins` - Build hello with example plugins
- `make clean` - Remove all built files
- `make test` - Run validation tests

## Learn More

See `PLUGIN_SYSTEM.md` for complete documentation including:
- Architecture overview
- API reference
- Creating custom plugins
- Plugin chaining
- Advanced use cases
