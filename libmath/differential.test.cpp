#include <gtest/gtest.h>
#include <iostream>
#include <libmath/differential.h>
#include <libmath/boolean.h>

TEST(partialDerivate, DiffSchemes)
{
	// define function
	std::function<double(const math::Matrix<double>&)> f1(
		[](const  math::Matrix<double>& x)
		{
			return (pow(x(0, 0), 2.0) + pow(x(1, 0), 2.0) - x(2, 0) - 6.0);
		}
	);

	// scheme 1
	double df1dx1_sch1 = math::partialDerivate(
		f1,
		{
			{3.0},
			{2.0},
			{4.0}
		},
		0
	);
	EXPECT_EQ(math::isEqual(df1dx1_sch1, 6.0), true);

	// scheme 2
	double df1dx1_sch2 = math::partialDerivate(
		f1,
		{
			{3.0},
			{2.0},
			{4.0}
		},
		0,
		2
	);
	EXPECT_EQ(math::isEqual(df1dx1_sch2, 6.0), true);

}

TEST(partialDerivate, Constraints)
{
	// define function
	std::function<double(const math::Matrix<double>&)> f1(
		[](const  math::Matrix<double>& x)
		{
			if(x(0,0)<0.0)
			{
				return 0.0;
			}
			if(x(0,0)>1.0)
			{
				return 1.0;
			}
			return 1.0*x(0, 0);
		}
	);

	// lower without constraint
	double dfdx_low = math::partialDerivate(
		f1,
		{
			{-1.0}
		}
	);
	EXPECT_EQ(math::isEqual(dfdx_low, 0.0), true);

	// lower with constraint
	double dfdx_low_constr = math::partialDerivate(
		f1,
		{
			{-1.0}
		},
		0,
		1,
		0.1 * math::settings::CurrentSettings.targetTolerance,
		0.0,
		1.0
	);
	EXPECT_EQ(math::isEqual(dfdx_low_constr, 1.0), true);

	// upper without constraint
	double dfdx_up = math::partialDerivate(
		f1,
		{
			{2.0}
		}
	);
	EXPECT_EQ(math::isEqual(dfdx_up, 0.0), true);

	// upper with constraint
	double dfdx_up_constr = math::partialDerivate(
		f1,
		{
			{2.0}
		},
		0,
		1,
		0.1 * math::settings::CurrentSettings.targetTolerance,
		0.0,
		1.0
	);
	EXPECT_EQ(math::isEqual(dfdx_up_constr, 1.0), true);

}

TEST(partialDerivate, IncorrectArgumentsVector)
{
	// define function
	std::function<double(const math::Matrix<double>&)> f1(
		[](const  math::Matrix<double>& x)
		{
			return (pow(x(0, 0), 2.0) + pow(x(1, 0), 2.0) - x(2, 0) - 6.0);
		}
	);
	double df1dx1_sch1 = 0.0;

	// incorrect x vector
	try
	{
		df1dx1_sch1 = math::partialDerivate(
			f1,
			{
				{3.0, 0.0},
				{2.0, 0.0},
				{4.0, 0.0}
			},
			0
		);
	}
	catch (const math::ExceptionIncorrectMatrix& exc)
	{
		EXPECT_EQ(exc.what(), std::string("partialDerivate: Matrix x argument must be column matrix!"));
	}
}

TEST(partialDerivate, IncorrectArgumentIndex)
{
	// define function
	std::function<double(const math::Matrix<double>&)> f1(
		[](const  math::Matrix<double>& x)
		{
			return (pow(x(0, 0), 2.0) + pow(x(1, 0), 2.0) - x(2, 0) - 6.0);
		}
	);
	double df1dx1_sch1 = 0.0;

	// incorrect index for x
	try
	{
		df1dx1_sch1 = math::partialDerivate(
			f1,
			{
				{3.0},
				{2.0},
				{4.0}
			},
			8
		);
	}
	catch (const math::ExceptionIndexOutOfBounds& exc)
	{
		EXPECT_EQ(exc.what(), std::string("partialDerivate: Incorrect xId argument!"));
	}
}

TEST(partialDerivate, IncorrectDiffScheme)
{
	// define function
	std::function<double(const math::Matrix<double>&)> f1(
		[](const  math::Matrix<double>& x)
		{
			return (pow(x(0, 0), 2.0) + pow(x(1, 0), 2.0) - x(2, 0) - 6.0);
		}
	);
	double df1dx1_sch1 = 0.0;

	// incorrect scheme
	try
	{
		df1dx1_sch1 = math::partialDerivate(
			f1,
			{
				{3.0},
				{2.0},
				{4.0}
			},
			0,
			-2
		);
	}
	catch (const math::ExceptionInvalidValue& exc)
	{
		EXPECT_EQ(exc.what(), std::string("partialDerivate: Incorrect scheme argument!"));
	}
}

TEST(partialDerivate, IncorrectConstraints)
{
	// define function
	std::function<double(const math::Matrix<double>&)> f1(
		[](const  math::Matrix<double>& x)
		{
			return (pow(x(0, 0), 2.0) + pow(x(1, 0), 2.0) - x(2, 0) - 6.0);
		}
	);
	double df1dx1_sch1 = 0.0;

	// incorrect constraints ( lower > upper)
	try
	{
		df1dx1_sch1 = math::partialDerivate(
			f1,
			{
				{3.0},
				{2.0},
				{4.0}
			},
			0,
			1,
			1.e-6,
			5.0,
			4.0
		);
	}
	catch (const math::ExceptionInvalidValue& exc)
	{
		EXPECT_EQ(exc.what(), std::string("partialDerivate with constrained arguments: Invalid constraints. Lower bound must be lower, than upper bound!"));
	}

	// incorrect constraints ( upper - lower < 2*dX)
	try
	{
		df1dx1_sch1 = math::partialDerivate(
			f1,
			{
				{3.0},
				{2.0},
				{4.0}
			},
			0,
			1,
			1.,
			4.0,
			4.1
		);
	}
	catch (const math::ExceptionInvalidValue& exc)
	{
		EXPECT_EQ(exc.what(), std::string("partialDerivate with constrained arguments: Distance between lower and upper bounds must greater, than 2*dX=" + std::to_string(2.0 * 1.0) + "!"));
	}
}

TEST(diff, Calculation)
{
	// define function
	std::function<double(double)> f(
		[](const double x)
		{
			return (pow(x, 2.0) - 9.0);
		}
	);

	double dfdx = math::diff(f, 3.0);

	EXPECT_EQ(math::isEqual(dfdx, 6.0), true);

}

TEST(jakobi, Calculation)
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

	// truth
	math::Matrix<double> J_t =
	{
		{2.0,2.0,-1.0},
		{1.0,1.0,1.0},
		{1.0,1.0,1.0}
	};

	EXPECT_EQ(J.compare(J_t), true);

}

TEST(jakobi, Constraints)
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

	math::Matrix<double> x0 =
	{
		{1.0},
		{1.0},
		{1.0}
	};

	// lower constraint
	math::Matrix<double> x_min = 
	{
		{0.0},
		{0.0},
		{0.0}
	};

	// upper constraint
	math::Matrix<double> x_max = 
	{
		{0.5},
		{0.5},
		{0.5}
	};

	// define J matrix for results of jakobian
	math::Matrix<double> J(3);

	// calculate Jakobi matrix
	// math::jacobi(F, x0, J, 1, );

}