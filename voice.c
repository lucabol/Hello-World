/* Voice Command Implementation
 * Provides voice-driven code editing capabilities for hello.c
 */
#include <stdio.h>
#include <string.h>
#include "voice.h"

/* Helper function to check if a string starts with a prefix */
static int starts_with(const char* str, const char* prefix) {
    size_t prefix_len = strlen(prefix);
    size_t str_len = strlen(str);
    if (str_len < prefix_len) {
        return 0;
    }
    return strncmp(str, prefix, prefix_len) == 0;
}

/* Process voice command: "say hello" */
static int cmd_say_hello(void) {
    printf("Hello world!\n");
    return 0;
}

/* Process voice command: "change message <text>" */
static int cmd_change_message(const char* new_message) {
    if (new_message == NULL || strlen(new_message) == 0) {
        printf("Error: 'change message' requires a message text\n");
        return 1;
    }
    printf("Would change message to: %s\n", new_message);
    printf("(Simulation mode - no actual file changes)\n");
    return 0;
}

/* Process voice command: "show code" */
static int cmd_show_code(void) {
    printf("Showing current code structure:\n");
    printf("  - Function: main()\n");
    printf("  - Output: \"Hello world!\\n\"\n");
    printf("  - Lines: 5\n");
    return 0;
}

/* Main voice command processor */
int process_voice_command(const char* command) {
    if (command == NULL) {
        printf("Error: NULL command\n");
        return 1;
    }
    
    /* Trim leading whitespace */
    while (*command == ' ' || *command == '\t') {
        command++;
    }
    
    /* Check for empty command */
    if (strlen(command) == 0) {
        printf("Error: Empty command\n");
        return 1;
    }
    
    /* Process "say hello" command */
    if (strcmp(command, "say hello") == 0) {
        return cmd_say_hello();
    }
    
    /* Process "change message" command */
    if (starts_with(command, "change message ")) {
        const char* message = command + strlen("change message ");
        return cmd_change_message(message);
    }
    
    /* Process "show code" command */
    if (strcmp(command, "show code") == 0) {
        return cmd_show_code();
    }
    
    /* Unknown command */
    printf("Voice command not recognized: %s\n", command);
    return 1;
}
