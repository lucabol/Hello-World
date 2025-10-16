# Hello World
Test repo for JediMaster

## Plugin System

This repository now includes a plugin architecture that allows external developers to extend hello.c functionality without modifying the core file. See [PLUGIN_SYSTEM.md](PLUGIN_SYSTEM.md) for complete documentation.

### Quick Example

```bash
# Default behavior
gcc -o hello hello.c
./hello
# Output: Hello world!

# With uppercase plugin
gcc -o hello hello_plugin_enabled.c plugin.c plugins/example_uppercase.c -DLOAD_UPPERCASE
./hello
# Output: HELLO WORLD!
```
