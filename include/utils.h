#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

/* Get current time in milliseconds */
uint32_t utils_get_ms(void);

/* Sleep for specified milliseconds */
void utils_sleep_ms(uint32_t ms);

/* Clear terminal screen */
void utils_clear_screen(void);

/* Get formatted timestamp string */
void utils_get_timestamp_str(char *buffer, size_t size);

#endif /* UTILS_H */
