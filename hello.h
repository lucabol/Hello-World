/* hello.h - Header file for hello world program
 * Provides function declarations for greeting functionality
 */
#ifndef HELLO_H
#define HELLO_H

/* Returns a static greeting string
 * Return value: Pointer to a static constant string
 * Memory: Caller must NOT free the returned pointer
 */
const char* get_greeting(void);

/* Prints a custom message to stdout
 * Parameters:
 *   message - The message to print (NULL-safe, will print "(null)" for NULL)
 */
void print_custom_message(const char* message);

#endif /* HELLO_H */
