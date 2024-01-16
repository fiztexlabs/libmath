#include <gtest/gtest.h>
#include <libmath/interpolator/bilinear_interpolator.h>
#include <libmath/interpolator/polygone_interpolator.h>


TEST(Interpolator, Polygone)
{
	// check 1d

	math::Matrix<double> x1 =
	{
		{1.},
		{2.}
	};

	math::Matrix<double> y1 =
	{
		{1.},
		{2.}
	};

	math::PolygoneInterpolator<double> polygone1(x1, y1);
	polygone1.build();

	double y_1d1 = polygone1.interpolate({{1.}});
	EXPECT_EQ(math::isEqual(y_1d1, 1.0), true);

	double y_1d2 = polygone1.interpolate({{1.5}});
	EXPECT_EQ(math::isEqual(y_1d2, 1.5), true);

	double y_1d3 = polygone1.interpolate({{3.}});
	EXPECT_EQ(math::isEqual(y_1d3, 3.), true);
	
	// check 2d

	math::Matrix<double> x2 =
	{
		{1.,1.},
		{2.,1.},
		{1.,2.}
	};

	math::Matrix<double> y2 =
	{
		{1.},
		{2.},
		{2.}
	};

	math::PolygoneInterpolator<double> polygone2(x2, y2);
	polygone2.build();

	double y_2d1 = polygone2.interpolate({{2., 1.}});
	EXPECT_EQ(math::isEqual(y_2d1, 2.0), true);

	double y_2d2 = polygone2.interpolate({{1., 1.5}});
	EXPECT_EQ(math::isEqual(y_2d2, 1.5), true);

	double y_2d3 = polygone2.interpolate({{2., 2.}});
	EXPECT_EQ(math::isEqual(y_2d3, 3.), true);
	
	// check 3d

	math::Matrix<double> x3 =
	{
		{1.,1.,1.},
		{2.,1.,2.},
		{1.,2.,2.},
		{3.,1.,2.}
	};

	math::Matrix<double> y3 =
	{
		{1.},
		{2.},
		{2.},
		{3.}
	};

	math::PolygoneInterpolator<double> polygone3(x3, y3);
	polygone3.build();

	double y_3d1 = polygone3.interpolate({{2., 1., 2.}});
	EXPECT_EQ(math::isEqual(y_3d1, 2.0), true);

	double y_3d2 = polygone3.interpolate({{1., 1.5, 1.5}});
	EXPECT_EQ(math::isEqual(y_3d2, 1.5), true);

	double y_3d3 = polygone3.interpolate({{3., 3., 3.}});
	EXPECT_EQ(math::isEqual(y_3d3, 5.), true);
}

// TEST(Interpolator, BiLinear)
// {
// 	math::Matrix<double> x =
// 	{
// 		{1.,1.},
// 		{2.,1.},
// 		{1.,2.},
// 		{2.,2.}
// 	};

// 	math::Matrix<double> y =
// 	{
// 		{1.},
// 		{1.},
// 		{1.},
// 		{1.}
// 	};

// 	math::BiLinearInterpolator<double> bilinear(x, y);
// 	bilinear.build();

// 	math::Matrix<double> new_x = 
// 	{
// 		{2., 1.}
// 	};
// 	double y_int = bilinear.interpolate(new_x);

// 	std::cout << y_int << std::endl;
// }