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

		/// @brief Check input points
        void checkInputs(math::Matrix<T> x, math::Matrix<T> y)
		{
			std::cerr << "%%err " + method_ +
							 ": Matrices of dependent and independent variables must have the same number of rows: " +
							 std::string(x_.rows()) + " != " + std::string(y_.rows())
					  << std::endl;

			if (x.rows() != y.rows())
			{
				throw(ExceptionNonEqualRowsNum(
					"%%err " + method_ +
					": Matrices of dependent and independent variables must have the same number of rows: " +
					std::string(x_.rows()) + " != " + std::string(y_.rows())));
			}
		};

	public:
		virtual ~Interpolator() {};

		/// @brief Evaluate interpolation coefficients
		virtual void build() = 0;

		virtual void interpolate(const Matrix<T>& x, Matrix<T>& y) = 0;
	};
}