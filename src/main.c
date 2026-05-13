#include "can.h"
#include "logger.h"
#include "menu.h"
#include "obd2.h"
#include <stdio.h>
#include <windows.h>

/**
 * @brief Main Entry Point
 * Initializes all systems and launches the menu.
 */
int main(void) {
    /* Initialize Virtual Hardware */
    can_bus_init();
    
    /* Initialize Logger (Ensure logs directory exists) */
    CreateDirectory("logs", NULL);
    logger_init("logs/can_bus.log");
    
    /* Initialize OBD-II Module */
    obd2_init();

    /* System Info */
    logger_log_msg("Virtual CAN Bus Simulator started successfully.");

    /* Start Menu Loop */
    menu_show_main();

    /* Cleanup and Shutdown */
    logger_close();
    printf("\nSystem Shutdown. Goodbye!\n");

    return 0;
}
