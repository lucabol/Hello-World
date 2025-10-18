/* Plugin System Implementation
 * 
 * Provides runtime plugin loading and management for hello.c
 */
#include "plugin.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Plugin registry - linked list of loaded plugins */
static plugin_entry_t* plugin_registry = NULL;

/* Initialize the plugin system */
void plugin_system_init(void) {
    plugin_registry = NULL;
}

/* Register a statically linked plugin */
int plugin_register(plugin_info_t* info) {
    if (!info) {
        return PLUGIN_ERROR;
    }
    
    /* Verify API version compatibility */
    if (info->api_version != PLUGIN_API_VERSION) {
        fprintf(stderr, "Plugin API version mismatch: expected %d, got %d\n", 
                PLUGIN_API_VERSION, info->api_version);
        return PLUGIN_ERROR;
    }
    
    /* Verify required fields */
    if (!info->transform) {
        fprintf(stderr, "Plugin missing required transform function\n");
        return PLUGIN_ERROR;
    }
    
    /* Create a new registry entry */
    plugin_entry_t* entry = (plugin_entry_t*)malloc(sizeof(plugin_entry_t));
    if (!entry) {
        return PLUGIN_ERROR;
    }
    
    entry->info = info;
    entry->handle = NULL;  /* Static plugins don't have dynamic handles */
    entry->next = NULL;
    
    /* Add to the end of the list to maintain registration order */
    if (!plugin_registry) {
        plugin_registry = entry;
    } else {
        plugin_entry_t* current = plugin_registry;
        while (current->next) {
            current = current->next;
        }
        current->next = entry;
    }
    
    /* Call plugin initialization if provided */
    if (info->init) {
        if (info->init() != PLUGIN_SUCCESS) {
            fprintf(stderr, "Plugin initialization failed: %s\n", info->name);
            return PLUGIN_ERROR;
        }
    }
    
    return PLUGIN_SUCCESS;
}

/* Apply all registered plugins to transform the greeting */
int plugin_apply_all(const char* input, char* output, size_t output_size) {
    if (!input || !output || output_size == 0) {
        return PLUGIN_ERROR;
    }
    
    /* If no plugins registered, just copy input to output */
    if (!plugin_registry) {
        size_t len = strlen(input);
        if (len >= output_size) {
            return PLUGIN_ERROR;
        }
        strncpy(output, input, output_size - 1);
        output[output_size - 1] = '\0';
        return PLUGIN_SUCCESS;
    }
    
    /* Use two buffers to alternate transformations */
    char buffer1[256];
    char buffer2[256];
    char* current_input = buffer1;
    char* current_output = buffer2;
    
    /* Copy initial input to first buffer */
    strncpy(current_input, input, sizeof(buffer1) - 1);
    current_input[sizeof(buffer1) - 1] = '\0';
    
    /* Apply each plugin in sequence */
    plugin_entry_t* current = plugin_registry;
    while (current) {
        int result = current->info->transform(current_input, current_output, sizeof(buffer2));
        if (result != PLUGIN_SUCCESS) {
            fprintf(stderr, "Plugin transformation failed: %s\n", current->info->name);
            return PLUGIN_ERROR;
        }
        
        /* Swap buffers for next iteration */
        char* temp = current_input;
        current_input = current_output;
        current_output = temp;
        
        current = current->next;
    }
    
    /* Copy final result to output buffer */
    size_t len = strlen(current_input);
    if (len >= output_size) {
        return PLUGIN_ERROR;
    }
    strncpy(output, current_input, output_size - 1);
    output[output_size - 1] = '\0';
    
    return PLUGIN_SUCCESS;
}

/* Cleanup and unload all plugins */
void plugin_system_cleanup(void) {
    plugin_entry_t* current = plugin_registry;
    
    while (current) {
        /* Call cleanup function if provided */
        if (current->info && current->info->cleanup) {
            current->info->cleanup();
        }
        
        plugin_entry_t* next = current->next;
        free(current);
        current = next;
    }
    
    plugin_registry = NULL;
}

/* Dynamic loading support (stub for now - can be extended with dlopen) */
int plugin_load(const char* path) {
    (void)path;  /* Unused parameter */
    fprintf(stderr, "Dynamic plugin loading not yet implemented\n");
    return PLUGIN_ERROR;
}
