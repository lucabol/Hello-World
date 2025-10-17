#ifndef HELLO_H
#define HELLO_H

/* Get the greeting message
 * Returns a pointer to a static string constant
 * The returned pointer remains valid for the lifetime of the program
 * Caller must NOT free() the returned pointer
 */
const char* get_greeting(void);

#endif /* HELLO_H */
