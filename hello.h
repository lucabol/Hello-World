/* Header file for Hello World program
 * Provides the greeting function declaration
 */
#ifndef HELLO_H
#define HELLO_H

/* Returns a pointer to the greeting string
 * The returned pointer is to static storage and remains valid
 * for the lifetime of the program. Caller should NOT free() it.
 */
const char* get_greeting(void);

#endif /* HELLO_H */
