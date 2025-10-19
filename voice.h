/* voice.h - Voice-driven code editing interface
 * Provides API for processing voice commands to edit and navigate code
 */
#ifndef VOICE_H
#define VOICE_H

/* Voice command result codes */
#define VOICE_CMD_SUCCESS 0
#define VOICE_CMD_ERROR 1

/* Process a voice command string
 * 
 * Supported commands:
 * - "say hello" - Execute the hello program and display output
 * - "change message <new_text>" - Simulate changing the greeting message
 * - "show code" - Display information about the current code structure
 * 
 * Parameters:
 *   command: The voice command string to process
 * 
 * Returns:
 *   VOICE_CMD_SUCCESS (0) if command was recognized and executed
 *   VOICE_CMD_ERROR (1) if command was not recognized
 */
int process_voice_command(const char* command);

#endif /* VOICE_H */
