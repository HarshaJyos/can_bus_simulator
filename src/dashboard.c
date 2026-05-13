#include "dashboard.h"
#include "can.h"
#include "ecu.h"
#include "utils.h"
#include <stdio.h>

static vehicle_data_t live_data = {0};

void dashboard_init(void) {
    live_data.rpm = 0;
    live_data.temp = 0;
    live_data.throttle = 0;
    live_data.brake_active = false;
    live_data.abs_active = false;
    live_data.brake_pressure = 0;
}

void dashboard_update(can_frame_t frame) {
    switch (frame.id) {
        case ECU_ID_ENGINE:
            live_data.rpm = (float)((frame.data[0] << 8) | frame.data[1]);
            break;
        case ECU_ID_ENGINE + 1:
            live_data.temp = (float)frame.data[0];
            live_data.throttle = (float)frame.data[1];
            break;
        case ECU_ID_BRAKE:
            live_data.brake_active = (frame.data[0] & 0x01) != 0;
            live_data.abs_active = (frame.data[0] & 0x02) != 0;
            live_data.brake_pressure = (float)frame.data[1] / 2.0f;
            break;
        default:
            /* Ignore other IDs or handle OBD responses if needed */
            break;
    }
}

void dashboard_render(void) {
    utils_clear_screen();
    printf("====================================================\n");
    printf("           VIRTUAL AUTOMOTIVE DASHBOARD             \n");
    printf("====================================================\n\n");
    
    printf("  [ENGINE STATUS]\n");
    printf("  RPM      : %6.1f RPM  ", live_data.rpm);
    /* Simple ASCII Bar for RPM */
    printf("[");
    int bars = (int)(live_data.rpm / 600);
    for(int i=0; i<10; i++) printf(i < bars ? "#" : " ");
    printf("]\n");

    printf("  TEMP     : %6.1f *C   ", live_data.temp);
    printf("  THROTTLE : %6.1f %%\n\n", live_data.throttle);

    printf("  [BRAKE SYSTEM]\n");
    printf("  STATUS   : %s\n", live_data.brake_active ? "DEPRESSED" : "RELEASED ");
    printf("  ABS      : %s\n", live_data.abs_active ? "ACTIVE   " : "INACTIVE ");
    printf("  PRESSURE : %6.1f PSI\n\n", live_data.brake_pressure);

    printf("====================================================\n");
    printf("  Press Ctrl+C to stop simulation and return to menu\n");
    printf("====================================================\n");
}
