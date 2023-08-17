#include <gtest/gtest.h>
#include <iostream>
#include <libmath/differential.h>
#include <libmath/boolean.h>

TEST(partialDerivate, Calculation)
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

TEST(partialDerivate, TryIncorrectArguments)
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