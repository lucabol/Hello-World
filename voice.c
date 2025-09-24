#include <stdio.h>
#include <string.h>
#include "voice.h"

void process_voice_command(const char* command) {
    // Input validation for safety
    if (command == NULL) {
        printf("Voice command error: NULL command received\n");
        return;
    }
    
    // Simulate voice command processing for code editing
    if (strcmp(command, "say hello") == 0) {
        printf("Hello world!\n");
    } else if (strncmp(command, "change message ", 15) == 0) {
        // Extract new message from command
        const char* new_message = command + 15;
        // Note: This only simulates changing the message - no files are modified
        printf("Voice command: Would change message to: %s\n", new_message);
    } else if (strcmp(command, "show code") == 0) {
        printf("Voice command: Showing current code structure\n");
        printf("- main() function with printf statement\n");
        printf("- Voice command processing framework\n");
    } else {
        printf("Voice command not recognized: %s\n", command);
    }
}