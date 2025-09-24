# include <stdio.h>
# include <string.h>

// Voice-driven code editing demonstration
// This version activates the voice command simulation for testing

void process_voice_command(const char* command) {
    // Simulate voice command processing for code editing
    if (strcmp(command, "say hello") == 0) {
        printf("Hello world!\n");
    } else if (strncmp(command, "change message ", 15) == 0) {
        // Extract new message from command
        const char* new_message = command + 15;
        printf("Voice command: Would change message to: %s\n", new_message);
    } else if (strcmp(command, "show code") == 0) {
        printf("Voice command: Showing current code structure\n");
        printf("- main() function with printf statement\n");
        printf("- Voice command processing framework\n");
    } else {
        printf("Voice command not recognized: %s\n", command);
    }
}

int main(){
    printf("=== Voice-Driven Code Editing Demo ===\n");
    printf("Simulating voice commands for code editing:\n\n");
    
    // Demonstrate voice command processing
    printf("1. Voice command: 'say hello'\n");
    process_voice_command("say hello");
    printf("\n");
    
    printf("2. Voice command: 'change message Custom greeting from voice'\n");
    process_voice_command("change message Custom greeting from voice");
    printf("\n");
    
    printf("3. Voice command: 'show code'\n");
    process_voice_command("show code");
    printf("\n");
    
    printf("4. Voice command: 'invalid command'\n");
    process_voice_command("invalid command");
    printf("\n");
    
    printf("=== Demo Complete ===\n");
    
    return 0;
}