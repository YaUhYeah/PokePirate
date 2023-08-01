#include "global.h"
#include "test.h"
#include "random.h"

// We expect each element to have an indexSum of 3.5 * 1024.
// Therefore the maximum error is 8*3584, or 28672.
#define SHUFFLE_TEST_IMPL \
    u32 i, j, error; \
    u16 indexSum[7]; \
    memset(indexSum, 0, sizeof(indexSum)); \
    for (i = 0; i < 1024; i++) \
    { \
        Shuffle(array, ARRAY_COUNT(array), sizeof(array[0])); \
        for (j = 0; j < ARRAY_COUNT(array); j++) \
            indexSum[array[j]] += j; \
    } \
    error = 0; \
    for (i = 0; i < ARRAY_COUNT(indexSum); i++) \
        error += abs(3584 - indexSum[i]); \
    EXPECT_LT(error, (int)(28672 * 0.025));

TEST("Shuffle randomizes the array [Shuffle8]")
{
    u8 array[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    SHUFFLE_TEST_IMPL;
}

TEST("Shuffle randomizes the array [Shuffle16]")
{
    u16 array[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    SHUFFLE_TEST_IMPL;
}

TEST("Shuffle randomizes the array [Shuffle32]")
{
    u32 array[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    SHUFFLE_TEST_IMPL;
}

TEST("Shuffle randomizes the array [Shuffle64]")
{
    u64 array[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    SHUFFLE_TEST_IMPL;
}

TEST("RandomUniform generates lo..hi")
{
    u32 lo, hi, i;
    PARAMETRIZE { lo = 0; hi = 1; }
    PARAMETRIZE { lo = 0; hi = 2; }
    PARAMETRIZE { lo = 0; hi = 3; }
    PARAMETRIZE { lo = 2; hi = 4; }
    for (i = 0; i < 1024; i++)
    {
        u32 r = RandomUniformDefault(RNG_NONE, lo, hi);
        EXPECT(lo <= r && r <= hi);
    }
}

TEST("RandomWeighted generates 0..n-1")
{
    u32 n, sum, i;
    static const u8 ws[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };
    PARAMETRIZE { n = 1; }
    PARAMETRIZE { n = 2; }
    PARAMETRIZE { n = 3; }
    PARAMETRIZE { n = 4; }
    ASSUME(n <= ARRAY_COUNT(ws));
    for (i = 0, sum = 0; i < n; i++)
        sum += ws[i];
    for (i = 0; i < 1024; i++)
    {
        u32 r = RandomWeightedArrayDefault(RNG_NONE, sum, n, ws);
        EXPECT(0 <= r && r < n);
    }
}

TEST("RandomElement generates an element")
{
    u32 i;
    static const u8 es[4] = { 1, 2, 4, 8 };
    for (i = 0; i < 1024; i++)
    {
        u32 e = *(const u8 *)RandomElementArrayDefault(RNG_NONE, es, sizeof(es[0]), ARRAY_COUNT(es));
        EXPECT(e == 1 || e == 2 || e == 4 || e == 8);
    }
}

TEST("RandomUniform generates uniform distribution")
{
    u32 i, error;
    u16 distribution[4];

    memset(distribution, 0, sizeof(distribution));
    for (i = 0; i < 4096; i++)
    {
        u32 r = RandomUniformDefault(RNG_NONE, 0, ARRAY_COUNT(distribution) - 1);
        EXPECT(0 <= r && r < ARRAY_COUNT(distribution));
        distribution[r]++;
    }

    error = 0;
    for (i = 0; i < ARRAY_COUNT(distribution); i++)
        error += abs(UQ_4_12(0.25) - distribution[i]);

    EXPECT_LT(error, UQ_4_12(0.025));
}

TEST("RandomWeighted generates distribution in proportion to the weights")
{
    u32 i, sum, error;
    static const u8 ws[4] = { 1, 2, 2, 3 };
    u16 distribution[ARRAY_COUNT(ws)];

    for (i = 0, sum = 0; i < ARRAY_COUNT(ws); i++)
        sum += ws[i];

    memset(distribution, 0, sizeof(distribution));
    for (i = 0; i < 4096; i++)
    {
        u32 r = RandomWeightedArrayDefault(RNG_NONE, sum, ARRAY_COUNT(ws), ws);
        EXPECT(0 <= r && r < ARRAY_COUNT(ws));
        distribution[r]++;
    }

    error = 0;
    error += abs(UQ_4_12(0.125) - distribution[0]);
    error += abs(UQ_4_12(0.250) - distribution[1]);
    error += abs(UQ_4_12(0.250) - distribution[2]);
    error += abs(UQ_4_12(0.375) - distribution[3]);

    EXPECT_LT(error, UQ_4_12(0.025));
}

TEST("RandomElement generates a uniform distribution")
{
    u32 i, error;
    static const u8 es[4] = { 1, 2, 4, 8 };
    u16 distribution[9];

    memset(distribution, 0, sizeof(distribution));
    for (i = 0; i < 4096; i++)
    {
        u32 e = *(const u8 *)RandomElementArrayDefault(RNG_NONE, es, sizeof(es[0]), ARRAY_COUNT(es));
        distribution[e]++;
    }

    error = 0;
    for (i = 0; i < ARRAY_COUNT(es); i++)
        error += abs(UQ_4_12(0.25) - distribution[es[i]]);

    EXPECT_LT(error, UQ_4_12(0.025));
}
