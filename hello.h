/* hello.h - Header file for Hello World program */
#ifndef HELLO_H
#define HELLO_H

/* Returns the greeting message string
 * Returns: Pointer to static string constant "Hello world!"
 * Note: The returned pointer is valid for the lifetime of the program
 */
const char *get_greeting(void);

#endif /* HELLO_H */
