#include "voice.h"

// Parse voice command from input string
voice_command_t* parse_voice_command(const char* input) {
    if (!input || strlen(input) == 0) {
        return NULL;
    }
    
    voice_command_t* cmd = malloc(sizeof(voice_command_t));
    if (!cmd) {
        return NULL;
    }
    
    cmd->parameter = NULL;
    cmd->line_number = -1;
    cmd->type = VOICE_CMD_UNKNOWN;
    
    // Parse different command patterns
    if (strncmp(input, "say hello", 9) == 0) {
        cmd->type = VOICE_CMD_SAY_HELLO;
    }
    else if (strncmp(input, "change message", 14) == 0) {
        cmd->type = VOICE_CMD_CHANGE_MESSAGE;
        // Extract the new message parameter
        const char* param_start = input + 14;
        while (*param_start == ' ') param_start++; // Skip spaces
        if (*param_start != '\0') {
            size_t param_len = strlen(param_start);
            cmd->parameter = malloc(param_len + 1);
            if (cmd->parameter) {
                strncpy(cmd->parameter, param_start, param_len);
                cmd->parameter[param_len] = '\0'; // Ensure null termination
            }
        }
    }
    else if (strncmp(input, "show code", 9) == 0) {
        cmd->type = VOICE_CMD_SHOW_CODE;
    }
    else if (strncmp(input, "insert code", 11) == 0) {
        cmd->type = VOICE_CMD_INSERT_CODE;
        // Extract code parameter
        const char* param_start = input + 11;
        while (*param_start == ' ') param_start++; // Skip spaces
        if (*param_start != '\0') {
            size_t param_len = strlen(param_start);
            cmd->parameter = malloc(param_len + 1);
            if (cmd->parameter) {
                strncpy(cmd->parameter, param_start, param_len);
                cmd->parameter[param_len] = '\0'; // Ensure null termination
            }
        }
    }
    else if (strncmp(input, "navigate to", 11) == 0) {
        cmd->type = VOICE_CMD_NAVIGATE_TO;
        // Extract navigation target
        const char* param_start = input + 11;
        while (*param_start == ' ') param_start++; // Skip spaces
        if (*param_start != '\0') {
            size_t param_len = strlen(param_start);
            cmd->parameter = malloc(param_len + 1);
            if (cmd->parameter) {
                strncpy(cmd->parameter, param_start, param_len);
                cmd->parameter[param_len] = '\0'; // Ensure null termination
            }
        }
    }
    else if (strncmp(input, "refactor", 8) == 0) {
        cmd->type = VOICE_CMD_REFACTOR;
        // Extract refactor parameter
        const char* param_start = input + 8;
        while (*param_start == ' ') param_start++; // Skip spaces
        if (*param_start != '\0') {
            size_t param_len = strlen(param_start);
            cmd->parameter = malloc(param_len + 1);
            if (cmd->parameter) {
                strncpy(cmd->parameter, param_start, param_len);
                cmd->parameter[param_len] = '\0'; // Ensure null termination
            }
        }
    }
    else {
        // Unknown command - store the full input
        cmd->type = VOICE_CMD_UNKNOWN;
        size_t input_len = strlen(input);
        cmd->parameter = malloc(input_len + 1);
        if (cmd->parameter) {
            strncpy(cmd->parameter, input, input_len);
            cmd->parameter[input_len] = '\0'; // Ensure null termination
        }
    }
    
    return cmd;
}

// Execute the parsed voice command
int execute_voice_command(const voice_command_t* command, voice_safety_mode_t safety_mode) {
    if (!command) {
        return -1;
    }
    
    switch (command->type) {
        case VOICE_CMD_SAY_HELLO:
            printf("Executing hello program:\n");
            return system("./hello");
            
        case VOICE_CMD_CHANGE_MESSAGE:
            if (command->parameter) {
                if (safety_mode == VOICE_SAFETY_DEMO) {
                    printf("Would change message to: %s\n", command->parameter);
                } else {
                    printf("Changing message to: %s\n", command->parameter);
                }
                return change_hello_message(command->parameter, safety_mode);
            } else {
                printf("Error: No message specified for change command\n");
                return -1;
            }
            
        case VOICE_CMD_SHOW_CODE:
            printf("Showing current code structure:\n");
            return show_current_code();
            
        case VOICE_CMD_INSERT_CODE:
            if (command->parameter) {
                if (safety_mode == VOICE_SAFETY_DEMO) {
                    printf("Would insert code: %s\n", command->parameter);
                } else {
                    printf("Inserting code: %s\n", command->parameter);
                }
                return insert_code_at_line(command->parameter, command->line_number, safety_mode);
            } else {
                printf("Error: No code specified for insert command\n");
                return -1;
            }
            
        case VOICE_CMD_NAVIGATE_TO:
            if (command->parameter) {
                printf("Navigating to: %s\n", command->parameter);
                return navigate_to_function(command->parameter);
            } else {
                printf("Error: No target specified for navigate command\n");
                return -1;
            }
            
        case VOICE_CMD_REFACTOR:
            if (command->parameter) {
                if (safety_mode == VOICE_SAFETY_DEMO) {
                    printf("Would refactor: %s\n", command->parameter);
                } else {
                    printf("Refactoring: %s\n", command->parameter);
                }
                // In a real implementation, this would perform refactoring
                return 0;
            } else {
                printf("Error: No refactor target specified\n");
                return -1;
            }
            
        case VOICE_CMD_UNKNOWN:
        default:
            printf("Voice command not recognized: %s\n", 
                   command->parameter ? command->parameter : "unknown");
            return 1;
    }
}

// Free memory allocated for voice command
void free_voice_command(voice_command_t* command) {
    if (command) {
        if (command->parameter) {
            free(command->parameter);
        }
        free(command);
    }
}

// Get string representation of command type
const char* voice_command_type_string(voice_command_type_t type) {
    switch (type) {
        case VOICE_CMD_SAY_HELLO: return "SAY_HELLO";
        case VOICE_CMD_CHANGE_MESSAGE: return "CHANGE_MESSAGE";
        case VOICE_CMD_SHOW_CODE: return "SHOW_CODE";
        case VOICE_CMD_INSERT_CODE: return "INSERT_CODE";
        case VOICE_CMD_NAVIGATE_TO: return "NAVIGATE_TO";
        case VOICE_CMD_REFACTOR: return "REFACTOR";
        case VOICE_CMD_UNKNOWN: return "UNKNOWN";
        default: return "INVALID";
    }
}

// Show current code structure (demo implementation)
int show_current_code(void) {
    FILE* file = fopen("hello.c", "r");
    if (!file) {
        printf("Error: Cannot open hello.c\n");
        return -1;
    }
    
    printf("Current hello.c contents:\n");
    printf("------------------------\n");
    char line[256];
    int line_num = 1;
    while (fgets(line, sizeof(line), file)) {
        printf("%2d: %s", line_num++, line);
    }
    printf("------------------------\n");
    
    fclose(file);
    return 0;
}

// Change hello message (demo implementation - shows what would be done)
int change_hello_message(const char* new_message, voice_safety_mode_t safety_mode) {
    if (!new_message) {
        return -1;
    }
    
    if (safety_mode == VOICE_SAFETY_DEMO) {
        printf("Demo: Would update hello.c to print: '%s'\n", new_message);
        printf("Note: This is a demonstration. In a real implementation,\n");
        printf("      this would modify the actual hello.c file.\n");
    } else {
        printf("WARNING: --apply mode not fully implemented yet.\n");
        printf("Demo: Would update hello.c to print: '%s'\n", new_message);
        printf("Note: File modification capabilities are planned for future versions.\n");
    }
    
    // For safety, we're not actually modifying files in this demo
    // A real implementation would:
    // 1. Read hello.c
    // 2. Parse and find the printf statement
    // 3. Replace the message
    // 4. Write back to hello.c
    
    return 0;
}

// Insert code at specified line (demo implementation)
int insert_code_at_line(const char* code, int line, voice_safety_mode_t safety_mode) {
    if (!code) {
        return -1;
    }
    
    if (safety_mode == VOICE_SAFETY_DEMO) {
        printf("Demo: Would insert '%s' at line %d\n", code, line > 0 ? line : 1);
        printf("Note: This is a demonstration. Real implementation would\n");
        printf("      modify the actual source file.\n");
    } else {
        printf("WARNING: --apply mode not fully implemented yet.\n");
        printf("Demo: Would insert '%s' at line %d\n", code, line > 0 ? line : 1);
        printf("Note: File modification capabilities are planned for future versions.\n");
    };
    printf("      modify the actual source file.\n");
    
    return 0;
}

// Navigate to function (demo implementation)
int navigate_to_function(const char* function_name) {
    if (!function_name) {
        return -1;
    }
    
    printf("Demo: Searching for function '%s' in hello.c\n", function_name);
    
    if (strcmp(function_name, "main") == 0) {
        printf("Found 'main' function at line 3\n");
    } else {
        printf("Function '%s' not found in current file\n", function_name);
    }
    
    return 0;
}