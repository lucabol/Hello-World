#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include "plugin.h"

#define MAX_PLUGINS 32
#define PLUGINS_DIR "./plugins"
#define MAX_PATH_LENGTH 512
#define MAX_MESSAGE_LENGTH 4096

/* Error codes for plugin operations */
typedef enum {
    PLUGIN_SUCCESS = 0,
    PLUGIN_ERROR_MEMORY,
    PLUGIN_ERROR_INVALID_PATH,
    PLUGIN_ERROR_LOAD_FAILED,
    PLUGIN_ERROR_INVALID_INTERFACE,
    PLUGIN_ERROR_VERSION_MISMATCH,
    PLUGIN_ERROR_INIT_FAILED,
    PLUGIN_ERROR_MAX_PLUGINS
} plugin_error_t;

/* Plugin manager structure */
typedef struct {
    void* handle;
    plugin_interface_t* interface;
} loaded_plugin_t;

/* Global plugin storage */
static loaded_plugin_t plugins[MAX_PLUGINS];
static int plugin_count = 0;

/* Validate plugin file path for security */
static int is_safe_plugin_path(const char* filepath) {
    if (!filepath) return 0;
    
    /* Check path length */
    if (strlen(filepath) >= MAX_PATH_LENGTH) return 0;
    
    /* Must end with .so */
    if (!strstr(filepath, ".so")) return 0;
    
    /* Must be within plugins directory */
    if (strncmp(filepath, PLUGINS_DIR, strlen(PLUGINS_DIR)) != 0) return 0;
    
    /* Check for path traversal attempts */
    if (strstr(filepath, "..") || strstr(filepath, "//")) return 0;
    
    /* Verify file exists and is regular file */
    struct stat st;
    if (stat(filepath, &st) != 0) return 0;
    if (!S_ISREG(st.st_mode)) return 0;
    
    return 1;
}

/* Load a single plugin with improved error handling */
static plugin_error_t load_plugin(const char* filename) {
    if (plugin_count >= MAX_PLUGINS) {
        fprintf(stderr, "Error: Maximum number of plugins (%d) reached\n", MAX_PLUGINS);
        return PLUGIN_ERROR_MAX_PLUGINS;
    }
    
    /* Validate plugin path for security */
    if (!is_safe_plugin_path(filename)) {
        fprintf(stderr, "Warning: Skipping unsafe plugin path: %s\n", filename);
        return PLUGIN_ERROR_INVALID_PATH;
    }
    
    void* handle = dlopen(filename, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Warning: Failed to load plugin %s: %s\n", filename, dlerror());
        return PLUGIN_ERROR_LOAD_FAILED;
    }
    
    /* Clear any existing error */
    dlerror();
    
    /* Get the plugin interface */
    plugin_interface_t* (*get_interface)(void) = dlsym(handle, "get_plugin_interface");
    char* error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "Warning: Plugin %s missing get_plugin_interface: %s\n", filename, error);
        dlclose(handle);
        return PLUGIN_ERROR_INVALID_INTERFACE;
    }
    
    if (!get_interface) {
        fprintf(stderr, "Warning: Plugin %s has NULL get_plugin_interface\n", filename);
        dlclose(handle);
        return PLUGIN_ERROR_INVALID_INTERFACE;
    }
    
    plugin_interface_t* interface = get_interface();
    if (!interface || !interface->get_info) {
        fprintf(stderr, "Warning: Plugin %s has invalid interface\n", filename);
        dlclose(handle);
        return PLUGIN_ERROR_INVALID_INTERFACE;
    }
    
    /* Check API version compatibility */
    plugin_info_t* info = interface->get_info();
    if (!info) {
        fprintf(stderr, "Warning: Plugin %s returned NULL info\n", filename);
        dlclose(handle);
        return PLUGIN_ERROR_INVALID_INTERFACE;
    }
    
    if (info->api_version != PLUGIN_API_VERSION) {
        fprintf(stderr, "Warning: Plugin %s API version mismatch (expected %d, got %d)\n", 
                filename, PLUGIN_API_VERSION, info->api_version);
        dlclose(handle);
        return PLUGIN_ERROR_VERSION_MISMATCH;
    }
    
    /* Validate plugin info for security */
    if (!info->name || !info->version || !info->description) {
        fprintf(stderr, "Warning: Plugin %s has incomplete metadata\n", filename);
        dlclose(handle);
        return PLUGIN_ERROR_INVALID_INTERFACE;
    }
    
    /* Check metadata lengths for security */
    if (strlen(info->name) > PLUGIN_MAX_NAME_LENGTH ||
        strlen(info->version) > PLUGIN_MAX_VERSION_LENGTH ||
        strlen(info->description) > PLUGIN_MAX_DESCRIPTION_LENGTH) {
        fprintf(stderr, "Warning: Plugin %s has metadata that is too long\n", filename);
        dlclose(handle);
        return PLUGIN_ERROR_INVALID_INTERFACE;
    }
    
    /* Validate plugin type */
    if (info->type != PLUGIN_TYPE_MESSAGE_TRANSFORMER && 
        info->type != PLUGIN_TYPE_OUTPUT_FORMATTER) {
        fprintf(stderr, "Warning: Plugin %s has invalid type %d\n", filename, info->type);
        dlclose(handle);
        return PLUGIN_ERROR_INVALID_INTERFACE;
    }
    
    /* Initialize plugin if it has an init function */
    if (interface->init) {
        if (interface->init() != 0) {
            fprintf(stderr, "Warning: Plugin %s initialization failed\n", filename);
            dlclose(handle);
            return PLUGIN_ERROR_INIT_FAILED;
        }
    }
    
    /* Store the plugin */
    plugins[plugin_count].handle = handle;
    plugins[plugin_count].interface = interface;
    plugin_count++;
    
    return PLUGIN_SUCCESS;
}

/* Load all plugins from the plugins directory with improved error handling */
static void load_plugins(void) {
    DIR* dir = opendir(PLUGINS_DIR);
    if (!dir) {
        /* No plugins directory - continue without plugins */
        if (errno != ENOENT) {
            fprintf(stderr, "Warning: Cannot access plugins directory: %s\n", strerror(errno));
        }
        return;
    }
    
    struct dirent* entry;
    char filepath[MAX_PATH_LENGTH];
    int loaded_count = 0;
    int total_attempted = 0;
    
    while ((entry = readdir(dir)) != NULL) {
        /* Look for .so files (shared libraries) */
        if (strstr(entry->d_name, ".so")) {
            total_attempted++;
            
            /* Build full file path */
            int ret = snprintf(filepath, sizeof(filepath), "%s/%s", PLUGINS_DIR, entry->d_name);
            if (ret >= (int)sizeof(filepath) || ret < 0) {
                fprintf(stderr, "Warning: Plugin filename too long: %s\n", entry->d_name);
                continue;
            }
            
            plugin_error_t result = load_plugin(filepath);
            if (result == PLUGIN_SUCCESS) {
                loaded_count++;
            }
        }
    }
    
    closedir(dir);
    
    if (total_attempted > 0) {
        printf("Loaded %d/%d plugins successfully\n", loaded_count, total_attempted);
    }
}

/* Apply message transformers to the input message with error handling */
static char* apply_transformers(const char* message) {
    if (!message) {
        fprintf(stderr, "Error: NULL message passed to transformers\n");
        return NULL;
    }
    
    /* Check message length for security */
    size_t msg_len = strlen(message);
    if (msg_len > MAX_MESSAGE_LENGTH) {
        fprintf(stderr, "Error: Message too long (%zu bytes, max %d)\n", msg_len, MAX_MESSAGE_LENGTH);
        return NULL;
    }
    
    char* current_message = strdup(message);
    if (!current_message) {
        fprintf(stderr, "Error: Memory allocation failed for message transformation\n");
        return NULL;
    }
    
    for (int i = 0; i < plugin_count; i++) {
        plugin_interface_t* interface = plugins[i].interface;
        if (!interface || !interface->get_info) {
            fprintf(stderr, "Warning: Invalid plugin interface at index %d\n", i);
            continue;
        }
        
        plugin_info_t* info = interface->get_info();
        if (!info) {
            fprintf(stderr, "Warning: Plugin at index %d returned NULL info\n", i);
            continue;
        }
        
        if (info->type == PLUGIN_TYPE_MESSAGE_TRANSFORMER && interface->transform_message) {
            char* new_message = interface->transform_message(current_message);
            if (new_message) {
                /* Validate transformed message length */
                if (strlen(new_message) > MAX_MESSAGE_LENGTH) {
                    fprintf(stderr, "Warning: Plugin %s produced message too long, skipping\n", 
                            info->name ? info->name : "unknown");
                    free(new_message);
                    continue;
                }
                
                free(current_message);
                current_message = new_message;
            } else {
                fprintf(stderr, "Warning: Plugin %s transformation failed\n", 
                        info->name ? info->name : "unknown");
            }
        }
    }
    
    return current_message;
}

/* Apply output formatters to display the message with error handling */
static void apply_formatters(const char* message) {
    if (!message) {
        fprintf(stderr, "Error: NULL message passed to formatters\n");
        return;
    }
    
    int formatted = 0;
    
    for (int i = 0; i < plugin_count; i++) {
        plugin_interface_t* interface = plugins[i].interface;
        if (!interface || !interface->get_info) {
            fprintf(stderr, "Warning: Invalid plugin interface at index %d\n", i);
            continue;
        }
        
        plugin_info_t* info = interface->get_info();
        if (!info) {
            fprintf(stderr, "Warning: Plugin at index %d returned NULL info\n", i);
            continue;
        }
        
        if (info->type == PLUGIN_TYPE_OUTPUT_FORMATTER && interface->format_output) {
            interface->format_output(message);
            formatted = 1;
        }
    }
    
    /* If no formatters handled the output, use default behavior */
    if (!formatted) {
        printf("%s", message);
    }
}

/* Cleanup all loaded plugins with error handling */
static void cleanup_plugins(void) {
    for (int i = 0; i < plugin_count; i++) {
        if (plugins[i].interface && plugins[i].interface->cleanup) {
            plugins[i].interface->cleanup();
        }
        if (plugins[i].handle) {
            if (dlclose(plugins[i].handle) != 0) {
                fprintf(stderr, "Warning: Failed to close plugin handle: %s\n", dlerror());
            }
        }
    }
    plugin_count = 0; /* Reset for safety */
}

int main() {
    const char* base_message = "Hello world!";
    
    /* Load plugins */
    load_plugins();
    
    /* Transform the message through plugins */
    char* transformed_message = apply_transformers(base_message);
    if (!transformed_message) {
        fprintf(stderr, "Error: Message transformation failed, using original message\n");
        transformed_message = strdup(base_message);
        if (!transformed_message) {
            fprintf(stderr, "Error: Critical memory allocation failure\n");
            cleanup_plugins();
            return 1;
        }
    }
    
    /* Output the message through formatters or default */
    apply_formatters(transformed_message);
    
    /* Ensure output ends with newline for proper formatting */
    if (transformed_message[strlen(transformed_message) - 1] != '\n') {
        printf("\n");
    }
    
    /* Cleanup */
    free(transformed_message);
    cleanup_plugins();
    
    return 0;
}
