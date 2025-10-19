# Hello World

Test repo for JediMaster

## Plugin System

This repository now includes a plugin architecture that allows extending the hello.c functionality without modifying the core file.

### Quick Start

**Build and run without plugins:**
```bash
gcc -o hello hello.c plugin.c
./hello
# Output: Hello world!
```

**Build and run with plugins:**
```bash
gcc -o demo examples/demo.c hello_lib.c plugin.c
./demo
# Output: HELLO WORLD!!!
```

### Documentation

- [Plugin Guide](PLUGIN_GUIDE.md) - Complete guide to creating and using plugins
- [Plugin Examples](plugins/README.md) - Example plugins included in this repo
- [Demo Programs](examples/README.md) - Working examples demonstrating the plugin system

### Features

- ✅ Simple plugin interface
- ✅ Header-only plugin support
- ✅ Multiple plugins can be chained
- ✅ No modifications needed to core hello.c
- ✅ Compile-time plugin registration
- ✅ Backward compatible with existing code

### Available Plugins

- **uppercase_plugin.h** - Converts greeting to uppercase
- **exclamation_plugin.h** - Adds extra exclamation marks

### Testing

```bash
# Run unit tests
./test/run_unit_tests.sh

# Run validation
./test/validate.sh
```