/* hello.h - Header file for Hello World program
 * 
 * This header exposes the greeting functionality for unit testing.
 * When compiled with -DUNIT_TEST, hello.c will provide the get_greeting()
 * function instead of the main() function.
 */
#ifndef HELLO_H
#define HELLO_H

#ifdef __cplusplus
extern "C" {
#endif

/* get_greeting() - Returns the greeting message
 * 
 * Returns: A pointer to a static string constant "Hello world!"
 *          The returned pointer remains valid for the lifetime of the program.
 *          Caller must NOT free() the returned pointer.
 *          The returned string is read-only and should not be modified.
 */
const char* get_greeting(void);

#ifdef __cplusplus
}
#endif

#endif /* HELLO_H */
