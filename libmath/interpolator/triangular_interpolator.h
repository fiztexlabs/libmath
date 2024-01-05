#pragma once
#include <libmath/interpolator/interpolator.h>

namespace math
{
    template <typename T>
    class TriangularInterpolator : public Interpolator<T>
    {
    public:
        TriangularInterpolator()
        {
            Interpolator<T>::method_ = "Triangular";
        };

        TriangularInterpolator(math::Matrix<T> x, math::Matrix<T> y)
        {
            Interpolator<T>::method_ = "Triangular";
            Interpolator<T>::x_(x);
            Interpolator<T>::y_(y);
        };

    virtual ~TriangularInterpolator(){};
};
}