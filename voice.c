/* voice.c - Voice-driven code editing implementation
 * Implements voice command processing for code navigation and editing
 */
#include <stdio.h>
#include <string.h>
#include "voice.h"

/* Standalone implementation of get_greeting for voice command processing
 * This allows voice.c to be compiled and tested independently
 */
static const char* get_greeting(void) {
    return "Hello world!";
}

/* Process a voice command string
 * Returns VOICE_CMD_SUCCESS if command recognized, VOICE_CMD_ERROR otherwise
 */
int process_voice_command(const char* command) {
    if (command == NULL) {
        printf("Voice command not recognized: (null)\n");
        return VOICE_CMD_ERROR;
    }
    
    /* Command: "say hello" - Execute the hello program */
    if (strcmp(command, "say hello") == 0) {
        const char* greeting = get_greeting();
        printf("%s\n", greeting);
        return VOICE_CMD_SUCCESS;
    }
    
    /* Command: "change message <new_text>" - Simulate message change */
    if (strncmp(command, "change message ", 15) == 0) {
        const char* new_message = command + 15;
        printf("Would change message to: %s\n", new_message);
        printf("(This is a simulation - actual file modification requires user confirmation)\n");
        return VOICE_CMD_SUCCESS;
    }
    
    /* Command: "show code" - Display code structure information */
    if (strcmp(command, "show code") == 0) {
        printf("Showing current code structure:\n");
        printf("  - File: hello.c\n");
        printf("  - Function: main()\n");
        printf("  - Function: get_greeting()\n");
        printf("  - Current message: \"%s\"\n", get_greeting());
        return VOICE_CMD_SUCCESS;
    }
    
    /* Unrecognized command */
    printf("Voice command not recognized: %s\n", command);
    printf("Available commands:\n");
    printf("  - say hello\n");
    printf("  - change message <new_text>\n");
    printf("  - show code\n");
    return VOICE_CMD_ERROR;
}
