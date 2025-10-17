/* Header file for hello.c
 * Provides the greeting function interface for unit testing
 * 
 * Contract for get_greeting():
 * - Returns a pointer to a static string constant
 * - The returned pointer remains valid for the lifetime of the program
 * - Caller must NOT free() the returned pointer
 * - The returned string is read-only and should not be modified
 * - Multiple calls return the same static pointer
 */

#ifndef HELLO_H
#define HELLO_H

/* Get the greeting message
 * Returns: Pointer to static greeting string "Hello world!"
 */
const char* get_greeting(void);

#endif /* HELLO_H */
