/* hello.h - Header file for Hello World program
 * Provides the greeting message functionality for testing
 */

#ifndef HELLO_H
#define HELLO_H

/* get_greeting - Returns the greeting message
 * 
 * Returns: Pointer to static string constant "Hello world!"
 * 
 * Contract:
 * - Returns a pointer to a static string constant
 * - The returned pointer remains valid for the lifetime of the program
 * - Caller must NOT free() the returned pointer
 * - The returned string is read-only and should not be modified
 * - Multiple calls return the same static pointer
 */
const char* get_greeting(void);

#endif /* HELLO_H */
