#include "ecu.h"
#include "can.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static float rpm = 800.0f;
static float temp = 20.0f;
static float throttle = 0.0f;
static bool increasing = true;

void engine_ecu_init(void) {
    rpm = 800.0f;
    temp = 20.0f;
    throttle = 0.0f;
}

void engine_ecu_process(void) {
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

    /* Keep values in range */
    if (temp > 100.0f) temp = 100.0f;
    if (throttle > 100.0f) throttle = 100.0f;
    if (throttle < 0.0f) throttle = 0.0f;

    /* Pack into CAN frames */
    
    /* Frame 1: RPM (ID 0x101) - 2 bytes */
    uint16_t rpm_raw = (uint16_t)rpm;
    uint8_t data_rpm[2] = { (uint8_t)(rpm_raw >> 8), (uint8_t)(rpm_raw & 0xFF) };
    can_send_frame(can_create_frame(ECU_ID_ENGINE, 2, data_rpm));

    /* Frame 2: Temp and Throttle (ID 0x102) - 2 bytes */
    uint8_t data_misc[2] = { (uint8_t)temp, (uint8_t)throttle };
    can_send_frame(can_create_frame(ECU_ID_ENGINE + 1, 2, data_misc));
}

void engine_ecu_stop(void) {
    /* Cleanup engine simulation state if needed */
}
