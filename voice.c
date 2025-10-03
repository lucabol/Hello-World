/* Voice command implementation
 * Implements voice-driven code editing commands
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "voice.h"

/* Helper function to trim leading whitespace */
static const char* trim_leading(const char* str) {
    while (*str && isspace((unsigned char)*str)) {
        str++;
    }
    return str;
}

/* Parse and execute a voice command */
int parse_voice_command(const char* command) {
    if (command == NULL) {
        return VOICE_ERROR_INVALID_ARG;
    }
    
    /* Command: "say hello" - Execute the program to show greeting */
    if (strcmp(command, "say hello") == 0) {
        printf("Ciao, Mondo!\n");
        return VOICE_SUCCESS;
    }
    
    /* Command: "change message <text>" - Simulate changing the message */
    if (strncmp(command, "change message", 14) == 0) {
        const char* rest = command + 14;
        /* Check if there's a space after "change message" */
        if (*rest == '\0') {
            printf("Error: 'change message' requires a message argument\n");
            return VOICE_ERROR_INVALID_ARG;
        }
        if (*rest == ' ') {
            const char* new_message = trim_leading(rest + 1);
            if (*new_message == '\0') {
                printf("Error: 'change message' requires a message argument\n");
                return VOICE_ERROR_INVALID_ARG;
            }
            printf("Would change message to: %s\n", new_message);
            return VOICE_SUCCESS;
        }
        /* Falls through to unknown command if not matching */
    }
    
    /* Command: "show code" - Display code structure */
    if (strcmp(command, "show code") == 0) {
        printf("Showing current code structure:\n");
        printf("  - main() function\n");
        printf("  - printf() call with greeting message\n");
        return VOICE_SUCCESS;
    }
    
    /* Unknown command */
    printf("Voice command not recognized: %s\n", command);
    return VOICE_ERROR_UNKNOWN_COMMAND;
}
