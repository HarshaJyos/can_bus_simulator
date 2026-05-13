#ifndef CAN_H
#define CAN_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief CAN Frame Structure
 * Represents a standard CAN 2.0B frame.
 */
typedef struct {
    uint32_t id;          /* 11-bit or 29-bit identifier */
    uint8_t  dlc;         /* Data Length Code (0-8) */
    uint8_t  data[8];     /* Payload */
    uint64_t timestamp;   /* Microseconds since start */
} can_frame_t;

/**
 * @brief Virtual CAN Bus statistics
 */
typedef struct {
    uint32_t frames_sent;
    uint32_t frames_received;
    uint32_t errors;
} can_stats_t;

/* CAN Bus Initialization */
void can_bus_init(void);

/* Send a frame to the virtual bus */
bool can_send_frame(can_frame_t frame);

/* Receive a frame from the virtual bus (non-blocking) */
bool can_receive_frame(can_frame_t *frame);

/* Get bus statistics */
can_stats_t can_get_stats(void);

/* Helper to create a frame */
can_frame_t can_create_frame(uint32_t id, uint8_t dlc, const uint8_t *data);

#endif /* CAN_H */
