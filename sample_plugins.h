#ifndef SAMPLE_PLUGINS_H
#define SAMPLE_PLUGINS_H

/**
 * Sample plugins for demonstration of the plugin system
 * 
 * These plugins demonstrate different types of plugins:
 * - Output transformers that modify the program output
 * - Pre-processors that run before main functionality
 * - Post-processors that run after main functionality
 */

/**
 * Register all sample plugins with the system
 * @return 0 on success, -1 on error
 */
int register_sample_plugins(void);

/**
 * Register only the uppercase transformer plugin
 * Used for backward compatibility testing
 * @return 0 on success, -1 on error  
 */
int register_uppercase_plugin_only(void);

#endif /* SAMPLE_PLUGINS_H */