#include <gtest/gtest.h>
#include <iostream>
#include <libmath/arithmetic.h>
#include <libmath/boolean.h>
#include <limits>

TEST(Arithmetic, sign)
{
    double v1 = 5.0;
    double v1_sign = math::sign(v1);
    EXPECT_EQ(math::isEqual(v1_sign, static_cast<double>(1.0)), true);

    int v2 = -5.0;
    int v2_sign = math::sign(v2);
    EXPECT_EQ(math::isEqual(v2_sign, static_cast<int>(-1.0)), true);

    int v3 = std::numeric_limits<int>::quiet_NaN();
    int v3_sign = math::sign(v3);
    EXPECT_EQ(math::isEqual(v3_sign, static_cast<int>(1.0)), true);
}

TEST(Arithmetic, round)
{
    // round to int
    double v1 = 5.9549649;
    double v1_round = math::round(v1);
    EXPECT_EQ(math::isEqual(v1_round, static_cast<double>(6.0)), true);

    // round with specified floating part numbers
    double v2 = 5.9549649;
    double v2_round = math::round(v2, 3);
    EXPECT_EQ(math::isEqual(v2_round, static_cast<double>(5.955)), true);

    // round with specified floating part numbers (negative number)
    double v3 = -5.9549649;
    double v3_round = math::round(v3, 3);
    EXPECT_EQ(math::isEqual(v3_round, static_cast<double>(-5.955)), true);

    // round with specified floating part numbers (negative number), float
    float v4 = -5.9549649;
    float v4_round = math::round(v4, 3);
    EXPECT_EQ(math::isEqual(v4_round, static_cast<float>(-5.955)), true);

    // round to int
    int v5 = 6;
    int v5_round = math::round(v5, 2);
    EXPECT_EQ(math::isEqual(v5_round, static_cast<int>(6)), true);
}