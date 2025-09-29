#ifndef VOICE_H
#define VOICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Voice command safety mode
typedef enum {
    VOICE_SAFETY_DEMO,    // Demo mode - show intended actions without modifying files
    VOICE_SAFETY_APPLY    // Apply mode - actually modify files (requires explicit flag)
} voice_safety_mode_t;

// Voice command types for code editing
typedef enum {
    VOICE_CMD_SAY_HELLO,      // Execute the current hello program
    VOICE_CMD_CHANGE_MESSAGE, // Change the message in hello.c
    VOICE_CMD_SHOW_CODE,      // Display current code structure
    VOICE_CMD_INSERT_CODE,    // Insert code at specific location
    VOICE_CMD_NAVIGATE_TO,    // Navigate to specific function/line
    VOICE_CMD_REFACTOR,       // Refactor code elements
    VOICE_CMD_UNKNOWN         // Unknown/unsupported command
} voice_command_type_t;

// Structure to hold parsed voice command
typedef struct {
    voice_command_type_t type;
    char *parameter;          // Additional parameter for the command
    int line_number;          // For navigation commands
} voice_command_t;

// Function declarations
voice_command_t* parse_voice_command(const char* input);
int execute_voice_command(const voice_command_t* command, voice_safety_mode_t safety_mode);
void free_voice_command(voice_command_t* command);
const char* voice_command_type_string(voice_command_type_t type);

// Helper functions for code editing
int show_current_code(void);
int change_hello_message(const char* new_message, voice_safety_mode_t safety_mode);
int insert_code_at_line(const char* code, int line, voice_safety_mode_t safety_mode);
int navigate_to_function(const char* function_name);

#endif // VOICE_H