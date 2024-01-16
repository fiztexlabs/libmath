#pragma once
#include <libmath/interpolator/interpolator.h>
#include <libmath/matrix.h>
#include <libmath/math_exception.h>

namespace math
{
    /**
     * @brief Intorpolate points by n-dimension plane
     * @details Polygone interpolator based on the n-dimension plane equation
     * @f[
     * x_0 K_0 + x_1 K_1 + ... + x_n K_n + y K_{n+1} = 0
     * @f],
     * where @f$ \mathbf{x} @f$ - n-dimension vector of dependent variables, @f$ y @f$ - independent variable.
     * This equation can be re-written as
     * @f[
     * x_0 \frac{K_0}{K_{n+1}} + x_1 \frac{K_1}{K_{n+1}} + ... + x_n \frac{K_n}{K_{n+1}} + y = 0
     * @f], or
     * @f[
     * x_0 C_0 + x_1 C_1 + ... + x_n C_n + y = 0
     * @f], 
     * where @f$ C_i = \frac{K_i}{K_{n+1}} @f$
     * Assuming, that @f$ \mathbf{x} @f$ and @f$ y @f$ are known for @f$ n @f$ points, we can build system of
     * @f$ n @f$ linear equation 
     * @f[
     * \begin{cases}
     * x_{0,0} C_0 + x_1 C_{0,1} + ... + x_{0,n} C_n + y_0 = 0
     * ...
     * x_{n,0} C_0 + x_1 C_{n,1} + ... + x_{n,n} C_n + y_n = 0
     * \end{cases}
     * @f]
     * Vector
     * @author Ilya Konovalov
     */
    template <typename T>
    class PolygoneInterpolator : public Interpolator<T>
    {
    private:
        /// @brief Matrix of data coefficients (independent variables points)
        Matrix<T> A_;

        /// @brief Vector of data free terms (dependent variables points)
        Matrix<T> b_;

        /// @brief Vector of interpolation coefficients
        Matrix<T> c_;

    public:
        PolygoneInterpolator() : Interpolator<T>("Polygone") {};

        PolygoneInterpolator(const math::Matrix<T>& x, const math::Matrix<T>& y) : Interpolator<T>("Polygone", x, y)
        {

            size_t rows = x.rows();

            A_ = cat(std::vector<math::Matrix<T>>{x, static_cast<T>(1.0) + Matrix<T>(rows, 1)}, Dimension::Column);
            b_ = y;
            c_ = Matrix<T>(rows, 1);
        };

        virtual ~PolygoneInterpolator(){};

        virtual void build() override
        {
            Interpolator<T>::solver_->solve(A_, b_, c_);
        }

		virtual T interpolate(const Matrix<T>& x) override
        {
            if (x.cols() != Interpolator<T>::dim_ || x.rows() != 1)
            {
                throw(ExceptionNonRowVector(
                    "PolygoneInterpolator<T>::interpolate: Vector x of independent variables must be the row-vector of " +
                    std::to_string(Interpolator<T>::dim_) + " elements!"));
            }

            T y{0};
            for (size_t j = 0; j < x.rows(); ++j)
            {
                y += x(j, 0) * c_(j, 0);
            }
            y += c_(c_.rows() - 1, 0);

            return y;
        }
};
}