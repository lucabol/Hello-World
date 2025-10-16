/* Hello World Program Header
 * Provides the greeting message for the hello world program
 */
#ifndef HELLO_H
#define HELLO_H

/* Returns the greeting message as a static string constant
 * Contract:
 * - Returns a pointer to a static string constant
 * - The returned pointer remains valid for the lifetime of the program
 * - Caller must NOT free() the returned pointer
 * - The returned string is read-only and should not be modified
 * - Multiple calls return the same static pointer
 */
const char* get_greeting(void);

#endif /* HELLO_H */
