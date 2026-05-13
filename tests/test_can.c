#include "unity_lite.h"
#include "can.h"
#include "dbc.h"
#include <string.h>

static int failures = 0;

int test_frame_creation() {
    uint8_t data[8] = {0x11, 0x22, 0x33};
    can_frame_t frame = can_create_frame(0x123, 3, data);
    TEST_ASSERT(frame.id == 0x123);
    TEST_ASSERT(frame.dlc == 3);
    TEST_ASSERT(frame.data[0] == 0x11);
    return 0;
}

int test_dbc_decoding() {
    can_frame_t frame;
    frame.id = 0x101;
    frame.dlc = 2;
    /* RPM = 1000 (0x03E8) */
    frame.data[0] = 0xE8;
    frame.data[1] = 0x03;
    
    float rpm = dbc_decode(frame, SIG_ENGINE_RPM);
    TEST_ASSERT(rpm == 1000.0f);
    return 0;
}

int main() {
    printf("Starting Unit Tests...\n");
    RUN_TEST(test_frame_creation);
    RUN_TEST(test_dbc_decoding);
    
    if (failures == 0) {
        printf("\nALL TESTS PASSED!\n");
    } else {
        printf("\n%d TESTS FAILED.\n", failures);
    }
    return failures;
}
