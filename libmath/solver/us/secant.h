#pragma once

#include <libmath/solver/us/unlinearsolver.h>
#include <libmath/differential.h>
#include <functional>
#include <vector>

namespace math
{
    /**
    * @brief Solver for unlinear equation with secant method (Newton)
    * @details Secant method can solve systems of unlinear equations as well
    * as single unlinear equations. See us.example.cpp
    */
	template<typename T>
	class Secant :
		public UnlinearSolver<T>
	{
	public:
		Secant()
		{
			UnlinearSolver<T>::method_ = "Secant";
		};

		Secant(const struct USsetup& setup)
		{
			UnlinearSolver<T>::method_ = "Secant";

            UnlinearSolver<T>::checkInputs(setup);

            UnlinearSolver<T>::currentSetup_ = setup;
		};

        /// @brief Copy constructor
        Secant(const Secant& uss)
        {
            UnlinearSolver<T>::method_ = uss.method_;
            UnlinearSolver<T>::currentSetup_ = uss.currentSetup_;
        }

        virtual ~Secant() {};

        virtual UnlinearSolver<T>* copy() override
        {
            return new Secant<T>(*this);
        }

		virtual void solve(const std::vector<std::function<T(const Matrix<T>&)>>& F, Matrix<T>& x) override
		{
            // check inputs
            if (x.cols() > 1)
            {
                throw(math::ExceptionIncorrectMatrix("Secant: Matrix x argument must be column matrix!"));
            }
            if (x.rows() != F.size())
            {
                throw(math::ExceptionIncorrectMatrix("Secant: Dimensions of input argument F and output x didn't agree!"));
            }

            size_t n = F.size();
            Matrix<T> dx(n, 1);
            dx.fill(static_cast<T>(0.0));

            Matrix<T> df(F.size(), x.rows());

            // residuals column-matrix
            Matrix<T> y(n, 1);
            y.fill(static_cast<T>(0.0));

            std::vector<T> r(n, static_cast<T>(1.0));
            T E = static_cast<T>(1.0);
            T x_l = static_cast<T>(x(0, 0));

            size_t iter_cnt = 0;

            // stopping criteria
            bool stop = 0;

            while (!stop)
            {
                math::jacobi(F, x, df, UnlinearSolver<T>::currentSetup_.diff_scheme, UnlinearSolver<T>::currentSetup_.diff_step);

                for (size_t i = 0; i < n; ++i)
                {
                    y(i, 0) = -F[i](x);
                }

                // solve system
                if (df.numel() > 1)
                {
                    UnlinearSolver<T>::currentSetup_.linearSolver->solve(df, y, dx);
                }

                // solve single equation
                if (df.numel() == 1)
                {
                    dx(0, 0) = y(0, 0) / df(0, 0);
                }

                for (size_t i = 0; i < n; ++i)
                {
                    x(i, 0) += dx(i, 0);
                }

                ++iter_cnt;

                // define stopping criteria
                if (UnlinearSolver<T>::currentSetup_.criteria == USStoppingCriteriaType::tolerance)
                {
                    for (size_t i = 0; i < n; ++i)
                    {
                        x_l = x(i, 0) - dx(i, 0);
                        r[i] = std::abs((x_l - x(i, 0)) / x(i, 0));
                    }
                    E = *std::max_element(r.begin(), r.end());

                    if (E <= static_cast<T>(UnlinearSolver<T>::currentSetup_.targetTolerance))
                    {
                        stop = 1;
                    }
                    else
                    {
                        if (iter_cnt > UnlinearSolver<T>::currentSetup_.abort_iter)
                        {
                            throw(math::ExceptionTooManyIterations("Secant.solve: Solver didn't converge with choosen tolerance. Too many iterations!"));
                        }
                    }
                }
                if (UnlinearSolver<T>::currentSetup_.criteria == USStoppingCriteriaType::iterations)
                {
                    if (iter_cnt > UnlinearSolver<T>::currentSetup_.max_iter)
                    {
                        stop = 1;
                    }
                }
            }


		}
	};
}