/* hello.h - Header file for Hello World program
 * Provides the greeting message functionality
 */
#ifndef HELLO_H
#define HELLO_H

/* get_greeting - Returns the greeting message
 * 
 * Returns: A pointer to a static string constant containing the greeting
 *          The returned pointer remains valid for the lifetime of the program
 *          Caller must NOT free() the returned pointer
 *          The returned string is read-only and should not be modified
 */
const char* get_greeting(void);

#endif /* HELLO_H */
