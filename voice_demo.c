#include <stdio.h>
#include "voice.h"

// Voice-driven code editing demonstration
// This version activates the voice command simulation for testing

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