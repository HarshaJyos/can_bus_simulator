#include "ecu.h"
#include "can.h"
#include "utils.h"
#include <stdio.h>
#include <stdbool.h>

static bool brake_active = false;
static bool abs_active = false;
static float brake_pressure = 0.0f;
static int counter = 0;
static bool running = false;

void brake_ecu_init(void) {
    brake_active = false;
    abs_active = false;
    brake_pressure = 0.0f;
    running = true;
}

void* brake_ecu_thread(void* arg) {
    (void)arg;
    while (running) {
        counter++;
        if (counter % 50 == 0) {
            brake_active = !brake_active;
            brake_pressure = brake_active ? 45.5f : 0.0f;
            abs_active = (brake_pressure > 40.0f);
        }

        uint8_t status = 0;
        if (brake_active) status |= 0x01;
        if (abs_active) status |= 0x02;

        uint8_t pressure_raw = (uint8_t)(brake_pressure * 2);
        uint8_t data[2] = { status, pressure_raw };
        
        can_send_frame(can_create_frame(ECU_ID_BRAKE, 2, data));
        
        utils_sleep_ms(BRAKE_UPDATE_INTERVAL);
    }
    return NULL;
}

void brake_ecu_stop(void) {
    running = false;
}
