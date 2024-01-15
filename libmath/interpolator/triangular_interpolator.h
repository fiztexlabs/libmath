#pragma once
#include <libmath/interpolator/interpolator.h>
#include <libmath/matrix.h>
#include <libmath/math_exception.h>

namespace math
{
    template <typename T>
    class TriangularInterpolator : public Interpolator<T>
    {
    private:
        /// @brief Matrix of data coefficients
        Matrix<T> A_;

        /// @brief Vector of data free terms (zeros)
        Matrix<T> b_;

        /// @brief Vector of interpolation coefficients
        Matrix<T> c_;

    public:
        TriangularInterpolator()
        {
            Interpolator<T>::method_ = "Triangular";
        };

        TriangularInterpolator(math::Matrix<T> x, math::Matrix<T> y)
        {
            if (x.rows() != y.rows())
            {
                throw(ExceptionNonEqualRowsNum("TriangularInterpolator<T>(x, y): Matrices x and y have non-equal number of rows!"));
            }

            size_t rows = x.rows();

            Interpolator<T>::method_ = "Triangular";
            Interpolator<T>::x_ = x;
            Interpolator<T>::y_ = y;

            A_ = cat(std::vector<math::Matrix<T>>{x, static_cast<T>(1.0) + Matrix<T>(rows, 1)}, Dimension::Column);
            // b_ = Matrix<T>(rows, 1);
            // b_.fill(static_cast<T>(2.0));
            b_ = y;
            c_ = Matrix<T>(rows, 1);
        };

        virtual ~TriangularInterpolator(){};

        virtual void build() override
        {
            Interpolator<T>::solver_->solve(A_, b_, c_);
        }

		virtual void interpolate(const Matrix<T>& x, Matrix<T>& y) override
        {
            if ((x.rows() != Interpolator<T>::x_.rows() - 1) || (x.cols() != 1))
            {
                throw(ExceptionNonEqualRowsNum("TriangularInterpolator<T>::interpolate: Vector x of independent variables has an incorrect rows number!"));
            }

            y = Matrix<T>(Interpolator<T>::y_.cols(), 1);
            for (size_t j = 0; j < x.rows(); ++j)
            {
                y(0, 0) += x(j, 0) * c_(j, 0);
            }
            y(0, 0) += c_(c_.rows() - 1, 0);
        }
};
}