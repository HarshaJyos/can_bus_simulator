#include "ecu.h"
#include "can.h"
#include <stdio.h>
#include <stdbool.h>

static bool brake_active = false;
static bool abs_active = false;
static float brake_pressure = 0.0f;
static int counter = 0;

void brake_ecu_init(void) {
    brake_active = false;
    abs_active = false;
    brake_pressure = 0.0f;
}

void brake_ecu_process(void) {
    /* Simulate periodic braking */
    counter++;
    if (counter % 50 == 0) {
        brake_active = !brake_active;
        brake_pressure = brake_active ? 45.5f : 0.0f;
        abs_active = (brake_pressure > 40.0f);
    }

    /* Pack into CAN frame (ID 0x201) */
    /* byte 0: status (bit 0: active, bit 1: ABS) */
    /* byte 1: pressure (0-255 scaled) */
    uint8_t status = 0;
    if (brake_active) status |= 0x01;
    if (abs_active) status |= 0x02;

    uint8_t pressure_raw = (uint8_t)(brake_pressure * 2);
    uint8_t data[2] = { status, pressure_raw };
    
    can_send_frame(can_create_frame(ECU_ID_BRAKE, 2, data));
}

void brake_ecu_stop(void) {
    /* Cleanup brake simulation state if needed */
}
