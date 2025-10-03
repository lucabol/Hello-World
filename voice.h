/* Voice command interface for code editing
 * Provides functionality to parse and execute voice commands
 * for editing hello.c source code
 */
#ifndef VOICE_H
#define VOICE_H

/* Voice command result codes */
typedef enum {
    VOICE_SUCCESS = 0,              /* Command executed successfully */
    VOICE_ERROR_UNKNOWN_COMMAND = 1, /* Command not recognized */
    VOICE_ERROR_INVALID_ARG = 2     /* Command has invalid arguments */
} voice_result_t;

/* Parse and execute a voice command
 * 
 * This function parses the provided command string and executes the
 * corresponding action. It both performs the action (e.g., printing output)
 * and returns a status code indicating success or the type of error.
 * 
 * Parameters:
 *   command - Pointer to a null-terminated command string. Must not be NULL.
 *            The string is not modified and does not need to be freed by
 *            this function. The caller retains ownership.
 * 
 * Returns:
 *   VOICE_SUCCESS (0) - Command was recognized and executed successfully
 *   VOICE_ERROR_UNKNOWN_COMMAND (1) - Command was not recognized
 *   VOICE_ERROR_INVALID_ARG (2) - Command recognized but has invalid arguments
 * 
 * Side effects:
 *   - Prints output to stdout as part of command execution
 *   - Does not modify any source files (all operations are simulated)
 */
int parse_voice_command(const char* command);

#endif /* VOICE_H */
