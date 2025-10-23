/* hello.h - Header for hello.c greeting functions
 * Provides testable greeting functionality
 */
#ifndef HELLO_H
#define HELLO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Get the greeting message
 * Returns: A pointer to a static string constant containing the greeting
 * Note: The returned pointer remains valid for the program lifetime
 *       Caller must NOT free() the returned pointer
 */
const char* get_greeting(void);

/* Print a custom message to stdout
 * Parameters:
 *   message - The message to print (NULL-safe)
 * Returns: void
 * Note: If message is NULL, prints a default message instead
 */
void print_custom_message(const char* message);

#ifdef __cplusplus
}
#endif

#endif /* HELLO_H */
