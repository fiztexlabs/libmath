#pragma once
#include <libmath/interpolator/interpolator.h>
#include <algorithm>
#include <cmath>
#include <omp.h>

namespace math
{
	template <typename T>
	class BiLinearInterpolator : public Interpolator<T>
	{
	private:
        /// @brief Matrix of data coefficients (from independent variables points)
        Matrix<T> A_;

        /// @brief Vector of data free terms (dependent variables points)
        Matrix<T> b_;

        /// @brief Vector of interpolation coefficients
        Matrix<T> c_;
	public:
        BiLinearInterpolator() : Interpolator<T>("BiLinear") {};

        BiLinearInterpolator(const math::Matrix<T>& x, const math::Matrix<T>& y) : Interpolator<T>("BiLinear", x, y)
        {
			A_ = Matrix<T>(std::pow(2,Interpolator<T>::dim_));
            b_ = static_cast<T>(-1.0) * y;
            c_ = Matrix<T>(std::pow(2,Interpolator<T>::dim_), 1);
        };

		virtual ~BiLinearInterpolator(){};

        virtual void build() override
        {
			for (int pos = 0; pos < A_.numel(); ++pos)
			{
				size_t row { 0 };
				size_t col { 0 };

				// A_ has row representation by default
				row = (size_t)std::floor(pos / A_.cols());
				col = pos - row * A_.cols();

				
			}
		}
		
		virtual T interpolate(const Matrix<T>& x) override
		{
			return 1.0;
		}
	};
}