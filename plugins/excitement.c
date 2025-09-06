#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../plugin.h"

/* Example plugin that adds excitement to the message */
static char* add_excitement(const char* input) {
    int len = strlen(input);
    char* result = malloc(len + 4); /* +3 for "!!!" +1 for null terminator */
    strcpy(result, input);
    strcat(result, "!!!");
    return result;
}

static void before_main(void) {
    printf("[Excitement Plugin] Adding some excitement...\n");
}

static plugin_t excitement_plugin = {
    .name = "Excitement Plugin",
    .transform_message = add_excitement,
    .pre_main_hook = before_main,
    .post_main_hook = NULL
};

/* Auto-register the plugin */
__attribute__((constructor))
static void register_excitement_plugin(void) {
    register_plugin(&excitement_plugin);
}