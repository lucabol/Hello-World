/* Test C++ compatibility with extern "C" guards */
#include <iostream>
#include "plugin.h"

extern const plugin_t uppercase_plugin;

int main() {
    plugin_init();
    
    int result = plugin_register(&uppercase_plugin);
    if (result != PLUGIN_SUCCESS) {
        std::cerr << "Failed to register plugin" << std::endl;
        return 1;
    }
    
    const char* output = plugin_apply_all("Hello from C++!");
    if (output == NULL) {
        int error = plugin_get_last_error();
        std::cerr << "Plugin transformation failed with error: " << error << std::endl;
        return 1;
    }
    
    std::cout << "Result: " << output << std::endl;
    std::cout << "Plugin count: " << plugin_count() << std::endl;
    
    return 0;
}
