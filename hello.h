#ifndef HELLO_H
#define HELLO_H

/* Function to get the greeting message
 * Returns: pointer to a static string constant "Hello world!"
 * Note: The returned pointer is valid for the lifetime of the program
 *       and should NOT be freed by the caller
 */
const char* get_greeting(void);

#endif /* HELLO_H */
