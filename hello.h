#ifndef HELLO_H
#define HELLO_H

/* Returns the greeting message
 * Contract:
 * - Returns a pointer to a static string constant
 * - The returned pointer remains valid for the lifetime of the program
 * - Caller must NOT free() the returned pointer
 * - The returned string is read-only and should not be modified
 * - Multiple calls return the same static pointer
 */
const char* get_greeting(void);

/* Prints a custom message
 * If message is NULL, prints a default message
 */
void print_custom_message(const char* message);

#endif /* HELLO_H */
