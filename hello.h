/* hello.h - Header file for hello.c
 * Declares the greeting function for unit testing
 */
#ifndef HELLO_H
#define HELLO_H

/* Get the greeting message
 * Returns: A pointer to a static string constant "Hello world!"
 * Note: The returned pointer remains valid for the program lifetime
 *       Caller must NOT free() the returned pointer
 */
const char* get_greeting(void);

#endif /* HELLO_H */
