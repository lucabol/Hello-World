/* hello.h - Header file for hello.c
 * Provides function declarations for testable greeting functionality
 */
#ifndef HELLO_H
#define HELLO_H

/* Get the greeting message as a static string
 * Returns: Pointer to static string constant "Ciao, Mondo!"
 * Note: Do not free the returned pointer
 */
const char* get_greeting(void);

/* Print a custom message to stdout with newline
 * msg: Message to print (NULL safe - prints nothing if NULL)
 */
void print_custom_message(const char* msg);

#endif /* HELLO_H */
