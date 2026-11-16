/* Copyright (c) 2026 Mbed CE Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstdint>
#include <cstring>
#include "gtest/gtest.h"
#include "mbed_trng.h"

extern "C" uint32_t arm_random_seed_get(void);

static unsigned poll_calls;
static const uint32_t test_seed = 0x12345678;

extern "C" int mbedtls_hardware_poll(void *, unsigned char *output, size_t len, size_t *olen)
{
    ++poll_calls;
    EXPECT_EQ(sizeof(test_seed), len);
    if (len != sizeof(test_seed)) {
        return -1;
    }
    memcpy(output, &test_seed, sizeof(test_seed));
    *olen = sizeof(test_seed);
    return 0;
}

TEST(RandlibSeed, UsesConfiguredEntropySource)
{
    poll_calls = 0;
#if TEST_HARDWARE_ENTROPY
    EXPECT_EQ(test_seed, arm_random_seed_get());
    EXPECT_EQ(1U, poll_calls);
#else
    EXPECT_EQ(0U, arm_random_seed_get());
    EXPECT_EQ(0U, poll_calls);
#endif
}
