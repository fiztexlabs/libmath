#pragma once
#include <libmath/interpolator/interpolator.h>

namespace math
{

	template <typename T>
	class BiLinearInterpolator : public Interpolator<T>
	{
	private:
	public:
		BiLinearInterpolator()
		{
			Interpolator<T>::method_ = "BiLinear";
		};

		virtual ~BiLinearInterpolator(){};
	};
}