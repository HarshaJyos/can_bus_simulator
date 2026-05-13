#ifndef ECU_H
#define ECU_H

#include "can.h"
#include <pthread.h>

/* ECU Identifiers (CAN IDs) */
#define ECU_ID_ENGINE    0x101
#define ECU_ID_BRAKE     0x201
#define ECU_ID_DASHBOARD 0x301
#define ECU_ID_OBD2_REQ  0x7DF
#define ECU_ID_OBD2_RES  0x7E8

/* Simulated Data update intervals (ms) */
#define ENGINE_UPDATE_INTERVAL    100
#define BRAKE_UPDATE_INTERVAL     200
#define DASHBOARD_UPDATE_INTERVAL 500

/**
 * @brief Common ECU State
 */
typedef struct {
    bool is_running;
    uint32_t last_update_time;
    pthread_t thread_id;
    pthread_mutex_t lock;
} ecu_state_t;

/* Engine ECU Functions */
void* engine_ecu_thread(void* arg);
void engine_ecu_init(void);
void engine_ecu_stop(void);

/* Brake ECU Functions */
void* brake_ecu_thread(void* arg);
void brake_ecu_init(void);
void brake_ecu_stop(void);

#endif /* ECU_H */
