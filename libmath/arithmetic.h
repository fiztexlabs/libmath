#pragma once
#include <libmath/boolean.h>
#include <cmath>
#include <algorithm>
#include <libmath/math_exception.h>

namespace math
{
    /// @brief Sign of any numeric value
    /// @param value: Value with sign 
    /// @return -1 or 1, casted to T
    template <typename T, class = std::enable_if<isNumeric<T>>>
    T sign(T value)
    {
        // if(std::isnan(value))
        // {
        //     throw(ExceptionInvalidValue("sign: cannot determine sign"))
        // }
        T sign = static_cast<T>(1.0);
        if (isEqual(value, static_cast<T>(0.0)))
        {
            return sign;
        }
        else
        {
            sign = static_cast<T>(value / std::abs(value));
        }

        return sign;
    }

}