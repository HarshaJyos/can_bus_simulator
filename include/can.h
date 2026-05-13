#ifndef CAN_H
#define CAN_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief CAN Frame Structure
 * Standardized for both SocketCAN and Virtual Bus.
 */
typedef struct {
    uint32_t id;          /* 11-bit or 29-bit identifier */
    uint8_t  dlc;         /* Data Length Code (0-8) */
    uint8_t  data[8];     /* Payload */
    uint64_t timestamp;   /* Microseconds since start */
} can_frame_t;

/**
 * @brief CAN Bus statistics
 */
typedef struct {
    uint32_t frames_sent;
    uint32_t frames_received;
    uint32_t errors;
} can_stats_t;

/* CAN Bus Lifecycle */
bool can_bus_init(const char *iface_name); /* "vcan0" for Linux, "sim" for Windows */
void can_bus_close(void);

/* Send/Receive */
bool can_send_frame(can_frame_t frame);
bool can_receive_frame(can_frame_t *frame); /* Non-blocking */

/* Diagnostics */
can_stats_t can_get_stats(void);
can_frame_t can_create_frame(uint32_t id, uint8_t dlc, const uint8_t *data);

#endif /* CAN_H */
