# Hello World
Test repo for JediMaster

## Plugin System

This repository now includes a plugin architecture that allows external developers to extend hello.c functionality without modifying the core file.

### Quick Start

```bash
# Build and run basic hello
make
./hello

# See plugin system demonstrations
make demo
```

### Documentation

- **[PLUGIN_README.md](PLUGIN_README.md)** - Quick start guide
- **[PLUGIN_SYSTEM.md](PLUGIN_SYSTEM.md)** - Complete documentation
- **[IMPLEMENTATION_SUMMARY.md](IMPLEMENTATION_SUMMARY.md)** - Implementation details

### Features

- ✅ Zero-modification extensibility
- ✅ Plugin chaining support
- ✅ Type-safe callback-based API
- ✅ No external dependencies
- ✅ Production-ready with full error handling
- ✅ Comprehensive examples and documentation

### Example Plugins Included

- **Uppercase** - Converts to uppercase
- **Reverse** - Reverses the message
- **Exclaim** - Adds extra exclamation marks
- **Prefix** - Adds a prefix tag
- **ROT13** - Caesar cipher transformation
- **Leetspeak** - Letter-to-number conversion

See the documentation for how to create your own plugins!