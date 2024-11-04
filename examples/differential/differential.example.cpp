#include <libmath/differential.h>
#include <libmath/matrix.h>
#include <iostream>

#ifdef MATH_OMP_DEFINE
#include <omp.h>
#endif

int main()
{
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Partial derivation

	// define function
	std::function<double(const math::Matrix<double>&)> f1(
		[](const  math::Matrix<double>& x)
		{
			return (pow(x(0, 0), 2.0) + pow(x(1, 0), 2.0) - x(2, 0) - 6.0);
		}
	);

	// partial derivate
	double df1dx1_sch1 = math::partialDerivate(
		f1,
		{
		{3.0},
		{2.0},
		{4.0}
		},
		0
	);
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Differentiation
	// define function
	std::function<double(double)> f(
		[](const double x)
		{
			return (pow(x, 2.0) - 9.0);
		}
	);

	double dfdx = math::diff(f, 3.0);
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Jacobi matrix
#ifdef MATH_OMP_DEFINE
omp_set_num_threads(4);
#endif

	// vector function F
	std::vector<std::function<double(const math::Matrix<double>&)>> F;

	F.push_back
	(
		[](const math::Matrix<double>& x)
		{
			return (pow(x(0, 0), 2.0) + pow(x(1, 0), 2.0) - x(2, 0) - 6.0);
		}
	);
	F.push_back
	(
		[](const math::Matrix<double>& x)
		{
			return (x(0, 0) + x(1, 0) * x(2, 0) - 2.0);
		}
	);
	F.push_back
	(
		[](const math::Matrix<double>& x)
		{
			return (x(0, 0) + x(1, 0) + x(2, 0) - 3.0);
		}
	);

	math::Matrix<double> x0 =
	{
		{1.0},
		{1.0},
		{1.0}
	};

	// define J matrix for results of jakobian
	math::Matrix<double> J(3);

	// calculate Jakobi matrix
	math::jacobi(F, x0, J);

	J.print();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

}