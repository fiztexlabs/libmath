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

        virtual ~Secant() {
            //std::cout << "Delete secant" << std::endl;
        };

        virtual UnlinearSolver<T>* copy() override
        {
            return new Secant<T>(*this);
        }

        virtual void solve(
            const std::vector<std::function<T(const Matrix<T> &)>> &F,
            Matrix<T> &x,
            const Matrix<T> &x_min = Matrix<T>(),
            const Matrix<T> &x_max = Matrix<T>()) const override
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
            Matrix<T> dx(n, 1, UnlinearSolver<T>::currentSetup_.diff_step);
            // dx.fill(static_cast<T>(0.0));

            Matrix<T> df(F.size(), x.rows());

            // residuals column-matrix
            Matrix<T> y(n, 1, 0.0);
            // y.fill(static_cast<T>(0.0));

            // std::vector<T> r(n, static_cast<T>(1.0));
            Matrix<T> r(n, 1, 1);
            T E = static_cast<T>(1.0);
            T r_l = static_cast<T>(1.0);
            T r_c = static_cast<T>(0.0);

            // constrained arguments
            Matrix<T> x_interm = x;

            Matrix<T> x_l = x_interm;

            size_t iter_cnt = 0;

            // stopping criteria
            bool stop = 0;

            while (!stop)
            {
                math::jacobi(F, x_interm, df, UnlinearSolver<T>::currentSetup_.diff_scheme, UnlinearSolver<T>::currentSetup_.diff_step, x_min, x_max);

                // if lower bound defined
                if (!x_min.empty())
                {
                    for (size_t i = 0; i < x_min.rows(); ++i)
                    {
                        x_interm(i, 0) = std::max(x_interm(i, 0), x_min(i, 0) + UnlinearSolver<T>::currentSetup_.diff_step);
                    }
                }
                // if upper bound defined
                if (!x_max.empty())
                {
                    for (size_t i = 0; i < x_max.rows(); ++i)
                    {
                        x_interm(i, 0) = std::min(x_interm(i, 0), x_max(i, 0) - UnlinearSolver<T>::currentSetup_.diff_step);
                    }
                }

                for (size_t i = 0; i < n; ++i)
                {
                    y(i, 0) = -F[i](x_interm);
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

                x_l = x_interm;

                for (size_t i = 0; i < n; ++i)
                {
                    x_interm(i, 0) += dx(i, 0);
                }

                ++iter_cnt;

                // define stopping criteria
                if (UnlinearSolver<T>::currentSetup_.criteria == USStoppingCriteriaType::tolerance)
                {
                    for (size_t i = 0; i < n; ++i)
                    {
                        // x_l = x_interm(i, 0) - dx(i, 0);
                        // r[i] = std::abs((x_l - x_interm(i, 0)) / x_interm(i, 0));
                        r_l = -y(i, 0);
                        r_c = F[i](x_interm);
                        if (UnlinearSolver<T>::currentSetup_.tol_method == USToleranceMethod::absolute)
                        {
                            r(i, 0) = std::abs(r_c);
                        }
                        if (UnlinearSolver<T>::currentSetup_.tol_method == USToleranceMethod::relative)
                        {
                            r(i, 0) = std::abs((r_l - r_c) / r_c);
                        }
                    }
                    // E = *std::max_element(r.begin(), r.end());
                    E = r.maxElement();

                    if (E <= static_cast<T>(UnlinearSolver<T>::currentSetup_.targetTolerance))
                    {
                        stop = 1;
                        x = x_interm;
                    }
                    else
                    {
                        if (iter_cnt > UnlinearSolver<T>::currentSetup_.abort_iter)
                        {
                            x = x_interm;
                            throw(math::ExceptionTooManyIterations("Secant.solve: Solver didn't converge with choosen tolerance. Too many iterations!"));
                        }
                    }
                }
                if (UnlinearSolver<T>::currentSetup_.criteria == USStoppingCriteriaType::iterations)
                {
                    if (iter_cnt > UnlinearSolver<T>::currentSetup_.max_iter)
                    {
                        stop = 1;
                        x = x_interm;
                    }
                }
            }


		}
	};
}