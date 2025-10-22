/* hello.h - Header file for hello.c
 * Provides the greeting function interface for unit testing
 */
#ifndef HELLO_H
#define HELLO_H

/* Returns a pointer to a static string containing the greeting message
 * The returned pointer remains valid for the lifetime of the program
 * Caller must NOT free() the returned pointer
 */
const char* get_greeting(void);

#endif /* HELLO_H */
