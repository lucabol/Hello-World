/* hello.h - Header file for hello world program
 * Provides functions for greeting message functionality
 */

#ifndef HELLO_H
#define HELLO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Get the greeting message
 * Returns: A pointer to a static string constant containing the greeting.
 *          The string does NOT include a trailing newline; callers should
 *          add one when printing if desired.
 * Note: The returned pointer is valid for the lifetime of the program
 *       and should NOT be freed by the caller.
 */
const char* get_greeting(void);

/* Print a custom message to stdout
 * Parameters:
 *   message - The message to print. If NULL, the function does nothing.
 */
void print_custom_message(const char* message);

#ifdef __cplusplus
}
#endif

#endif /* HELLO_H */

