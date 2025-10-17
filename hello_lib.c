/* hello_lib.c - Hello World Library Implementation
 * 
 * Implements the greeting message API.
 * This file contains only the library logic without I/O or main().
 */

#include "hello.h"

/* Implementation of get_greeting()
 * Returns a static constant string with the greeting message.
 */
const char *get_greeting(void) {
    return "Hello world!";
}
