#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../plugin.h"

/* Example plugin that converts message to uppercase */
static char* to_uppercase(const char* input) {
    int len = strlen(input);
    char* result = malloc(len + 1);
    for (int i = 0; i <= len; i++) {
        result[i] = toupper(input[i]);
    }
    return result;
}

static void after_main(void) {
    printf("\n[Uppercase Plugin] Message transformation complete.");
}

static plugin_t uppercase_plugin = {
    .name = "Uppercase Plugin",
    .transform_message = to_uppercase,
    .pre_main_hook = NULL,
    .post_main_hook = after_main
};

/* Auto-register the plugin */
__attribute__((constructor))
static void register_uppercase_plugin(void) {
    register_plugin(&uppercase_plugin);
}