#ifndef OBD2_H
#define OBD2_H

#include "can.h"

/* OBD-II PIDs (Parameter IDs) */
#define PID_ENGINE_LOAD       0x04
#define PID_COOLANT_TEMP      0x05
#define PID_ENGINE_RPM        0x0C
#define PID_VEHICLE_SPEED     0x0D
#define PID_THROTTLE_POS      0x11

/* OBD-II Modes */
#define MODE_SHOW_CURRENT_DATA 0x01

/* OBD-II Response offset */
#define OBD2_RESPONSE_ID_OFFSET 0x08

/* Initialize OBD-II Module */
void obd2_init(void);

/* Process incoming OBD-II requests */
void obd2_process_request(can_frame_t frame);

/* Generate an OBD-II request frame */
can_frame_t obd2_create_request(uint8_t pid);

#endif /* OBD2_H */
