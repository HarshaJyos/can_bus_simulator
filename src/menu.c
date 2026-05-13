#include "menu.h"
#include "can.h"
#include "ecu.h"
#include "dashboard.h"
#include "obd2.h"
#include "utils.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <windows.h>

static volatile bool sim_running = false;
static pthread_t engine_tid, brake_tid;

static BOOL WINAPI console_handler(DWORD signal) {
    if (signal == CTRL_C_EVENT) {
        sim_running = false;
        return TRUE;
    }
    return FALSE;
}

void menu_show_main(void) {
    int choice;
    while (1) {
        utils_clear_screen();
        printf("====================================================\n");
        printf("      ADVANCED AUTOMOTIVE CAN BUS SIMULATOR         \n");
        printf("====================================================\n");
        printf(" 1. Start Multi-threaded Simulation\n");
        printf(" 2. Send Custom CAN Frame\n");
        printf(" 3. View CAN Bus Logs\n");
        printf(" 4. Run OBD-II Diagnostics Test\n");
        printf(" 5. Exit\n");
        printf("====================================================\n");
        printf(" Select an option: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: menu_run_simulation(); break;
            case 2: {
                uint32_t id; int dlc;
                printf("Enter CAN ID (hex): ");
                if (scanf("%x", &id) != 1) { while (getchar() != '\n'); break; }
                printf("Enter DLC: ");
                if (scanf("%d", &dlc) != 1) { while (getchar() != '\n'); break; }
                uint8_t data[8] = {0};
                printf("Enter %d bytes (hex): ", dlc);
                for(int i=0; i<dlc; i++) {
                    int val; scanf("%x", &val); data[i] = (uint8_t)val;
                }
                while (getchar() != '\n');
                can_send_frame(can_create_frame(id, dlc, data));
                break;
            }
            case 3: menu_view_logs(); break;
            case 4: menu_run_diagnostics(); break;
            case 5: return;
        }
    }
}

void menu_run_simulation(void) {
    sim_running = true;
    SetConsoleCtrlHandler(console_handler, TRUE);
    
    engine_ecu_init();
    brake_ecu_init();
    dashboard_init();
    
    /* Launch ECU Threads */
    pthread_create(&engine_tid, NULL, engine_ecu_thread, NULL);
    pthread_create(&brake_tid, NULL, brake_ecu_thread, NULL);
    
    printf("Threads Launched. Simulation Active. Use Ctrl+C to stop.\n");

    while (sim_running) {
        uint32_t now = utils_get_ms();

        /* Process Incoming frames in main loop (Consumer) */
        can_frame_t frame;
        while (can_receive_frame(&frame)) {
            dashboard_update(frame);
            obd2_process_request(frame);
        }

        dashboard_render();
        utils_sleep_ms(200); /* UI refresh rate */
    }
    
    /* Shutdown Threads */
    engine_ecu_stop();
    brake_ecu_stop();
    pthread_join(engine_tid, NULL);
    pthread_join(brake_tid, NULL);
    
    SetConsoleCtrlHandler(console_handler, FALSE);
    printf("\nSimulation Stopped. Press Enter...");
    getchar(); getchar();
}

void menu_run_diagnostics(void) {
    printf("\nRunning OBD-II Diagnostics Test...\n");
    can_frame_t req = obd2_create_request(PID_ENGINE_RPM);
    can_send_frame(req);
    
    /* Manually process to simulate ECU response */
    obd2_process_request(req);
    
    utils_sleep_ms(100);
    can_frame_t frame;
    while (can_receive_frame(&frame)) {
        if (frame.id == ECU_ID_OBD2_RES) {
            if (frame.data[2] == PID_ENGINE_RPM) {
                uint16_t rpm = (frame.data[3] << 8) | frame.data[4];
                printf("[OBD-II RES] Calculated RPM: %d\n", rpm);
            }
        }
    }
    printf("\nPress Enter...");
    getchar(); getchar();
}

void menu_view_logs(void) {
    printf("\nCheck 'logs/can_bus.log' for details.\nPress Enter...");
    getchar(); getchar();
}
