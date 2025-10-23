# Hello World
Test repo for JediMaster

## Plugin System

This repository includes a plugin architecture that allows external developers to extend hello.c functionality without modifying the core file.

### Quick Start

```bash
# Build standard hello (no plugins)
make

# Build with all example plugins
make with-plugins

# Run the plugin-enabled version
./hello_with_plugins
```

### What is the Plugin System?

The plugin system enables you to:
- Transform the output message (e.g., uppercase, add decorations)
- Execute code before the message is printed (before hooks)
- Execute code after the message is printed (after hooks)
- Chain multiple plugins together

### Example Plugins

Two example plugins are included:
- **uppercase**: Converts the message to uppercase
- **decorator**: Adds decorative borders and headers/footers

### Documentation

For complete plugin development documentation, see [PLUGIN_GUIDE.md](PLUGIN_GUIDE.md)

### Building

```bash
make              # Standard hello (no plugins)
make with-plugins # All example plugins
make uppercase    # Uppercase plugin only
make decorator    # Decorator plugin only
make clean        # Clean build artifacts
```

### Backward Compatibility

The plugin system is completely optional. When built without plugins (the default), hello.c behaves exactly as the original simple program.