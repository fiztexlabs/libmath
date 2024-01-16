#include <libmath/interpolator/polygone_interpolator.h>
#include <libmath/matrix.h>
#include <iostream>

int main()
{
	// experimental independetn variables at 3 points
	math::Matrix<double> x =
	{
		{1.,1.},
		{2.,1.},
		{1.,2.}
	};

	// experimental dependetn variables at 3 points
	math::Matrix<double> y =
	{
		{0.},
		{0.},
		{0.}
	};

	// create interpolator and build vector of interpolation coefficients
	math::PolygoneInterpolator<double> polygone(x, y);
	polygone.build();

	// evaluate value of dependent variable, correspond to new set of independent variables
	math::Matrix<double> new_x = 
	{
		{1.5, 1.5}
	};
	double y_int = polygone.interpolate(new_x);

	std::cout << y_int << std::endl;
}