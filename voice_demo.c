#include "voice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_help(void) {
    printf("Voice-Driven Code Editing Demo\n");
    printf("==============================\n\n");
    printf("Supported voice commands:\n");
    printf("  'say hello'                 - Execute the hello program\n");
    printf("  'change message <text>'     - Change the greeting message\n");
    printf("  'show code'                 - Display current code structure\n");
    printf("  'insert code <code>'        - Insert code at current location\n");
    printf("  'navigate to <function>'    - Navigate to specified function\n");
    printf("  'refactor <target>'         - Refactor code elements\n\n");
}

void interactive_demo(void) {
    printf("Voice-Driven Code Editing Demo\n");
    printf("==============================\n\n");
    
    // Demo sequence showing different voice commands
    const char* demo_commands[] = {
        "say hello",
        "change message Greetings from Voice Control!",
        "show code",
        "navigate to main",
        "insert code // Voice-controlled comment",
        "refactor function main",
        "invalid command"  // Test error handling
    };
    
    int num_commands = sizeof(demo_commands) / sizeof(demo_commands[0]);
    
    for (int i = 0; i < num_commands; i++) {
        printf("Voice Command: \"%s\"\n", demo_commands[i]);
        printf("Response: ");
        
        voice_command_t* cmd = parse_voice_command(demo_commands[i]);
        if (cmd) {
            execute_voice_command(cmd);
            free_voice_command(cmd);
        } else {
            printf("Failed to parse command\n");
        }
        
        printf("\n");
    }
    
    printf("Demo completed. Voice-driven code editing system ready!\n");
}

int main(int argc, char* argv[]) {
    // Check if a specific command was provided as argument
    if (argc > 1) {
        // Combine all arguments into a single command string
        char command[1024] = "";
        for (int i = 1; i < argc; i++) {
            if (i > 1) strcat(command, " ");
            strcat(command, argv[i]);
        }
        
        // Execute the single command
        voice_command_t* cmd = parse_voice_command(command);
        if (cmd) {
            int result = execute_voice_command(cmd);
            free_voice_command(cmd);
            return result;
        } else {
            printf("Failed to parse voice command: %s\n", command);
            return 1;
        }
    }
    
    // If no arguments, run interactive demo
    print_help();
    interactive_demo();
    return 0;
}