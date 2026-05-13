#include "can.h"
#include "logger.h"
#include "menu.h"
#include "obd2.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Main Entry Point (Upgraded)
 */
int main(void) {
    /* Initialize Virtual Hardware / SocketCAN */
    /* Use "vcan0" if on Linux, "sim" if on Windows */
    #ifdef __linux__
        can_bus_init("vcan0");
    #else
        can_bus_init("sim");
    #endif
    
    /* Ensure logs directory exists */
    #ifdef _WIN32
        system("if not exist logs mkdir logs");
    #else
        system("mkdir -p logs");
    #endif

    logger_init("logs/can_bus.log");
    obd2_init();

    logger_log_msg("Advanced CAN Bus Simulator (Multi-threaded) started.");

    /* Launch Menu System */
    menu_show_main();

    /* Cleanup */
    can_bus_close();
    logger_close();
    printf("\nSystem Shutdown Cleanly.\n");

    return 0;
}
