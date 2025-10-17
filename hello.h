/* hello.h - Hello World Program Header
 * Provides greeting functionality with customizable messages
 */
#ifndef HELLO_H
#define HELLO_H

/* Print a custom message to stdout
 * Parameters:
 *   message - The message to print (must not be NULL)
 * Behavior:
 *   Prints the message exactly as provided, without adding newline
 */
void print_message(const char* message);

/* Get the default greeting message
 * Returns:
 *   Pointer to static string constant "Hello world!"
 * Contract:
 *   - Returned pointer remains valid for program lifetime
 *   - Caller must NOT free() the returned pointer
 *   - Returned string is read-only
 */
const char* get_greeting(void);

#endif /* HELLO_H */
