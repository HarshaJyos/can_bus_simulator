#include "ecu.h"
#include "can.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static float rpm = 800.0f;
static float temp = 20.0f;
static float throttle = 0.0f;
static bool increasing = true;
static bool running = false;

void engine_ecu_init(void) {
    rpm = 800.0f;
    temp = 20.0f;
    throttle = 0.0f;
    running = true;
}

void* engine_ecu_thread(void* arg) {
    (void)arg;
    while (running) {
        /* Simulate engine dynamics */
        if (increasing) {
            rpm += 50.0f;
            throttle += 1.5f;
            temp += 0.1f;
            if (rpm > 6000.0f) increasing = false;
        } else {
            rpm -= 40.0f;
            throttle -= 1.0f;
            temp -= 0.05f;
            if (rpm < 900.0f) increasing = true;
        }

        if (temp > 100.0f) temp = 100.0f;
        if (throttle > 100.0f) throttle = 100.0f;
        if (throttle < 0.0f) throttle = 0.0f;

        /* Frame 1: RPM */
        uint16_t rpm_raw = (uint16_t)rpm;
        uint8_t data_rpm[2] = { (uint8_t)(rpm_raw >> 8), (uint8_t)(rpm_raw & 0xFF) };
        can_send_frame(can_create_frame(ECU_ID_ENGINE, 2, data_rpm));

        /* Frame 2: Temp and Throttle */
        uint8_t data_misc[2] = { (uint8_t)temp, (uint8_t)throttle };
        can_send_frame(can_create_frame(ECU_ID_ENGINE + 1, 2, data_misc));

        utils_sleep_ms(ENGINE_UPDATE_INTERVAL);
    }
    return NULL;
}

void engine_ecu_stop(void) {
    running = false;
}
