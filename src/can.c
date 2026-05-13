#include "can.h"
#include "logger.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
    #include <unistd.h>
    #include <net/if.h>
    #include <sys/ioctl.h>
    #include <sys/socket.h>
    #include <linux/can.h>
    #include <linux/can/raw.h>
    #include <fcntl.h>
    static int can_socket = -1;
#else
    #include <windows.h>
    static CRITICAL_SECTION bus_lock;
    #define BUS_BUFFER_SIZE 100
    static can_frame_t bus_buffer[BUS_BUFFER_SIZE];
    static int write_idx = 0;
    static int read_idx = 0;
#endif

static can_stats_t bus_stats = {0};

bool can_bus_init(const char *iface_name) {
#ifdef __linux__
    struct sockaddr_can addr;
    struct ifreq ifr;

    if ((can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("SocketCAN: Error while opening socket");
        return false;
    }

    strcpy(ifr.ifr_name, iface_name);
    ioctl(can_socket, SIOCGIFINDEX, &ifr);

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(can_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("SocketCAN: Error in socket bind");
        return false;
    }

    /* Set non-blocking */
    int flags = fcntl(can_socket, F_GETFL, 0);
    fcntl(can_socket, F_SETFL, flags | O_NONBLOCK);
    
    logger_log_msg("SocketCAN initialized on %s", iface_name);
#else
    InitializeCriticalSection(&bus_lock);
    write_idx = 0;
    read_idx = 0;
    logger_log_msg("Virtual CAN Bus initialized");
#endif
    return true;
}

void can_bus_close(void) {
#ifdef __linux__
    if (can_socket >= 0) close(can_socket);
#else
    DeleteCriticalSection(&bus_lock);
#endif
}

bool can_send_frame(can_frame_t frame) {
#ifdef __linux__
    struct can_frame linux_frame;
    linux_frame.can_id = frame.id;
    linux_frame.can_dlc = frame.dlc;
    memcpy(linux_frame.data, frame.data, 8);

    if (write(can_socket, &linux_frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
        bus_stats.errors++;
        return false;
    }
#else
    EnterCriticalSection(&bus_lock);
    if (frame.dlc > 8) {
        bus_stats.errors++;
        LeaveCriticalSection(&bus_lock);
        return false;
    }
    bus_buffer[write_idx] = frame;
    write_idx = (write_idx + 1) % BUS_BUFFER_SIZE;
    LeaveCriticalSection(&bus_lock);
#endif

    bus_stats.frames_sent++;
    logger_log_frame(frame, "TX");
    return true;
}

bool can_receive_frame(can_frame_t *frame) {
#ifdef __linux__
    struct can_frame linux_frame;
    int nbytes = read(can_socket, &linux_frame, sizeof(struct can_frame));

    if (nbytes < (int)sizeof(struct can_frame)) {
        return false;
    }

    frame->id = linux_frame.can_id;
    frame->dlc = linux_frame.can_dlc;
    memcpy(frame->data, linux_frame.data, 8);
    frame->timestamp = 0; /* Timestamp handling omitted for simplicity */
#else
    bool found = false;
    EnterCriticalSection(&bus_lock);
    if (read_idx != write_idx) {
        *frame = bus_buffer[read_idx];
        read_idx = (read_idx + 1) % BUS_BUFFER_SIZE;
        found = true;
    }
    LeaveCriticalSection(&bus_lock);
    if (!found) return false;
#endif

    bus_stats.frames_received++;
    return true;
}

can_stats_t can_get_stats(void) {
    return bus_stats;
}

can_frame_t can_create_frame(uint32_t id, uint8_t dlc, const uint8_t *data) {
    can_frame_t frame;
    frame.id = id;
    frame.dlc = (dlc > 8) ? 8 : dlc;
    memset(frame.data, 0, 8);
    if (data) memcpy(frame.data, data, frame.dlc);
#ifdef __linux__
    frame.timestamp = 0;
#else
    frame.timestamp = GetTickCount64();
#endif
    return frame;
}
