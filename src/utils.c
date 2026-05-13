#include "utils.h"
#include <windows.h>
#include <stdio.h>
#include <time.h>

uint32_t utils_get_ms(void) {
    return (uint32_t)GetTickCount();
}

void utils_sleep_ms(uint32_t ms) {
    Sleep(ms);
}

void utils_clear_screen(void) {
    system("cls");
}

void utils_get_timestamp_str(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "[%H:%M:%S]", t);
}
