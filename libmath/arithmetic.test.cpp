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