/* hello.h - Header file for hello.c
 * Provides the greeting function for the Hello World program
 */
#ifndef HELLO_H
#define HELLO_H

/* Get the greeting message
 * Returns: A pointer to a static string constant containing the greeting
 * Note: The returned pointer should not be freed by the caller
 */
const char* get_greeting(void);

#endif /* HELLO_H */
