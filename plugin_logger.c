/* Example Plugin: Logger
 * Demonstrates using only hooks without message transformation
 * This plugin logs execution without modifying the message
 */

#include "plugin.h"
#include <stdio.h>
#include <time.h>

/* Before hook - logs timestamp before execution */
static void logger_before(void) {
    time_t now = time(NULL);
    printf("[%s] Starting hello.c execution\n", ctime(&now));
}

/* After hook - logs completion */
static void logger_after(void) {
    printf("\n[INFO] Execution completed successfully\n");
}

/* Register the plugin - no transform function needed (pass NULL) */
PLUGIN_REGISTER(LoggerPlugin, NULL, logger_before, logger_after);
