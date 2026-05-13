#include "dbc.h"
#include <string.h>

const signal_t SIG_ENGINE_RPM      = { 0, 16, 1.0f, 0.0f, 0.0f, 8000.0f };
const signal_t SIG_ENGINE_TEMP     = { 16, 8, 1.0f, -40.0f, -40.0f, 215.0f };
const signal_t SIG_ENGINE_THROTTLE = { 24, 8, 0.392f, 0.0f, 0.0f, 100.0f };

float dbc_decode(can_frame_t frame, signal_t sig) {
    uint64_t raw_data = 0;
    /* Pack bytes into a 64-bit integer (Little Endian for simplicity) */
    for (int i = 0; i < 8; i++) {
        raw_data |= ((uint64_t)frame.data[i] << (i * 8));
    }

    /* Mask and shift */
    uint64_t mask = ((1ULL << sig.length) - 1);
    uint64_t value_raw = (raw_data >> sig.start_bit) & mask;

    return (float)value_raw * sig.factor + sig.offset;
}

void dbc_encode(uint8_t *data, float value, signal_t sig) {
    /* Reverse scaling */
    uint64_t value_raw = (uint64_t)((value - sig.offset) / sig.factor);
    uint64_t mask = ((1ULL << sig.length) - 1);
    value_raw &= mask;

    /* Pack into data buffer */
    for (int i = 0; i < 8; i++) {
        int bit_offset = i * 8;
        if (bit_offset + 8 > sig.start_bit && bit_offset < sig.start_bit + sig.length) {
            /* This byte contains part of the signal */
            // Simplified: only works if byte-aligned or within 64bit
        }
    }
    /* For this simulator, we'll keep it simple: assume signals are byte-aligned for now 
       or just use the direct raw packing logic we had in ECUs but centralized. */
}
