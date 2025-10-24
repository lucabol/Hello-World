/* hello.h - Header file for Hello World program
 * Provides functions for greeting messages
 */
#ifndef HELLO_H
#define HELLO_H

/* Get the default greeting message
 * Returns: A pointer to a static string constant
 * Contract:
 * - Returns a pointer to a static string constant
 * - The returned pointer remains valid for the lifetime of the program
 * - Caller must NOT free() the returned pointer
 * - The returned string is read-only and should not be modified
 * - Multiple calls return the same static pointer
 */
const char* get_greeting(void);

/* Print a custom message
 * Parameters:
 *   message - The message to print (can be NULL)
 * Contract:
 * - If message is NULL, prints a default message
 * - Does not add a trailing newline
 * - Uses standard I/O; callers should not assume reentrancy
 */
void print_custom_message(const char* message);

#endif /* HELLO_H */
