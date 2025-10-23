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
- **logger**: Logs execution timestamps using only hooks

### Documentation

For complete plugin development documentation, see [PLUGIN_GUIDE.md](PLUGIN_GUIDE.md)

### Implementation Files

**Core Plugin System:**
- [`plugin.h`](plugin.h) - Plugin API, error codes, macros, and comprehensive inline documentation
- [`plugin.c`](plugin.c) - Plugin registry with double-buffering implementation for safe chaining
- [`hello.c`](hello.c) - Modified to support plugins (12 lines added within `#ifdef USE_PLUGINS`)

**Example Plugins:**
- [`plugin_uppercase.c`](plugin_uppercase.c) - Transforms message to uppercase
- [`plugin_decorator.c`](plugin_decorator.c) - Adds decorative borders with before/after hooks
- [`plugin_logger.c`](plugin_logger.c) - Logs execution timestamps using only hooks
- [`plugin_repeat_example.c`](plugin_repeat_example.c) - Template/example for creating custom plugins

**Build & Test:**
- [`Makefile`](Makefile) - Build targets for various plugin combinations
- [`test_plugins.sh`](test_plugins.sh) - Comprehensive test suite (13 tests)
- [`.github/workflows/ci.yml`](.github/workflows/ci.yml) - CI pipeline with GCC and Clang

**Additional Documentation:**
- [`PLUGIN_SUMMARY.md`](PLUGIN_SUMMARY.md) - Complete implementation summary
- [`CHANGELOG.md`](CHANGELOG.md) - Version history and maintainer guidelines

### Building

```bash
make              # Standard hello (no plugins)
make with-plugins # All example plugins
make uppercase    # Uppercase plugin only
make decorator    # Decorator plugin only
make logger       # Logger plugin only
make clean        # Clean build artifacts
```

### Backward Compatibility

The plugin system is completely optional. When built without plugins (the default), hello.c behaves exactly as the original simple program.