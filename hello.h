/* Hello World Program Header
 * 
 * This header exposes the core functionality of the Hello World program
 * for use by unit tests and plugins.
 */

#ifndef HELLO_H
#define HELLO_H

/* Get the greeting message
 * Returns: A pointer to a static string constant containing the greeting
 * Note: The returned pointer should NOT be freed by the caller
 */
const char* get_greeting(void);

#endif /* HELLO_H */
