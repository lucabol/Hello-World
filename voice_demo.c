#include "voice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_help(void) {
    printf("Voice-Driven Code Editing Demo\n");
    printf("==============================\n\n");
    printf("Usage: ./voice_demo [--demo|--apply] [command]\n");
    printf("  --demo   (default) Safe demonstration mode - shows intended actions\n");
    printf("  --apply  Apply mode - actually modify files (use with caution!)\n\n");
    printf("Supported voice commands:\n");
    printf("  'say hello'                 - Execute the hello program\n");
    printf("  'change message <text>'     - Change the greeting message\n");
    printf("  'show code'                 - Display current code structure\n");
    printf("  'insert code <code>'        - Insert code at current location\n");
    printf("  'navigate to <function>'    - Navigate to specified function\n");
    printf("  'refactor <target>'         - Refactor code elements\n\n");
    printf("Safety: Commands default to demo mode unless --apply is specified.\n\n");
}

void interactive_demo(voice_safety_mode_t safety_mode) {
    printf("Voice-Driven Code Editing Demo\n");
    printf("==============================\n");
    printf("Mode: %s\n\n", safety_mode == VOICE_SAFETY_DEMO ? "DEMO (safe)" : "APPLY (modifies files)");
    
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
            execute_voice_command(cmd, safety_mode);
            free_voice_command(cmd);
        } else {
            printf("Failed to parse command\n");
        }
        
        printf("\n");
    }
    
    printf("Demo completed. Voice-driven code editing system ready!\n");
}

int main(int argc, char* argv[]) {
    voice_safety_mode_t safety_mode = VOICE_SAFETY_DEMO;  // Default to safe demo mode
    int first_command_arg = 1;
    
    // Parse safety mode flags
    if (argc > 1) {
        if (strcmp(argv[1], "--demo") == 0) {
            safety_mode = VOICE_SAFETY_DEMO;
            first_command_arg = 2;
        } else if (strcmp(argv[1], "--apply") == 0) {
            safety_mode = VOICE_SAFETY_APPLY;
            first_command_arg = 2;
            printf("WARNING: --apply mode selected. This may modify files!\n");
        } else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            print_help();
            return 0;
        }
    }
    
    // Check if a specific command was provided as argument
    if (argc > first_command_arg) {
        // Combine remaining arguments into a single command string
        char command[1024] = "";
        for (int i = first_command_arg; i < argc; i++) {
            if (i > first_command_arg) strcat(command, " ");
            strcat(command, argv[i]);
        }
        
        // Execute the single command
        voice_command_t* cmd = parse_voice_command(command);
        if (cmd) {
            int result = execute_voice_command(cmd, safety_mode);
            free_voice_command(cmd);
            return result;
        } else {
            printf("Failed to parse voice command: %s\n", command);
            return 1;
        }
    }
    
    // If no command arguments, run interactive demo
    print_help();
    interactive_demo(safety_mode);
    return 0;
}