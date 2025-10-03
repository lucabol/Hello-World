/* Voice command interface for code editing
 * Provides functionality to parse and execute voice commands
 * for editing hello.c source code
 */
#ifndef VOICE_H
#define VOICE_H

/* Voice command result codes */
#define VOICE_SUCCESS 0
#define VOICE_ERROR 1

/* Parse and execute a voice command
 * Parameters:
 *   command - The voice command string to execute
 * Returns:
 *   VOICE_SUCCESS (0) on successful command execution
 *   VOICE_ERROR (1) if command is not recognized or fails
 */
int parse_voice_command(const char* command);

#endif /* VOICE_H */
