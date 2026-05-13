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
#include <windows.h>

/* Flag to stop simulation loop */
static volatile bool sim_running = false;

/* Simple signal handler simulation for Ctrl+C */
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
        printf("      VIRTUAL AUTOMOTIVE CAN BUS SIMULATOR          \n");
        printf("====================================================\n");
        printf(" 1. Start Simulation (Engine + Brake + Dash)\n");
        printf(" 2. Send Custom CAN Frame\n");
        printf(" 3. View CAN Bus Logs\n");
        printf(" 4. Run OBD-II Diagnostics Test\n");
        printf(" 5. Exit\n");
        printf("====================================================\n");
        printf(" Select an option: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); /* clear buffer */
            continue;
        }

        switch (choice) {
            case 1: menu_run_simulation(); break;
            case 2: {
                uint32_t id;
                int dlc;
                if (scanf("%x", &id) != 1) { while (getchar() != '\n'); break; }
                printf("Enter DLC (0-8): ");
                if (scanf("%d", &dlc) != 1) { while (getchar() != '\n'); break; }
                uint8_t data[8] = {0};
                printf("Enter %d bytes (hex): ", dlc);
                for(int i=0; i<dlc; i++) {
                    int val;
                    if (scanf("%x", &val) != 1) break;
                    data[i] = (uint8_t)val;
                }
                while (getchar() != '\n'); /* clear any remaining input */
                can_send_frame(can_create_frame(id, dlc, data));
                printf("Frame sent! Press Enter to continue...");
                getchar(); getchar();
                break;
            }
            case 3: menu_view_logs(); break;
            case 4: menu_run_diagnostics(); break;
            case 5: return;
            default: break;
        }
    }
}

void menu_run_simulation(void) {
    sim_running = true;
    SetConsoleCtrlHandler(console_handler, TRUE);
    
    engine_ecu_init();
    brake_ecu_init();
    dashboard_init();
    
    uint32_t last_engine_tick = 0;
    uint32_t last_brake_tick = 0;
    uint32_t last_dash_tick = 0;
    
    printf("Simulation Starting. Use Ctrl+C to return to menu.\n");
    utils_sleep_ms(1000);

    while (sim_running) {
        uint32_t now = utils_get_ms();

        /* Process Engine ECU */
        if (now - last_engine_tick >= ENGINE_UPDATE_INTERVAL) {
            engine_ecu_process();
            last_engine_tick = now;
        }

        /* Process Brake ECU */
        if (now - last_brake_tick >= BRAKE_UPDATE_INTERVAL) {
            brake_ecu_process();
            last_brake_tick = now;
        }

        /* Dashboard and OBD-II Consumer */
        can_frame_t frame;
        while (can_receive_frame(&frame)) {
            dashboard_update(frame);
            obd2_process_request(frame); // Ensure OBD-II module listens
        }

        /* Update UI */
        if (now - last_dash_tick >= DASHBOARD_UPDATE_INTERVAL) {
            dashboard_render();
            last_dash_tick = now;
        }

        utils_sleep_ms(10); /* Low CPU usage */
    }
    
    SetConsoleCtrlHandler(console_handler, FALSE);
    printf("\nSimulation Stopped. Press Enter...");
    getchar(); getchar();
}

void menu_run_diagnostics(void) {
    printf("\nRunning OBD-II Diagnostics Test...\n");
    
    /* Send RPM request */
    can_frame_t req = obd2_create_request(PID_ENGINE_RPM);
    can_send_frame(req);
    
    /* Manually trigger the OBD-II responder for this request 
       (Simulating the ECU receiving the message immediately) */
    obd2_process_request(req);
    
    /* Wait a moment for the "bus" to stabilize */
    utils_sleep_ms(100);
    
    can_frame_t frame;
    bool response_found = false;
    while (can_receive_frame(&frame)) {
        if (frame.id == ECU_ID_OBD2_RES) {
            response_found = true;
            printf("[OBD-II RES] Received Response for PID 0x%02X\n", frame.data[2]);
            if (frame.data[2] == PID_ENGINE_RPM) { // Check PID
                uint16_t rpm = (frame.data[3] << 8) | frame.data[4];
                printf(" >> Calculated RPM: %d\n", rpm);
            }
        }
    }

    if (!response_found) {
        printf("[!] No OBD-II Response detected on the bus.\n");
    }
    
    printf("\nDiagnostics Complete. Press Enter...");
    getchar(); getchar();
}

void menu_view_logs(void) {
    printf("\n--- CAN BUS LOGS (Last 20 lines) ---\n");
    /* In a real app we'd read the file, here we'll just advise checking the file */
    printf("Please check 'logs/can_bus.log' for detailed history.\n");
    printf("\nPress Enter to return...");
    getchar(); getchar();
}
