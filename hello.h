/* hello.h - Header file for hello world program
 * Provides functions for greeting message functionality
 */

#ifndef HELLO_H
#define HELLO_H

/* Get the greeting message
 * Returns: A pointer to a static string constant containing the greeting
 * Note: The returned pointer is valid for the lifetime of the program
 *       and should NOT be freed by the caller
 */
const char* get_greeting(void);

/* Print a custom message to stdout
 * Parameters:
 *   message - The message to print (must not be NULL)
 */
void print_custom_message(const char* message);

#endif /* HELLO_H */
