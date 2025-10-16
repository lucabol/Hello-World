#ifndef HELLO_H
#define HELLO_H

/* Get the greeting message
 * Returns a pointer to a static string constant
 * The returned pointer remains valid for the lifetime of the program
 * Caller must NOT free() the returned pointer
 */
const char* get_greeting(void);

/* Print a custom message to stdout
 * msg: The message to print (no newline added)
 */
void print_message(const char* msg);

#endif /* HELLO_H */
