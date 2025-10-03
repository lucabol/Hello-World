# Plugin System Architecture

## Overview

```
┌─────────────────────────────────────────────────────────────┐
│                         hello.c                             │
│                                                             │
│  1. Defines original message: "Hello world!"               │
│  2. Calls: apply_plugins(message)                          │
│  3. Prints: final transformed message                       │
└──────────────────┬──────────────────────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────────────────────┐
│                      plugin.c / plugin.h                    │
│                                                             │
│  Plugin Registry:                                           │
│  - Stores array of plugin pointers                          │
│  - register_plugin(): Adds plugins at startup               │
│  - apply_plugins(): Applies all plugins sequentially        │
└──────────────────┬──────────────────────────────────────────┘
                   │
                   │  Plugins auto-register at program startup
                   │  using __attribute__((constructor))
                   │
     ┌─────────────┼─────────────┬──────────────┐
     │             │             │              │
     ▼             ▼             ▼              ▼
┌─────────┐  ┌─────────┐  ┌─────────┐  ┌──────────────┐
│ Plugin  │  │ Plugin  │  │ Plugin  │  │   Custom     │
│   #1    │  │   #2    │  │   #3    │  │   Plugins    │
└─────────┘  └─────────┘  └─────────┘  └──────────────┘
  Uppercase     Prefix       ROT13      (user-defined)
```

## Message Flow

```
Original Message
      │
      ▼
  "Hello world!"
      │
      ▼
┌─────────────┐
│  Plugin #1  │  (if registered)
└──────┬──────┘
       ▼
  Transformed by Plugin #1
       │
       ▼
┌─────────────┐
│  Plugin #2  │  (if registered)
└──────┬──────┘
       ▼
  Transformed by Plugin #2
       │
       ▼
┌─────────────┐
│  Plugin #3  │  (if registered)
└──────┬──────┘
       ▼
  Final Message
       │
       ▼
   printf()
```

## Example: Chained Transformation

```
Input: "Hello world!"
  │
  ├─► [PrefixPlugin] ──► "*** Hello world!"
  │
  └─► [UppercasePlugin] ──► "*** HELLO WORLD!"
  
Output: "*** HELLO WORLD!"
```

## Plugin Interface

```c
typedef struct {
    char* (*process_message)(const char* input);
    const char* plugin_name;
    const char* plugin_version;
} plugin_interface;
```

## Key Design Principles

1. **Non-Invasive**: hello.c only knows about the plugin interface, not specific plugins
2. **Extensible**: New plugins can be added without modifying existing code
3. **Sequential Processing**: Plugins form a processing pipeline
4. **Static Linking**: All plugins compiled into the final binary
5. **Auto-Registration**: Plugins register themselves at program startup

## Compilation Models

### Without Plugins
```
hello.c + plugin.c → hello (outputs "Hello world!")
```

### With One Plugin
```
hello.c + plugin.c + example_plugin.c → hello (outputs "HELLO WORLD!")
```

### With Multiple Plugins
```
hello.c + plugin.c + prefix_plugin.c + example_plugin.c 
    → hello (outputs "*** HELLO WORLD!")
```

## Directory Structure

```
Hello-World/
├── hello.c                 # Main program (9 lines)
├── plugin.h                # Plugin interface (86 lines)
├── plugin.c                # Plugin system (49 lines)
├── plugins/                # Plugin implementations
│   ├── example_plugin.c   # Uppercase transformer (45 lines)
│   ├── prefix_plugin.c    # Prefix transformer (33 lines)
│   └── rot13_plugin.c     # ROT13 cipher (60 lines)
├── PLUGIN_GUIDE.md        # Complete plugin development guide
└── README.md              # Project documentation
```

## Benefits

✓ **Modularity**: Each plugin is independent and self-contained
✓ **Flexibility**: Enable/disable plugins at compile time
✓ **Simplicity**: Easy to understand and extend
✓ **Type Safety**: Strong typing via C interface
✓ **No Runtime Overhead**: Static linking, no dynamic loading
✓ **Composability**: Plugins can be chained in any order
