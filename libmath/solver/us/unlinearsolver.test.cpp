#include <gtest/gtest.h>
#include <iostream>
#include <libmath/matrix.h>
#include <libmath/solver/us/unlinearsolver.h>
#include <libmath/solver/us/secant.h>
#include <libmath/boolean.h>
#include <omp.h>

TEST(USS, Secant)
{
	omp_set_num_threads(4);

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

	math::Secant<double> secant_solver;

	secant_solver.solve(F, x);

	EXPECT_EQ(math::isEqual(F[0](x), 0.0), true);
	EXPECT_EQ(math::isEqual(F[1](x), 0.0), true);
	EXPECT_EQ(math::isEqual(F[2](x), 0.0), true);
}

TEST(US, Secant)
{
	omp_set_num_threads(4);

	// single function f
	std::function<double(const math::Matrix<double>&)> f(
		[](const math::Matrix<double>& x)
		{
			return (pow(x(0, 0), 2.0) + pow(x(0, 0), 2.0) - x(0, 0) - 6.0);
		}
	);

	// define x
	math::Matrix<double> x = { {1.0} };

	math::Secant<double> secant_solver;

	secant_solver.solve({ f }, x);

	EXPECT_EQ(math::isEqual(f(x), 0.0), true);
}

TEST(US, SecantSetup)
{
	omp_set_num_threads(4);

	// single function f
	std::function<double(const math::Matrix<double>&)> f(
		[](const math::Matrix<double>& x)
		{
			return (pow(x(0, 0), 2.0) + pow(x(0, 0), 2.0) - x(0, 0) - 6.0);
		}
	);

	// define x
	math::Matrix<double> x = { {1.0} };

	math::Secant<double> secant_solver;

	math::USsetup new_setup = 
	{
		math::USStoppingCriteriaType::tolerance,
		100,
		1000,
		1.e-7,
		1.e-10,
		2,
		new math::Kholetsky<double>()
	};

	secant_solver.setupSolver(new_setup);

	secant_solver.solve({ f }, x);

	EXPECT_EQ(math::isEqual(f(x), 0.0), true);
}