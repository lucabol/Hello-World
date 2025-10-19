/* voice_demo.c - Demonstration of voice-driven code editing
 * Shows how voice commands can be used to interact with and edit code
 */
#include <stdio.h>
#include <stdlib.h>
#include "voice.h"

int main(int argc, char* argv[]) {
    /* If a command is provided as argument, process it and exit */
    if (argc > 1) {
        /* Join all arguments into a single command string */
        char command[256] = "";
        int pos = 0;
        
        for (int i = 1; i < argc && pos < 255; i++) {
            int len = 0;
            while (argv[i][len] != '\0' && pos < 254) {
                command[pos++] = argv[i][len++];
            }
            if (i < argc - 1 && pos < 254) {
                command[pos++] = ' ';
            }
        }
        command[pos] = '\0';
        
        int result = process_voice_command(command);
        return result;
    }
    
    /* Interactive demo mode - demonstrate various voice commands */
    printf("========================================\n");
    printf("Voice-Driven Code Editing Demo\n");
    printf("========================================\n");
    printf("\n");
    
    printf("Demonstrating voice commands:\n");
    printf("\n");
    
    /* Demo 1: Say hello command */
    printf("1. Voice command: \"say hello\"\n");
    printf("   Output:\n   ");
    process_voice_command("say hello");
    printf("\n");
    
    /* Demo 2: Change message command */
    printf("2. Voice command: \"change message Greetings, everyone!\"\n");
    printf("   Output:\n   ");
    process_voice_command("change message Greetings, everyone!");
    printf("\n");
    
    /* Demo 3: Show code command */
    printf("3. Voice command: \"show code\"\n");
    printf("   Output:\n   ");
    process_voice_command("show code");
    printf("\n");
    
    /* Demo 4: Invalid command (error handling) */
    printf("4. Voice command: \"invalid command\" (testing error handling)\n");
    printf("   Output:\n   ");
    process_voice_command("invalid command");
    printf("\n");
    
    printf("========================================\n");
    printf("Demo completed successfully!\n");
    printf("========================================\n");
    
    return 0;
}
