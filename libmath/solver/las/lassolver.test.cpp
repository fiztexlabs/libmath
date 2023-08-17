#include <gtest/gtest.h>
#include <iostream>
#include <libmath/matrix.h>
#include <libmath/solver/las/lassolver.h>
#include <libmath/solver/las/bicgstab.h>
#include <libmath/solver/las/kholetsky.h>


TEST(LAS, BicGStab)
{
    omp_set_num_threads(1);

    size_t dim = 10;

    math::Matrix<double> A(dim);
    A.rfill(1);

    math::Matrix<double> b(dim, 1);
    b.rfill(2);

    math::Matrix<double> x(dim, 1);
    x.fill(0.0);

    math::BicGStab<double> bicgstab_solver;

    bicgstab_solver.solve(A, b, x);

    (A*x - b).print();
}

TEST(LAS, Kholetsky)
{
    omp_set_num_threads(1);

    size_t dim = 10;

    math::Matrix<double> A(dim);
    A.rfill(1);

    math::Matrix<double> b(dim, 1);
    b.rfill(2);

    math::Matrix<double> x(dim, 1);
    x.fill(0.0);

    math::Kholetsky<double> kholetsky_solver;

    kholetsky_solver.solve(A, b, x);

    (A * x - b).print();

}