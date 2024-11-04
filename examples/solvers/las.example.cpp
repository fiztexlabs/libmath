#include <libmath/solver/las/bicgstab.h>
#include <libmath/matrix.h>
#include <iostream>

#ifdef MATH_OMP_DEFINE
#include <omp.h>
#endif

int main()
{
	// set threads number for parallelization
#ifdef MATH_OMP_DEFINE
omp_set_num_threads(1);
#endif

	// set LAS dimension
	size_t dim = 10;

	// generate matrix of LAS coefficients
	math::Matrix<double> A(dim);
	A.rfill(1);

	// generate column-vector of right-parts
	math::Matrix<double> b(dim, 1);
	b.rfill(2);

	// set initial guess to x column-vector
	math::Matrix<double> x(dim, 1);
	x.fill(0.0);

	// create solver with default setup
	math::BicGStab<double> bicgstab_solver;

	// solve system
	bicgstab_solver.solve(A, b, x);

	std::cout << "System:" << std::endl;
	std::cout << std::endl;

	A.print();
	std::cout << "*" << std::endl;
	x.print();
	std::cout << "=" << std::endl;
	b.print();
	std::cout << std::endl;

	std::cout << "Equations residuals:" << std::endl;
	(A * x - b).print();

}