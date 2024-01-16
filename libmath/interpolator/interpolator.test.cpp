#include <gtest/gtest.h>
#include <libmath/interpolator/bilinear_interpolator.h>
#include <libmath/interpolator/polygone_interpolator.h>


TEST(Interpolator, Polygone)
{
	math::Matrix<double> x =
	{
		{1.,1.},
		{2.,1.},
		{1.,2.}
	};

	math::Matrix<double> y =
	{
		{1.},
		{2.},
		{2.}
	};

	math::PolygoneInterpolator<double> polygone(x, y);
	polygone.build();

	math::Matrix<double> new_x = 
	{
		{2., 1.}
	};
	double y_int = polygone.interpolate(new_x);

	std::cout << y_int << std::endl;
}

TEST(Interpolator, BiLinear)
{
	// math::BiLinearInterpolator<double> bilinear;

	std::cout << "hi" << std::endl;
}