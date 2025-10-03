/* Voice command implementation
 * Implements voice-driven code editing commands
 */
#include <stdio.h>
#include <string.h>
#include "voice.h"

/* Parse and execute a voice command */
int parse_voice_command(const char* command) {
    if (command == NULL) {
        return VOICE_ERROR;
    }
    
    /* Command: "say hello" - Execute the program to show greeting */
    if (strcmp(command, "say hello") == 0) {
        printf("Ciao, Mondo!");
        return VOICE_SUCCESS;
    }
    
    /* Command: "change message <text>" - Simulate changing the message */
    if (strncmp(command, "change message ", 15) == 0) {
        const char* new_message = command + 15;
        printf("Would change message to: %s", new_message);
        return VOICE_SUCCESS;
    }
    
    /* Command: "show code" - Display code structure */
    if (strcmp(command, "show code") == 0) {
        printf("Showing current code structure:");
        printf("\n  - main() function");
        printf("\n  - printf() call with greeting message");
        return VOICE_SUCCESS;
    }
    
    /* Unknown command */
    printf("Voice command not recognized: %s", command);
    return VOICE_ERROR;
}
