# Hello World

Test repo for JediMaster

## Plugin System

This Hello World program includes a plugin architecture that allows external developers to extend functionality without modifying the core `hello.c` file.

See [PLUGIN_SYSTEM.md](PLUGIN_SYSTEM.md) for complete documentation.

### Quick Start with Plugins

```bash
# Build the plugin-enabled version and example plugins
make hello-plugin plugins

# Run with a plugin
HELLO_PLUGINS=./plugins/uppercase_plugin.so ./hello-plugin
```