#pragma once
#include <libmath/matrix.h>
#include <libmath/math_settings.h>
#include <libmath/boolean.h>
#include <libmath/solver/las/lassolver.h>
#include <libmath/solver/las/kholetsky.h>
#include <libmath/solver/las/bicgstab.h>
#include <libmath/math_exception.h>
#include <omp.h>
#include <string>
#include <memory>
#include <iostream>

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
		/// @brief Internal linear solver
		std::unique_ptr<LASsolver<T>> solver_{ std::unique_ptr<LASsolver<T>>(new Kholetsky<T>()) };

		/// @brief Dependent variables
		math::Matrix<T> y_;

		/// @brief Independent variables
		math::Matrix<T> x_;
	
		/// @brief Method name
		std::string method_ = "";

		/// @brief Dimension
		size_t dim_ = 0;

	public:
		Interpolator(const std::string& method) : method_(method) {};

		Interpolator(const std::string &method,
					 const math::Matrix<T> &x,
					 const math::Matrix<T> &y) : method_(method),
												 x_(x),
												 y_(y),
												 dim_(x.cols())
		{
			if (x_.rows() != y_.rows())
			{
				throw(ExceptionNonEqualRowsNum(
					"Interpolator<T> (" + method_ + ") interpolation: Matrices x and y have non-equal number of rows!"));
			}
			if (y_.cols() != 1)
			{
				throw(ExceptionNonColumnVector(
					"Interpolator<T> (" + method_ + ") interpolation: Matrix y of dependent variables must be column vector!"));
			}
		};

		virtual ~Interpolator() {};

		/// @brief Evaluate interpolation coefficients
		virtual void build() = 0;

		virtual T interpolate(const Matrix<T>& x) = 0;
	};
}