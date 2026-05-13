#include "logger.h"
#include "utils.h"
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

static FILE *log_file = NULL;
static CRITICAL_SECTION log_lock;

void logger_init(const char *filename) {
    InitializeCriticalSection(&log_lock);
    log_file = fopen(filename, "a");
    if (log_file) {
        logger_log_msg("--- Logger Initialized ---");
    }
}

void logger_log_frame(can_frame_t frame, const char *direction) {
    if (!log_file) return;

    EnterCriticalSection(&log_lock);
    char time_str[32];
    utils_get_timestamp_str(time_str, sizeof(time_str));

    fprintf(log_file, "%s %s ID: 0x%03X DLC: %d Data: ", 
            time_str, direction, frame.id, frame.dlc);
    
    for (int i = 0; i < frame.dlc; i++) {
        fprintf(log_file, "%02X ", frame.data[i]);
    }
    fprintf(log_file, "\n");
    fflush(log_file);
    LeaveCriticalSection(&log_lock);
}

void logger_log_msg(const char *format, ...) {
    if (!log_file) return;

    EnterCriticalSection(&log_lock);
    char time_str[32];
    utils_get_timestamp_str(time_str, sizeof(time_str));
    fprintf(log_file, "%s [INFO] ", time_str);

    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);

    fprintf(log_file, "\n");
    fflush(log_file);
    LeaveCriticalSection(&log_lock);
}

void logger_close(void) {
    if (log_file) {
        logger_log_msg("--- Logger Shutting Down ---");
        fclose(log_file);
        log_file = NULL;
    }
    DeleteCriticalSection(&log_lock);
}
