/* Header file for hello.c - greeting functionality */
#ifndef HELLO_H
#define HELLO_H

/* Get the default greeting message
 * Returns a pointer to a static string constant
 * The returned pointer remains valid for the lifetime of the program */
const char* get_greeting(void);

/* Print a custom message
 * message: The message to print (must not be NULL) */
void print_custom_message(const char* message);

#endif /* HELLO_H */
