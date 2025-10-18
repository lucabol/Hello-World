# Hello World

Test repo for JediMaster

## Plugin System

This repository now includes a plugin system that allows external developers to extend the functionality of hello.c without modifying the core file. 

### Quick Start

```bash
# Build and run plugin demonstration
make plugin-demo
./hello_plugin_demo
```

### Documentation

- **[PLUGIN_SYSTEM.md](PLUGIN_SYSTEM.md)** - Complete plugin system documentation
- **[plugins/README.md](plugins/README.md)** - Plugin development guide

### Example Plugins

- **uppercase** - Converts greeting to uppercase
- **exclaim** - Adds extra exclamation marks

For detailed information, see [PLUGIN_SYSTEM.md](PLUGIN_SYSTEM.md)