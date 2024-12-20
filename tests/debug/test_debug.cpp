#include <iostream>
#include <chrono>
#include <gtest/gtest.h>
#include <libmath/matrix.h>
#include <libmath/solver/us/unlinearsolver.h>
#include <libmath/solver/us/secant.h>

TEST(US, constructor)
{
    math::UnlinearSolver<double>* solver1 = new math::Secant<double>();
	math::USsetup setup1
	{
		math::USStoppingCriteriaType::tolerance,
		100,
		1000,
		1.e-7,
		1.e-10,
		2,
		new math::Kholetsky<double>()
	};
	solver1->setupSolver(setup1);

	math::UnlinearSolver<double>* solver2 = solver1->copy();
    delete solver1;
	math::USsetup setup2;
	solver2->getSolverSetup(setup2);
}


TEST(Matrix, MulMbyM)
{
#ifdef MATH_OMP_DEFINE
omp_set_num_threads(1);
#endif

    math::Matrix<double> m1(1000);
    m1.rfill(2);
    math::Matrix<double> m2(1000);
    m2.rfill(2);

    math::Matrix<double> m3 = m1 * m2;
	// m2.print();
}

TEST(Debug, Matrix)
{
 //   math::Matrix<double> m1(10000, 10000);
 //   m1.fill(2.0);

	//math::Matrix<double> m2 = 2 * m1;
 //   math::pMatrix<double> pm1(m1);
 //   math::pMatrix<double> pm2(m2);
 //   math::pMatrix<double> pm3;
 //   math::Matrix<double> m3;

 //   //m3 = m1 + m2;
 //   pm3 = pm1 + pm2;

}

//int main()
//{
//
//	size_t n = 100000000;
//	std::vector<double> Av(n);
//	std::vector<double> Bv(n);
//	std::vector<double> Cv(n);
//
//	std::generate(Av.begin(), Av.end(), []() {
//		return rand() % 100;
//		});
//
//	std::generate(Bv.begin(), Bv.end(), []() {
//		return rand() % 100;
//		});
//
//	std::generate(Cv.begin(), Cv.end(), []() {
//		return rand() % 100;
//		});
//
//	int threads = 10;
//	size_t npt = n / threads;
//	omp_set_num_threads(threads);
//
//	auto start = std::chrono::steady_clock::now();
//	int i;
//#pragma omp parallel for shared (Av, Bv, Cv, n) private(i) schedule(static, npt)
//	for (i = 0; i < n; ++i)
//	{
//		Cv[i] = Av[i] + Bv[i];
//	}
//	auto end = std::chrono::steady_clock::now();
//
//	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
//
//}