/* Voice-Driven Code Editing Demo
 * Demonstrates voice command functionality for editing hello.c
 * 
 * Usage:
 *   ./voice_demo                    - Run interactive demo with all commands
 *   ./voice_demo "command"          - Execute a single command non-interactively
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "voice.h"

/* Run interactive demo showing all voice commands */
static void run_demo(void) {
    printf("========================================\n");
    printf("Voice-Driven Code Editing Demo\n");
    printf("========================================\n\n");
    
    /* Demonstrate each command */
    printf("Command: say hello\n");
    printf("Output: ");
    parse_voice_command("say hello");
    printf("\n");
    
    printf("Command: change message New greeting text\n");
    printf("Output: ");
    parse_voice_command("change message New greeting text");
    printf("\n");
    
    printf("Command: show code\n");
    printf("Output: ");
    parse_voice_command("show code");
    printf("\n");
    
    printf("Command: invalid command\n");
    printf("Output: ");
    parse_voice_command("invalid command");
    printf("\n");
    
    printf("========================================\n");
    printf("Demo completed\n");
    printf("========================================\n");
}

int main(int argc, char* argv[]) {
    /* If command provided as argument, execute it non-interactively */
    if (argc > 1) {
        /* Concatenate all arguments after the first to form the command */
        char command[256];
        size_t pos = 0;
        
        /* Use snprintf for safe buffer assembly */
        for (int i = 1; i < argc && pos < sizeof(command) - 1; i++) {
            int written;
            if (i > 1) {
                /* Add space between arguments */
                written = snprintf(command + pos, sizeof(command) - pos, " %s", argv[i]);
            } else {
                /* First argument, no leading space */
                written = snprintf(command + pos, sizeof(command) - pos, "%s", argv[i]);
            }
            
            /* Check for truncation or error */
            if (written < 0) {
                fprintf(stderr, "Error formatting command\n");
                return VOICE_ERROR_INVALID_ARG;
            }
            if ((size_t)written >= sizeof(command) - pos) {
                fprintf(stderr, "Error: Command too long (max %zu characters)\n", sizeof(command) - 1);
                return VOICE_ERROR_INVALID_ARG;
            }
            
            pos += (size_t)written;
        }
        
        command[pos] = '\0';
        
        int result = parse_voice_command(command);
        return result;
    }
    
    /* Otherwise run the interactive demo */
    run_demo();
    return 0;
}
