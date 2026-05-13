#ifndef DBC_H
#define DBC_H

#include "can.h"

/**
 * @brief Signal definition (based on DBC standard)
 */
typedef struct {
    uint8_t start_bit;
    uint8_t length;
    float   factor;
    float   offset;
    float   min;
    float   max;
} signal_t;

/* Database of signals */
extern const signal_t SIG_ENGINE_RPM;
extern const signal_t SIG_ENGINE_TEMP;
extern const signal_t SIG_ENGINE_THROTTLE;

/* Decode a signal from a frame */
float dbc_decode(can_frame_t frame, signal_t sig);

/* Encode a value into a frame data buffer */
void dbc_encode(uint8_t *data, float value, signal_t sig);

#endif /* DBC_H */
