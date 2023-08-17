#include <gtest/gtest.h>
#include <iostream>
#include <libmath/matrix.h>
#include <libmath/solver/las/lassolver.h>
#include <libmath/solver/las/bicgstab.h>
#include <libmath/solver/las/kholetsky.h>
#include <libmath/boolean.h>
#include <omp.h>


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

    double r = (A * x - b).pnorm(2);

    EXPECT_EQ(math::isEqual(r, 0.0), true);
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

    double r = (A * x - b).pnorm(2);

    EXPECT_EQ(math::isEqual(r, 0.0), true);

}

TEST(LAS, Setup)
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

    math::LASsetup new_setup =
    {
        math::LASStoppingCriteriaType::tolerance,
        100,
        1000,
        0.0001
    };

    kholetsky_solver.setupSolver(new_setup);

    kholetsky_solver.solve(A, b, x);

    double r = (A * x - b).pnorm(2);

    EXPECT_EQ(math::isEqual(r, 0.0), true);

}