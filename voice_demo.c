/* Voice-Driven Code Editing Demo
 * Demonstrates voice command capabilities for hello.c
 * 
 * Usage:
 *   ./voice_demo              - Run interactive demo with all commands
 *   ./voice_demo "command"    - Execute a single voice command
 * 
 * Examples:
 *   ./voice_demo "say hello"
 *   ./voice_demo "change message Greetings!"
 *   ./voice_demo "show code"
 */
#include <stdio.h>
#include "voice.h"

/* Run the interactive demo showcasing all voice commands */
static void run_demo(void) {
    int result;
    
    printf("Voice-Driven Code Editing Demo\n");
    printf("================================\n\n");
    
    printf("Command 1: 'say hello'\n");
    printf("Result: ");
    result = process_voice_command("say hello");
    printf("\n");
    
    printf("Command 2: 'change message Hello, Voice World!'\n");
    printf("Result: ");
    result = process_voice_command("change message Hello, Voice World!");
    printf("\n");
    
    printf("Command 3: 'show code'\n");
    printf("Result: ");
    result = process_voice_command("show code");
    printf("\n");
    
    printf("Command 4: 'invalid command' (error handling test)\n");
    printf("Result: ");
    result = process_voice_command("invalid command");
    printf("\n");
    
    printf("Demo completed!\n");
    
    /* Suppress unused variable warning */
    (void)result;
}

int main(int argc, char* argv[]) {
    /* If a command is provided as argument, execute it */
    if (argc > 1) {
        int result = process_voice_command(argv[1]);
        return result;
    }
    
    /* Otherwise, run the interactive demo */
    run_demo();
    return 0;
}
