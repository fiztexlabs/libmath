#pragma once
#include <libmath/core/matrix.h>
#include <libmath/core/math_exception.h>
#include <vector>
#include <chrono>

namespace math
{
    /**
    * @brief Solver of linear equation systems with biconjugate gradient stabilized method
    * @todo Realization of biconjugate gradient stabilized method
    * @details Solve systems of view @f$ \mathbf{A}\mathbf{x} = \mathbf{b} @f$.
    * @param[in] A: Coefficient matrix of linear system
    * @param[in] b: Column matrix of additional terms of equations
    * @param[in] x0: Initial values of roots
    * @param[in] stop: Stopping criteria (0 - target tolerance, != 0 - max iterations)
    * @param[out] x: Column matrix of roots.
    */
    template<typename T, class = std::enable_if<isNumeric<T>>>
    void bicGStabLS(const Matrix<T>& A, const Matrix<T>& b, const Matrix<T>& x0, Matrix<T>& x, const size_t iters = 0)
    {
        // check inputs
        if (A.cols() != A.rows())
        {
            throw(math::ExceptionNonSquareMatrix("bicGStab: Inconsistent linear system. Matrix A argument must be square!"));
        }
        if (b.cols() > 1)
        {
            throw(ExceptionIncorrectLASMatrix("bicGStab: Matrix b argument must be column matrix!"));
        }
        if (b.rows() != A.rows())
        {
            throw(ExceptionIncorrectLASMatrix("bicGStab: dimensions of arguments A and b didn't agree!"));
        }
        if (x.cols() > 1)
        {
            throw(ExceptionIncorrectLASMatrix("bicGStab: Matrix x argument must be column matrix!"));
        }
        if (x.rows() != A.rows())
        {
            throw(ExceptionIncorrectLASMatrix("bicGStab: dimensions of input argument A and output x didn't agree!"));
        }

        x = x0;
        Matrix<T> x_l = x0;

        Matrix<T> r = b - A* x0;
        Matrix<T> r1 = r;

        Matrix<T> p(b.rows(), 1);
        p.fill(static_cast<T>(0.0));

        Matrix<T> v(b.rows(), 1);
        v.fill(static_cast<T>(0.0));

        Matrix<T> h(b.rows(), 1);
        h.fill(static_cast<T>(0.0));

        Matrix<T> s(b.rows(), 1);

        Matrix<T> t(b.rows(), 1);

        T rho = static_cast<T>(1.0);
        T rho_l = static_cast<T>(1.0);
        T alpha = static_cast<T>(1.0);
        T omega = static_cast<T>(1.0);

        T betta = static_cast<T>(0.0);

        T tmp1 = static_cast<T>(0.0);
        T tmp2 = static_cast<T>(0.0);

        std::vector<T> tmp_E(b.rows(), static_cast<T>(1.0));
        T E = static_cast<T>(1.0);
        T E_l = static_cast<T>(1.0);
        T e = static_cast<T>(math::settings::CurrentSettings.targetTolerance);

        size_t iter_cnt = 0;
        size_t disconv_cnt = 0;

        auto start = std::chrono::steady_clock::now();
        while (E > e)
        {
            if (iters == 0)
            {
                if (iter_cnt > 100)
                {
                    break;
                    //std::cerr << "bicGStab: too many iterations!\n";
                    //Exception exc(Exception::Type::TooManyIterations);
                    //throw(exc);
                }
            }
            else
            {
                if (iter_cnt > iters)
                {
                    E = 0;
                    break;
                }
            }

            rho_l = rho;
            rho = (r1.getTr() * r)(0, 0);
            betta = (rho / rho_l) * (alpha / omega);
            p = r + betta * (p - omega* v);
            v = A * p;
            alpha = rho / (r1.getTr() * v)(0, 0);
            h = x + alpha * p;
            E = (b - A* h).maxElement();
            if (E <= e)
            {
                x = h;
                break;
            }
            s = r - alpha* v;
            t = A * s;
            omega = (t.getTr() * s)(0, 0) / (t.getTr() * t)(0, 0);
            x = h + omega * s;
            E = (b - A* x).maxElement();
            if (E <= e)
            {
                break;
            }
            r = s - omega* t;

            if (E > E_l)
            {
                E_l = E;
                ++disconv_cnt;
                if (disconv_cnt > 10)
                {
                    std::cerr << "bicGStab: iterations didn't converges!\n";
                    //Exception exc(Exception::Type::DisconvergedIterations);
                    //throw(exc);
                }
            }
            else
            {
                disconv_cnt = 0;
            }

            ++iter_cnt;
        }
        auto end = std::chrono::steady_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
    }
}
