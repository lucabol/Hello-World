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
    printf("\n\n");
    
    printf("Command: change message New greeting text\n");
    printf("Output: ");
    parse_voice_command("change message New greeting text");
    printf("\n\n");
    
    printf("Command: show code\n");
    printf("Output: ");
    parse_voice_command("show code");
    printf("\n\n");
    
    printf("Command: invalid command\n");
    printf("Output: ");
    parse_voice_command("invalid command");
    printf("\n\n");
    
    printf("========================================\n");
    printf("Demo completed\n");
    printf("========================================\n");
}

int main(int argc, char* argv[]) {
    /* If command provided as argument, execute it non-interactively */
    if (argc > 1) {
        /* Concatenate all arguments after the first to form the command */
        char command[256] = "";
        for (int i = 1; i < argc; i++) {
            if (i > 1) {
                strcat(command, " ");
            }
            strcat(command, argv[i]);
        }
        
        int result = parse_voice_command(command);
        return result;
    }
    
    /* Otherwise run the interactive demo */
    run_demo();
    return 0;
}
