#pragma once

#include <libmath/matrix.h>
#include <libmath/math_settings.h>
#include <libmath/math_exception.h>
#include <libmath/boolean.h>
#include <vector>
#include <functional>

#ifdef MATH_OMP_DEFINE
#include <omp.h>
#endif

#include <chrono>
#include <utility>
#include <numeric>
#include <algorithm>
#include <cmath>

namespace math
{
	/**
	 * @brief Partial derivate of function @f$ f @f$
	 * @details Calculate @f$ \frac{\partial f}{\partial x} @f$.
	 *
	 * Example of using in C++:
	 * @code
	 * #include <libmath/differential.h>
	 *
	 * int main()
	 * {
	 *	// define function
	 *	std::function<double(const math::Matrix<double>&)> f1(
	 *		[](const  math::Matrix<double>& x)
	 *		{
	 *			return (pow(x(0, 0), 2.0) + pow(x(1, 0), 2.0) - x(2, 0) - 6.0);
	 *		}
	 *	);
	 *
	 *	// scheme 1
	 *	double df1dx1_sch1 = math::partialDerivate(
	 *		f1,
	 *		{
	 *			{3.0},
	 *			{2.0},
	 *			{4.0}
	 *		},
	 *		0
	 *	);
	 * }
	 * @endcode
	 * @param scheme: Scheme of differentiation (1 by default)
	 *	- 1: Scheme of first order, calculates as @f$ \frac{\partial f}{\partial x} = \frac{f(x_i) - f(x_{i-1})}{x_i-x_{i-1}} @f$
	 *	- 2: Scheme of second order, calculates as
	 * @f$ \frac{\partial f}{\partial x} = \frac{\frac{3}{2} f(x_{i+1} - 2 f(x_i) + \frac{1}{2} f(x_{i-1}}{x_i-x_{i-1}} @f$
	 * @param stepX: Step of derivate calculation, @f$ \Delta x = x_i-x_{i-1} @f$ (0.1*math::settings::Settings.targetTolerance by default)
	 * @param F: Function
	 * @param x: Column-vector of aruments
	 * @param xId: index of derivated variable
	 * @param lower_bound: Lower constraints for independent variables. Stay empty, for evaluating without lower constraints.
	 * @param upper_bound: Upper constraints for independent variables. Stay empty, for evaluating without upper constraints.
	 *
	 * @return Partial derivate of f with x variable @f$ \frac{\partial f}{\partial x} @f$
	 */
	template <typename T, typename T1, class = std::enable_if<isNumeric<T> && isNumeric<T1>>>
	T partialDerivate(const std::function<T(const Matrix<T1> &)> &F,
					  const math::Matrix<T1> &x,
					  const size_t xId = 0,
					  const int scheme = 1,
					  T1 stepX = static_cast<T1>(0.1 * math::settings::CurrentSettings.targetTolerance),
					  const Matrix<T1> &lower_bound = Matrix<T1>(),
					  const Matrix<T1> &upper_bound = Matrix<T1>())
	{
		// check inputs
		if (x.cols() > 1)
		{
			throw(math::ExceptionIncorrectMatrix("partialDerivate: Matrix x argument must be column matrix!"));
		}
		if (xId >= x.rows())
		{
			throw(math::ExceptionIndexOutOfBounds("partialDerivate: Incorrect xId argument!"));
		}
		// check constraints
		if (!lower_bound.empty() && !upper_bound.empty() )
		{
			if (lower_bound.rows() != upper_bound.rows())
			{
				throw(math::ExceptionIncorrectMatrix("partialDerivate with constrained arguments: Dimensions of lower and upper bounds must agree!"));
			}
		}

		if (!lower_bound.empty()) // check vector dimension, if defined
		{
			if (lower_bound.cols() > 1)
			{
				throw(ExceptionIncorrectMatrix("partialDerivate with constrained arguments: Lower bounds must be column matrix!"));
			}
		}

		if (!upper_bound.empty()) // check vector dimension, if defined
		{
			if (upper_bound.cols() > 1)
			{
				throw(ExceptionIncorrectMatrix("partialDerivate with constrained arguments: Upper bounds must be column matrix!"));
			}
		}

		if (!lower_bound.empty() && !upper_bound.empty())
		{
			for (size_t i = 0; i < lower_bound.rows(); ++i)
			{
				if (lower_bound(i, 0) > upper_bound(i, 0))
				{
					throw(math::ExceptionInvalidValue("partialDerivate with constrained arguments: Invalid constraints. Lower bound must be lower, than upper bound!"));
				}
				if (std::abs(upper_bound(i, 0) - lower_bound(i, 0)) < static_cast<T1>(2.) * stepX)
				{
					throw(math::ExceptionInvalidValue("partialDerivate with constrained arguments: Distance between lower and upper bounds must greater, than 2*dX=" + std::to_string(static_cast<T1>(2.) * stepX) + "!"));
				}
			}
		}

		T dFdX = static_cast<T>(0.0);
		math::Matrix<T1> current_x = x;

		// if lower bound defined
		if (!lower_bound.empty())
		{
			for (size_t i = 0; i < lower_bound.rows(); ++i)
			{
				current_x(i, 0) = std::max(x(i, 0), lower_bound(i, 0));
			}
			current_x(xId, 0) = std::max(x(xId, 0), lower_bound(xId, 0) + stepX);
		}
		// if upper bound defined
		if (!upper_bound.empty())
		{
			for (size_t i = 0; i < upper_bound.rows(); ++i)
			{
				current_x(i, 0) = std::min(current_x(i, 0), upper_bound(i, 0));
			}
			current_x(xId, 0) = std::min(current_x(xId, 0), upper_bound(xId, 0) - stepX);
		}

		math::Matrix<T1> previous_x = current_x;
		math::Matrix<T1> next_x = current_x;

		previous_x(xId, 0) = current_x(xId, 0) - stepX;
		next_x(xId, 0) = current_x(xId, 0) + stepX;

		switch (scheme)
		{
		case 1:
			dFdX = (F(current_x) - F(previous_x)) / static_cast<T>(stepX);
			break;
		case 2:
			dFdX = ((3.0 / 2.0) * F(next_x) - 2.0 * F(current_x) + 0.5 * F(previous_x)) / static_cast<T>(stepX);
			break;
		default:
			throw(math::ExceptionInvalidValue("partialDerivate: Incorrect scheme argument!"));
		}

		return dFdX;
	}

	/**
	 * @brief Derivate of function @f$ f(x) @f$
	 * @details Call partialDerivate and calculate full derivate as partial derivate of function with single argument x by x.
	 *
	 * Example of using in C++:
	 * @code
	 * #include <libmath/differential.h>
	 * #include <iostream>
	 *
	 * int main()
	 * {
	 *	// define function
	 *	std::function<double(double)> f(
	 *		[](const double x)
	 *		{
	 *			return (pow(x, 2.0) - 9.0);
	 *		}
	 *	);
	 *
	 *	double dfdx = math::diff(f, 3.0);
	 * }
	 * @endcode
	 * @param scheme: Scheme of differentiation (1 by default)
	 *	- 1: Scheme of first order, calculates as @f$ \frac{df}{dx} = \frac{f(x_i) - f(x_{i-1})}{x_i-x_{i-1}} @f$
	 *	- 2: Scheme of second order, calculates as
	 * @f$ \frac{df}{dx} = \frac{\frac{3}{2} f(x_{i+1} - 2 f(x_i) + \frac{1}{2} f(x_{i-1}}{x_i-x_{i-1}} @f$.
	 * @param stepX: Step of derivate calculation, @f$ \Delta x = x_i-x_{i-1} @f$ (0.1*math::settings::Settings.targetTolerance by default)
	 * @param F: Function
	 * @param x: Argument
	 * @param lower_bound: Lower constraints for independent variable x. Stay NaN value, for evaluating without lower constraints.
	 * @param upper_bound: Upper constraints for independent variable x. Stay NaN value, for evaluating without upper constraints.
	 *
	 * @return Derivate of f with x argument, @f$ \frac{df}{dx} @f$
	 */
	template <typename T, typename T1, class = std::enable_if<isNumeric<T> && isNumeric<T1>>>
	T diff(
		const std::function<T(const T1)> &F,
		const T1 x,
		const int scheme = 1,
		T1 stepX = static_cast<T1>(0.1 * math::settings::CurrentSettings.targetTolerance),
		T1 lower_bound = std::numeric_limits<T1>::quiet_NaN(),
		T1 upper_bound = std::numeric_limits<T1>::quiet_NaN())
	{
		Matrix<T1> lb;
		Matrix<T1> ub;

		if(!std::isnan(lower_bound))
		{
			lb = {{lower_bound}};
		} 
		if(!std::isnan(upper_bound))
		{
			ub = {{upper_bound}};
		} 

		Matrix<T1> args(std::vector<T1>{x}, 1);
		std::function<T(const Matrix<T1> &)> f(
			[F](const Matrix<T1> &args)
			{
				return F(args(0, 0));
			});

		return math::partialDerivate<T, T1>(
			f,
			args,
			0,
			scheme,
			stepX,
			lb,
			ub);
	}

	/**
	 * @brief Jacobi matrix of vector function @f$ \mathbf{u} @f$ with arguments @f$ \mathbf{x} @f$
	 * @details Calculate Matrix of size MxN, where M - number of functions F, N - number of functions arguments x.
	 * @f$ \mathbf{J} =
	 * \begin{pmatrix}
	 *  \frac{\partial u_1}{\partial x_1} & \frac{\partial u_1}{\partial x_2} & \cdots & \frac{\partial u_1}{\partial x_n} \\
	 *  \frac{\partial u_2}{\partial x_1} & \frac{\partial u_2}{\partial x_2} & \cdots & \frac{\partial u_2}{\partial x_n} \\
	 *  \vdots  & \vdots  & \ddots & \vdots  \\
	 *  \frac{\partial u_m}{\partial x_1} & \frac{\partial u_m}{\partial x_2} & \cdots & \frac{\partial u_m}{\partial x_n} \\
	 * \end{pmatrix} @f$.
	 *
	 * Example of using in C++:
	 * @code
	 * #include <libmath/differential.h>
	 * #include <libmath/matrix.h>
	 * #include <iostream>
	 * #include <omp.h>
	 *
	 * int main()
	 * {
	 *	omp_set_num_threads(4);
	 *
	 *	// vector function F
	 *	std::vector<std::function<double(const math::Matrix<double>&)>> F;
	 *
	 *	F.push_back
	 *	(
	 *		[](const math::Matrix<double>& x)
	 *		{
	 *			return (pow(x(0, 0), 2.0) + pow(x(1, 0), 2.0) - x(2, 0) - 6.0);
	 *		}
	 *	);
	 *	F.push_back
	 *	(
	 *		[](const math::Matrix<double>& x)
	 *		{
	 *			return (x(0, 0) + x(1, 0) * x(2, 0) - 2.0);
	 *		}
	 *	);
	 *	F.push_back
	 *	(
	 *		[](const math::Matrix<double>& x)
	 *		{
	 *			return (x(0, 0) + x(1, 0) + x(2, 0) - 3.0);
	 *		}
	 *	);
	 *
	 *	math::Matrix<double> x0 =
	 *	{
	 *		{1.0},
	 *		{1.0},
	 *		{1.0}
	 *	};
	 *
	 *	// define J matrix for results of jakobian
	 *	math::Matrix<double> J(3);
	 *
	 *	// calculate Jakobi matrix
	 *	math::jacobi(F, x0, J);
	 *
	 * }
	 * @endcode
	 * @param[in] scheme: Scheme of differentiation (1 by default)
	 *	- 1: Scheme of first order, calculates as @f$ \frac{\partial f}{\partial x} = \frac{f(x_i) - f(x_{i-1})}{x_i-x_{i-1}} @f$
	 *	- 2: Scheme of second order, calculates as
	 * @f$ \frac{\partial f}{\partial x} = \frac{\frac{3}{2} f(x_{i+1} - 2 f(x_i) + \frac{1}{2} f(x_{i-1}}{x_i-x_{i-1}} @f$
	 * @param stepX: Step of derivate calculation, @f$ \Delta x = x_i-x_{i-1} @f$ (0.001*math::settings::Settings.targetTolerance by default)
	 * @param[in] F: Vector of functions (F = vector function @f$ \mathbf{u} @f$)
	 * @param[in] x: Column matrix of arguments of F, for which Jakobian calculates
	 * @param[out] J: Jakobi's matrix of size MxN
	 * @param[in] lower_bound: Vector of arguments lower bounds
	 * @param[in] upper_bound: Vector of arguments upper bounds
	 */
	template <typename T, typename T1, class = std::enable_if<isNumeric<T> && isNumeric<T1>>>
	void jacobi(
		const std::vector<std::function<T(const Matrix<T1> &)>> &F,
		const math::Matrix<T1> &x,
		math::Matrix<T> &J,
		const int scheme = 1,
		T1 stepX = static_cast<T1>(0.001 * math::settings::CurrentSettings.targetTolerance),
		const Matrix<T1> &lower_bound = Matrix<T1>(),
		const Matrix<T1> &upper_bound = Matrix<T1>())
	{
		// check inputs
		if (x.cols() > 1)
		{
			throw(math::ExceptionIncorrectMatrix("jacobi: Matrix x argument must be column matrix!"));
		}
		if (x.rows() != F.size())
		{
			throw(math::ExceptionIncorrectMatrix("jacobi: Dimensions of input argument F and output x didn't agree!"));
		}

		// check constraints
		if (!lower_bound.empty() && !upper_bound.empty() )
		{
			if (lower_bound.rows() != upper_bound.rows())
			{
				throw(math::ExceptionIncorrectMatrix("jacobi with constrained arguments: Dimensions of lower and upper bounds must agree!"));
			}
		}
		if (!lower_bound.empty()) // check vector dimension, if defined
		{
			if (lower_bound.cols() > 1)
			{
				throw(ExceptionIncorrectMatrix("jacobi with constrained arguments: Lower bounds must be column matrix!"));
			}
		}

		if (!upper_bound.empty()) // check vector dimension, if defined
		{
			if (upper_bound.cols() > 1)
			{
				throw(ExceptionIncorrectMatrix("jacobi with constrained arguments: Upper bounds must be column matrix!"));
			}
		}

		size_t m = F.size();
		size_t n = x.rows();

		// check inputs
		if ((J.rows() != m) || (J.cols() != n))
		{
			throw(ExceptionIncorrectMatrix("jacobi: Output matrix J must be " + std::to_string(m) + "x" + std::to_string(n) + " matrix!"));
		}

		size_t n_els = J.numel();

		// auto start = std::chrono::steady_clock::now();

#ifdef MATH_OMP_DEFINE
#pragma omp parallel for shared(J, n_els, lower_bound, upper_bound) schedule(static)
#endif
		for (int pos = 0; pos < n_els; ++pos)
		{
			size_t row = 0;
			size_t col = 0;
			if (J.representation() == math::MatRep::Row) // row repr
			{
				row = (size_t)std::floor(pos / J.cols());
				col = pos - row * J.cols();
			}
			else if (J.representation() == math::MatRep::Column) // column repr
			{
				col = (size_t)std::floor(pos / J.rows());
				row = pos - J.rows() * col;
			}
			J(col, row) = math::partialDerivate<T, T1>(F[col], x, row, scheme, stepX, lower_bound, upper_bound);
		}
		// auto end = std::chrono::steady_clock::now();
		// std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
	}
}