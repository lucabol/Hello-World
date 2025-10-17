/* hello.h - Header for hello.c greeting functionality
 * Provides a simple greeting message function for unit testing
 */
#ifndef HELLO_H
#define HELLO_H

/* Returns a pointer to the greeting string "Hello world!"
 * The returned pointer is to a static string constant
 * Caller must NOT free() the returned pointer
 */
const char* get_greeting(void);

#endif /* HELLO_H */
