# Hello World
Test repo for JediMaster

A simple C "Hello World" program that demonstrates basic C programming concepts and includes an extensible plugin system.

## Features

- **Standard Mode**: Classic "Hello world!" output with no dependencies
- **Plugin System**: Optional extensible architecture for adding custom functionality
- **Backward Compatible**: All existing behavior preserved

## Plugin System

The program now supports an optional plugin system that allows external developers to extend functionality without modifying the core code. See [PLUGIN_SYSTEM.md](PLUGIN_SYSTEM.md) for detailed documentation.

### Quick Plugin Demo
```bash
make plugins      # Build sample plugins
make test-plugins # Run with plugins loaded
```