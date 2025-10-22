/* Example plugin declarations
 * Include this header to use the example plugins
 */

#ifndef EXAMPLE_PLUGINS_H
#define EXAMPLE_PLUGINS_H

/* Uppercase Plugin - converts message to uppercase */
const char* uppercase_plugin(const char* input);

/* Reverse Plugin - reverses the message */
const char* reverse_plugin(const char* input);

/* Exclaim Plugin - adds extra exclamation marks */
const char* exclaim_plugin(const char* input);

#endif /* EXAMPLE_PLUGINS_H */
