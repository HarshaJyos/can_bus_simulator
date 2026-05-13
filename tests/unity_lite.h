#ifndef UNITY_LITE_H
#define UNITY_LITE_H

#include <stdio.h>

#define TEST_ASSERT(cond) \
    if (!(cond)) { printf("  [FAIL] %s:%d\n", __FILE__, __LINE__); return 1; }

#define RUN_TEST(func) \
    printf("Running %s...\n", #func); \
    if (func() == 0) { printf("  [PASS]\n"); } else { failures++; }

#endif
