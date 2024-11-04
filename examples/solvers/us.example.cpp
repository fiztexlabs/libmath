#include <libmath/solver/us/secant.h>
#include <libmath/matrix.h>
#include <iostream>

#ifdef MATH_OMP_DEFINE
#include <omp.h>
#endif

int main()
{
	// solve system of unlinear equations

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

	// define initial values
	math::Matrix<double> x =
	{
		{1.0},
		{1.0},
		{1.0}
	};

	// create Secant solver with default setup
	math::Secant<double> secant_solver;

	secant_solver.solve(F, x);

	std::cout <<"Residual of function F[0] fot solving system of unlinear equations " << F[0](x) << std::endl;
	std::cout <<"Residual of function F[1] fot solving system of unlinear equations " << F[1](x) << std::endl;
	std::cout <<"Residual of function F[2] fot solving system of unlinear equations " << F[2](x) << std::endl;

	// using US solver for solving single unlinear eqution

	// single function f
	std::function<double(const math::Matrix<double>&)> f(
		[](const math::Matrix<double>& x)
		{
			return (pow(x(0, 0), 2.0) + pow(x(0, 0), 2.0) - x(0, 0) - 6.0);
		}
	);

	// define x
	x = { {1.0} };

	secant_solver.solve({ f }, x);

	std::cout << "Residual of function f fot solving single unlinear equation " << f(x) << std::endl;

}