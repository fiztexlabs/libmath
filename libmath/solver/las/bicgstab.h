#pragma once

#include <libmath/solver/las/lassolver.h>
#include <libmath/math_settings.h>
#include <libmath/math_exception.h>
#include <vector>
#include <string>
#include <chrono>

namespace math
{
	/**
	* @brief Class for solving LAS with biconjugate gradient stabilized method
	*/
	template <typename T>
	class BicGStab :
		public LASsolver<T>
	{
	private:
		/**
		* @brief Service function for checking input settings
		*/
		void checkInputs(const LASsetup& setup)
		{
			if (setup.criteria == StoppingCriteriaType::tolerance)
			{
				if (setup.targetTolerance < 0.0)
				{
					throw(math::ExceptionInvalidValue("BicGStab: Invalid target tolerance. Tolerance must be positive number!"));
				}
				if (setup.targetTolerance == 0.0)
				{
					throw(math::Exception("BicGStab: Invalid target tolerance. Tolerance must be greater than 0!"));
				}
			}
		};

	public:
		/// @brief Default constructor
		BicGStab() 
		{
			method_ = "BicGStab";
		};

		/**
		* @brief BicGStab solver constructor.
		* @param setup: Solver settings
		*/
		BicGStab(const LASsetup& setup)
		{
			method_ = "BicGStab";

			checkInputs(setup);

			currentSetup_ = setup;
		}

		/**
		* @brief LASsolver::solve
		* @todo Add stopping criterias
		*/
		virtual void solve(const Matrix<T>& A, const Matrix<T>& b, Matrix<T>& x) override
		{
			// check inputs
			if (A.cols() != A.rows())
			{
				throw(math::ExceptionNonSquareMatrix("BicGStab: Inconsistent linear system. Matrix A argument must be square!"));
			}
			if (b.cols() > 1)
			{
				throw(ExceptionIncorrectLASMatrix("BicGStab: Matrix b argument must be column matrix!"));
			}
			if (b.rows() != A.rows())
			{
				throw(ExceptionIncorrectLASMatrix("BicGStab: dimensions of arguments A and b didn't agree!"));
			}
			if (x.cols() > 1)
			{
				throw(ExceptionIncorrectLASMatrix("BicGStab: Matrix x argument must be column matrix!"));
			}
			if (x.rows() != A.rows())
			{
				throw(ExceptionIncorrectLASMatrix("BicGStab: dimensions of input argument A and output x didn't agree!"));
			}

			Matrix<T> x_l = x;

			Matrix<T> r = b - A * x;
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
				int iters = 0;

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
				p = r + betta * (p - omega * v);
				v = A * p;
				alpha = rho / (r1.getTr() * v)(0, 0);
				h = x + alpha * p;
				E = (b - A * h).maxElement();
				if (E <= e)
				{
					x = h;
					break;
				}
				s = r - alpha * v;
				t = A * s;
				omega = (t.getTr() * s)(0, 0) / (t.getTr() * t)(0, 0);
				x = h + omega * s;
				E = (b - A * x).maxElement();
				if (E <= e)
				{
					break;
				}
				r = s - omega * t;

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

		/// @brief LASsolver::solve
		virtual void setupSolver(const LASsetup& setup) override
		{
			checkInputs(setup);

			currentSetup_ = setup;
		};
	};
}