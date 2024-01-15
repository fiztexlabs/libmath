#include <gtest/gtest.h>
#include <libmath/interpolator/bilinear_interpolator.h>
#include <libmath/interpolator/triangular_interpolator.h>


TEST(Interpolator, Triangular)
{
	math::Matrix<double> x =
	{
		{1.,1.},
		{2.,1.},
		{1.,2.}
	};

	math::Matrix<double> y =
	{
		{0.},
		{0.},
		{0.}
	};

	math::TriangularInterpolator<double> triangular(x, y);
	triangular.build();

	math::Matrix<double> new_y;
	math::Matrix<double> new_x = 
	{
		{1.5},
		{1.5}
	};
	triangular.interpolate(new_x, new_y);

	std::cout << "hi" << std::endl;
}

TEST(Interpolator, BiLinear)
{
	// math::BiLinearInterpolator<double> bilinear;

	std::cout << "hi" << std::endl;
}