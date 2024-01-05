#include <gtest/gtest.h>
#include <libmath/interpolator/bilinear_interpolator.h>
#include <libmath/interpolator/triangular_interpolator.h>


TEST(Interpolator, Triangular)
{
	math::TriangularInterpolator<double> triangular;

	std::cout << "hi" << std::endl;
}

TEST(Interpolator, BiLinear)
{
	math::BiLinearInterpolator<double> bilinear;

	std::cout << "hi" << std::endl;
}