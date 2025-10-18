/* hello.h - Hello World program interface
 * Provides greeting functionality for testing and reuse
 */
#ifndef HELLO_H
#define HELLO_H

/* Get the greeting message
 * Returns: Pointer to a static string constant "Hello world!"
 * Note: The returned pointer is valid for the lifetime of the program
 *       and should NOT be freed by the caller
 */
const char* get_greeting(void);

#endif /* HELLO_H */
