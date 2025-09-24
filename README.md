# Hello World

Test repo for JediMaster with extensible plugin system.

## Features

- Simple C program that outputs "Hello world!"
- Extensible plugin architecture for message transformations
- Example plugins for uppercase conversion and adding exclamation marks

## Basic Usage

```bash
make && ./hello
# Output: Hello world!
```

## Plugin System

The program supports loading plugins from the `plugins/` directory that can transform the output message.

### Build and Test with Example Plugins

```bash
# Build with example plugins
make plugins

# Run with plugins loaded
./hello
# Output: HELLO WORLD!!!

# Run tests (removes plugins first)
rm plugins/*.so && make test
```

### Available Plugins

- **uppercase.so**: Converts message to uppercase
- **exclamation.so**: Adds extra exclamation marks

## Development

See [PLUGIN_DEVELOPMENT.md](PLUGIN_DEVELOPMENT.md) for detailed plugin development guide.

```bash
# Build everything
make all plugins

# Run validation tests
make test

# Clean build artifacts
make clean
```