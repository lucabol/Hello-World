#ifndef VOICE_H
#define VOICE_H

/**
 * Process voice commands for code editing simulation
 * 
 * This function simulates how voice recognition would process spoken commands
 * for code editing operations. It only simulates changes and does not modify
 * any source files or persistent state.
 *
 * @param command Null-terminated string containing the voice command
 *                Expected format: "say hello", "change message <text>", "show code"
 *                Input should be reasonable length (< 1024 chars) for safety
 * 
 * @return 0 on success (recognized command), 1 on failure (unrecognized command or error)
 * 
 * Supported commands:
 * - "say hello" - Execute the main greeting functionality
 * - "change message <text>" - Simulate changing output message (display only)
 * - "show code" - Display current code structure for navigation
 * - Any other input - Reports unrecognized command
 */
int process_voice_command(const char* command);

#endif /* VOICE_H */