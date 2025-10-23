# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

### Added - Plugin System (2025-10-23)

#### Overview
Introduced a plugin architecture for hello.c that allows external developers to extend functionality without modifying the core file.

#### Features
- **Transform API**: Plugins can transform the output message using a safe out-buffer model
  - Function signature: `int transform(const char* input, char* output, size_t output_size)`
  - Returns 0 on success, non-zero on error
  - Buffer overflow protection built-in
- **Hooks**: Plugins can execute code before and after the message is printed
- **Plugin Chaining**: Multiple plugins work together seamlessly with double-buffering
- **Compile-time Linking**: Plugins are statically linked (no dynamic loading complexity)
- **Backward Compatible**: Core hello.c unchanged when built without `-DUSE_PLUGINS` flag

#### API
- `plugin_register()` - Register a plugin (explicit or via macro)
- `PLUGIN_REGISTER()` - Convenience macro for automatic registration (GCC/Clang only)
- `plugin_execute_transforms()` - Execute all transform functions with safe chaining
- `plugin_execute_before_hooks()` - Execute all before hooks
- `plugin_execute_after_hooks()` - Execute all after hooks
- `plugin_get_count()` - Get number of registered plugins (useful for testing)

#### Files Added
- `plugin.h` - Plugin system API and documentation
- `plugin.c` - Plugin registry and execution engine
- `plugin_uppercase.c` - Example: uppercase transformer
- `plugin_decorator.c` - Example: decorator with hooks
- `plugin_logger.c` - Example: hooks-only logging
- `plugin_repeat_example.c` - Template for custom plugins
- `Makefile` - Build system with plugin targets
- `PLUGIN_GUIDE.md` - Comprehensive developer documentation
- `PLUGIN_SUMMARY.md` - Implementation summary
- `test_plugins.sh` - Automated test suite
- `CHANGELOG.md` - This file

#### Files Modified
- `hello.c` - Added plugin hooks (conditional on USE_PLUGINS)
- `README.md` - Added plugin system overview
- `.gitignore` - Added plugin binaries

#### Compilation
```bash
# Without plugins (default)
make

# With plugins
make with-plugins
make uppercase
make decorator
make logger

# Custom compilation
gcc -DUSE_PLUGINS -o hello hello.c plugin.c my_plugin.c
```

#### Configuration
- `MAX_PLUGINS` - Maximum plugins (default 10, override with `-DMAX_PLUGINS=N`)
- `PLUGIN_BUFFER_SIZE` - Transform buffer size (default 1024, override with `-DPLUGIN_BUFFER_SIZE=N`)

#### Limitations and Portability
- **Compiler Support**: Requires GCC or Clang for automatic registration (constructor attribute)
  - MSVC and other compilers require explicit `plugin_register()` calls
- **Thread Safety**: Not thread-safe - registration must complete before execution
- **Registration Order**: Depends on link order, not guaranteed across translation units
- **Transform Requirements**: Must write to provided output buffer, cannot use static returns
- **Buffer Limits**: Input and transform output limited to PLUGIN_BUFFER_SIZE

#### Testing
- 10 comprehensive tests covering:
  - Core functionality without plugins
  - Individual plugin transformations
  - Multi-plugin chaining (2 and 3 plugins)
  - Hooks-only plugins
  - Hooks combined with transforms
  - Strict compilation (-Werror, -std=c99)
  - Buffer safety in chaining
- Run tests: `./test_plugins.sh`
- All tests pass on GCC and Clang

#### Security
- Buffer overflow protection in transform API
- Explicit buffer size passing
- Truncation warnings if output exceeds buffer
- Error handling for transform failures
- Validated with strict compilation flags

#### Documentation
- `PLUGIN_GUIDE.md` - Complete developer guide (7KB)
  - API reference with examples
  - Creating custom plugins
  - Compilation instructions
  - Best practices
  - Limitations and troubleshooting
- `PLUGIN_SUMMARY.md` - Implementation overview (5KB)
- Inline code documentation in all headers
- Example plugins with detailed comments

#### Maintainer Notes

**Reviewing Plugin Contributions:**
1. Verify plugin uses new out-buffer API: `int transform(const char*, char*, size_t)`
2. Check for buffer overflow handling (must return error if output too large)
3. Ensure plugin compiles with strict flags: `make strict-plugins`
4. Test plugin independently and in combination with existing plugins
5. Verify documentation updates if plugin adds new patterns
6. Confirm tests pass: `./test_plugins.sh`

**Adding New Plugins:**
- Use `plugin_repeat_example.c` as template
- Test buffer handling with long inputs
- Document any dependencies or requirements
- Update PLUGIN_GUIDE.md with usage example
- Add to Makefile if it's an official example

**Breaking Changes:**
This release introduces a new transform API that is **incompatible** with any external plugins written against an earlier draft version (if any existed). The old API returned `const char*` to static buffers; the new API uses out-buffers for safety.

**Migration Guide** (if upgrading from earlier draft):
- Old: `const char* my_transform(const char* input)`
- New: `int my_transform(const char* input, char* output, size_t output_size)`
- Change: Write to `output` buffer, return 0 on success, -1 on error
- Remove static buffers from plugin code
- Check return value of `snprintf` to detect buffer overflow

#### References
- Issue: #1752 - Create a plugin system for extending hello.c functionality
- PR Review: Comprehensive security and API safety review by @lucabol
