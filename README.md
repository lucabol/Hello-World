# Hello World

Test repo for JediMaster

## Features

- Simple "Hello world!" program in C
- Optional plugin system for extending functionality
- Backward compatible - works exactly as before when plugins are not used

## Quick Start

```bash
# Build and run standard version
make hello && ./hello

# Build and test plugin system
make test-plugins
```

## Plugin System

This repository now includes an optional plugin architecture that allows external developers to extend functionality without modifying the core hello.c file. See [PLUGINS.md](PLUGINS.md) for complete documentation.

### Example Usage

```bash
# Standard behavior
./hello-plugins
# Output: Hello world!

# With uppercase plugin
HELLO_PLUGIN=./plugins/uppercase.so ./hello-plugins  
# Output: HELLO WORLD!

# With exclamation plugin
HELLO_PLUGIN=./plugins/exclamation.so ./hello-plugins
# Output: Hello world!!!!
```