#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "can.h"

/**
 * @brief Vehicle Live Data structure for Dashboard
 */
typedef struct {
    float rpm;
    float temp;
    float throttle;
    bool brake_active;
    bool abs_active;
    float brake_pressure;
} vehicle_data_t;

/* Initialize Dashboard */
void dashboard_init(void);

/* Update dashboard data based on received CAN frame */
void dashboard_update(can_frame_t frame);

/* Render the dashboard UI in terminal */
void dashboard_render(void);

#endif /* DASHBOARD_H */
