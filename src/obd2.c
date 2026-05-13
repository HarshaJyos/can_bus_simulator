#include "obd2.h"
#include "can.h"
#include "ecu.h"
#include <stdio.h>
#include <string.h>

void obd2_init(void) {
    /* No specific state needed for this simple simulator */
}

void obd2_process_request(can_frame_t frame) {
    if (frame.id != ECU_ID_OBD2_REQ) return;

    /* OBD-II Request format: byte 0=Len, byte 1=Mode, byte 2=PID */
    uint8_t mode = frame.data[1];
    uint8_t pid = frame.data[2];

    if (mode == MODE_SHOW_CURRENT_DATA) {
        can_frame_t response;
        response.id = ECU_ID_OBD2_RES;
        response.dlc = 8;
        memset(response.data, 0, 8);
        response.data[0] = 3; /* Length of useful data (Mode + PID + Val) */
        response.data[1] = mode + 0x40; /* Response mode */
        response.data[2] = pid;

        /* Provide dummy/static values for OBD-II simulation */
        switch (pid) {
            case PID_ENGINE_RPM:
                /* 700 RPM fixed for OBD test */
                response.data[3] = (uint8_t)(700 >> 8);
                response.data[4] = (uint8_t)(700 & 0xFF);
                response.data[0] = 4;
                break;
            case PID_COOLANT_TEMP:
                response.data[3] = 90 + 40; /* Value = Temp + 40 */
                break;
            case PID_VEHICLE_SPEED:
                response.data[3] = 65; /* 65 km/h */
                break;
            default:
                response.data[1] = 0x7F; /* Negative response */
                break;
        }
        
        can_send_frame(response);
    }
}

can_frame_t obd2_create_request(uint8_t pid) {
    uint8_t data[8] = {0x02, MODE_SHOW_CURRENT_DATA, pid, 0, 0, 0, 0, 0};
    return can_create_frame(ECU_ID_OBD2_REQ, 8, data);
}
