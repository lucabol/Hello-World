/* hello.h - Public API for Hello World program
 * 
 * This header exposes the greeting functionality for unit testing
 * while keeping the main program implementation in hello.c
 */
#ifndef HELLO_H
#define HELLO_H

/* Get the default greeting message
 * 
 * Returns: A pointer to a static string constant containing the greeting
 * 
 * Contract:
 * - Returns a pointer to a static string constant
 * - The returned pointer remains valid for the lifetime of the program
 * - Caller must NOT free() the returned pointer
 * - The returned string is read-only and should not be modified
 * - Multiple calls return the same static pointer
 */
const char* get_greeting(void);

/* Print a custom message without trailing newline
 * 
 * Prints the given message to stdout without adding a trailing newline.
 * This maintains the program's contract of producing output with no trailing newline.
 * 
 * Parameters:
 * - message: The message to print (must not be NULL)
 */
void print_custom_message(const char* message);

#endif /* HELLO_H */
