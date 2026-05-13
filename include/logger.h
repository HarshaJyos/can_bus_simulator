#ifndef LOGGER_H
#define LOGGER_H

#include "can.h"

/* Logger Initialization */
void logger_init(const char *filename);

/* Log a single CAN frame */
void logger_log_frame(can_frame_t frame, const char *direction);

/* Log a general message */
void logger_log_msg(const char *format, ...);

/* Close the log file */
void logger_close(void);

#endif /* LOGGER_H */
