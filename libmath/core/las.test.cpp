#include <gtest/gtest.h>
#include <iostream>
#include <libmath/core/matrix.h>
#include <libmath/core/las.h>


TEST(LAS, bicGStabLS)
{
    omp_set_num_threads(1);

    size_t dim = 30;

    math::Matrix<double> A(dim);
    A.rfill(1);

    math::Matrix<double> b(dim, 1);
    b.rfill(2);

    math::Matrix<double> x0(dim, 1);
    x0.fill(0.0);

    math::Matrix<double> x(dim, 1);

    math::bicGStabLS(A, b, x0, x);

    (A*x - b).print();
}