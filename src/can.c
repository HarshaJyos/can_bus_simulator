#include "can.h"
#include "logger.h"
#include <string.h>
#include <stdio.h>
#include <windows.h>

#define BUS_BUFFER_SIZE 100

static can_frame_t bus_buffer[BUS_BUFFER_SIZE];
static int write_idx = 0;
static int read_idx = 0;
static can_stats_t bus_stats = {0};
static CRITICAL_SECTION bus_lock;

void can_bus_init(void) {
    InitializeCriticalSection(&bus_lock);
    memset(bus_buffer, 0, sizeof(bus_buffer));
    write_idx = 0;
    read_idx = 0;
    memset(&bus_stats, 0, sizeof(bus_stats));
}

bool can_send_frame(can_frame_t frame) {
    EnterCriticalSection(&bus_lock);
    
    /* Error simulation: DLC check */
    if (frame.dlc > 8) {
        printf("[CAN BUS ERROR] Invalid DLC: %d\n", frame.dlc);
        bus_stats.errors++;
        LeaveCriticalSection(&bus_lock);
        return false;
    }

    /* Add to circular buffer */
    bus_buffer[write_idx] = frame;
    write_idx = (write_idx + 1) % BUS_BUFFER_SIZE;
    
    bus_stats.frames_sent++;
    
    /* Log the frame */
    logger_log_frame(frame, "TX");
    
    LeaveCriticalSection(&bus_lock);
    return true;
}

bool can_receive_frame(can_frame_t *frame) {
    bool found = false;
    EnterCriticalSection(&bus_lock);
    
    if (read_idx != write_idx) {
        *frame = bus_buffer[read_idx];
        read_idx = (read_idx + 1) % BUS_BUFFER_SIZE;
        bus_stats.frames_received++;
        found = true;
    }
    
    LeaveCriticalSection(&bus_lock);
    return found;
}

can_stats_t can_get_stats(void) {
    return bus_stats;
}

can_frame_t can_create_frame(uint32_t id, uint8_t dlc, const uint8_t *data) {
    can_frame_t frame;
    frame.id = id;
    frame.dlc = (dlc > 8) ? 8 : dlc;
    memset(frame.data, 0, 8);
    if (data) {
        memcpy(frame.data, data, frame.dlc);
    }
    frame.timestamp = GetTickCount64();
    return frame;
}
