#pragma once

#include <libmath/matrix.h>
#include <libmath/math_settings.h>
#include <string>

namespace math
{
	/**
	* @brief Types of stopping criteria.
	* - iterations: Stopping solver by target iterations
	* - tolerance: Stopping solver by target tolerance
	*/
	enum class StoppingCriteriaType
	{
		iterations,
		tolerance
	};

	/**
	* @brief Solver settings.
	*/
	struct LASsetup
	{
		/// @brief Stopping criteria
		StoppingCriteriaType criteria = StoppingCriteriaType::tolerance;

		/// @brief Maximum number of internal iterations
		size_t max_iter = 100;

		/// @brief Target tolerance for numerical method
		real targetTolerance = math::settings::DefaultSettings.targetTolerance;
	};

	/**
	* @brief Interfacial class for solving linear algebraic systems (LAS) of view 
	* of view @f$ \mathbf{A}\mathbf{x} = \mathbf{b} @f$
	*/
	template <typename T>
	class LASsolver
	{
	protected:
		/// @brief Current solver settings
		LASsetup currentSetup_;

		/// @brief Method's name
		std::string method_ = "";
	public:

		/**
		* @brief Solve LAS
		* @param A[in]: Coefficients matrix
		* @param b[in]: Column-vector of equations right-hands
		* @param x[out]: Column vector of solution. Initial value of x used
		* as initial guess for methods, that requires initial gues values
		*/
		virtual void solve(const Matrix<T>& A, const Matrix<T>& b, Matrix<T>& x) = 0;

		/**
		* @brief Set solver settings
		* @param setup: Solver settings
		*/
		virtual void setupSolver(const LASsetup& setup) = 0;

		/**
		* @brief Get solver settings
		* @param setup[out]: Solver settings
		*/
		void getSolverSetup(LASsetup& setup)
		{
			setup = currentSetup_;
		};

		/**
		* @brief Get method name
		* @param mathod[out]: Solving method
		*/
		void getMethod(std::string& method)
		{
			method = method_;
		}
	};

}