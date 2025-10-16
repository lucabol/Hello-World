/* hello.h - Public interface for hello library */
#ifndef HELLO_H
#define HELLO_H

/* Returns a pointer to the greeting string
 * The returned string is statically allocated and must NOT be freed
 * Returns: "Hello world!" (no trailing newline)
 */
const char* get_greeting(void);

#endif /* HELLO_H */
