/* hello.h - Header file for Hello World program
 * Provides greeting functionality for the Hello World application
 */
#ifndef HELLO_H
#define HELLO_H

/* Get the greeting message
 * 
 * Returns a pointer to a static string constant containing the greeting.
 * The returned pointer remains valid for the lifetime of the program.
 * 
 * Contract:
 * - Returns a pointer to a static string constant
 * - The returned pointer remains valid for the lifetime of the program  
 * - Caller must NOT free() the returned pointer
 * - The returned string is read-only and should not be modified
 * - Multiple calls return the same static pointer
 *
 * @return Pointer to the greeting string "Hello world!"
 */
const char* get_greeting(void);

#endif /* HELLO_H */
