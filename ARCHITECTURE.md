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

## Portability Constraints

**Supported Compilers:**
- GCC 4.7 or later
- Clang 3.3 or later

**Not Supported:**
- Microsoft Visual C++ (MSVC)
- Compilers without `__attribute__((constructor))` support

The plugin system uses GCC/Clang-specific constructor attributes for automatic registration. The system includes compile-time checks to prevent compilation on unsupported toolchains.

**Why Constructor Attributes:**
- Automatic registration before main()
- No manual initialization code needed
- Plugins self-register at program startup
- Deterministic ordering based on link order

## Memory Management

**No Dynamic Allocation:**
- Plugins use static/global storage
- No malloc/free required
- No memory leaks possible
- Simple and predictable

**Ownership Model:**
- Plugins own their output buffers
- Caller (apply_plugins) does NOT take ownership
- Returned pointers must remain valid for program lifetime
- Thread-unsafe by default (use thread-local storage for concurrency)

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
├── plugin.h                # Plugin interface (extensively documented)
├── plugin.c                # Plugin system (robust error handling)
├── plugins/                # Plugin implementations
│   ├── example_plugin.c   # Uppercase transformer
│   ├── prefix_plugin.c    # Prefix transformer
│   ├── rot13_plugin.c     # ROT13 cipher
│   └── null_test_plugin.c # Conditional transformation example
├── test/
│   └── test_plugins.sh    # Comprehensive tests with ASAN
├── PLUGIN_GUIDE.md        # Complete plugin development guide
├── ARCHITECTURE.md        # This file
└── README.md              # Project documentation
```

## Benefits

✓ **Modularity**: Each plugin is independent and self-contained
✓ **Flexibility**: Enable/disable plugins at compile time
✓ **Simplicity**: Easy to understand and extend
✓ **Type Safety**: Strong typing via C interface
✓ **No Runtime Overhead**: Static linking, no dynamic loading
✓ **Composability**: Plugins can be chained in any order
✓ **Memory Safe**: No dynamic allocation, no leaks (verified with ASAN)
✓ **Error Resilient**: Graceful handling of NULL returns
