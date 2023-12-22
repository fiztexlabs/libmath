#pragma once
#include <libmath/matrix.h>
#include <libmath/math_settings.h>
#include <libmath/boolean.h>
#include <omp.h>
#include <string>

namespace math
{
	/**
	* @brief Interpolator base class
	* @author Ilya Konovalov
	*/
	template <typename T, typename = typename std::enable_if<isNumeric<T>>::type>
	class Interpolator
	{
	protected:
		/// @brief Dependent variables
		math::Matrix<T> y_;

		/// @brief Independent variables
		math::Matrix<T> x_;
	
		/// @brief Method name
		std::string method_ = "";

		/// @brief Dimension
		size_t dim_ = 0;

	public:
		virtual ~Interpolator() {};
	};
}