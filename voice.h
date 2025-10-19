/* Voice Command API for Hello World Program
 * Provides voice-driven code editing capabilities
 * 
 * Features:
 * - Voice command parsing and execution
 * - Read-only operations (no file modifications during demos)
 * - Support for code navigation, inspection, and simulated editing
 * 
 * Supported Commands:
 * - "say hello" - Execute the hello program
 * - "change message <text>" - Simulate changing the greeting message
 * - "show code" - Display the code structure
 * 
 * Error Handling:
 * - Returns 0 on success, 1 on error
 * - Unrecognized commands are reported with appropriate error messages
 */
#ifndef VOICE_H
#define VOICE_H

/* Process a voice command string
 * 
 * Args:
 *   command: The voice command string to process
 * 
 * Returns:
 *   0 on success, 1 on error (invalid command)
 * 
 * Notes:
 *   - Commands are case-sensitive
 *   - All operations are read-only
 *   - Output is written to stdout
 */
int process_voice_command(const char* command);

#endif /* VOICE_H */
