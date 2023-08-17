#pragma once

#include <libmath/matrix.h>
#include <libmath/boolean.h>
#include <libmath/math_settings.h>
#include <functional>
#include <vector>
#include <omp.h>

namespace math
{
	enum class USStoppingCriteriaType
	{
		iterations,
		tolerance
	};

	struct USsetup
	{
		/// @brief Stopping criteria
		USStoppingCriteriaType criteria = USStoppingCriteriaType::tolerance;

		/// @brief Maximum number of internal iterations
		size_t max_iter = 100;

		/// @brief Target tolerance for numerical method
		real targetTolerance = math::settings::DefaultSettings.targetTolerance;

	};

	template <typename T, typename = typename std::enable_if<isNumeric<T>>::type>
	class UnlinearSolver
	{
	protected:
		/// @brief Current solver settings
		USsetup currentSetup_;

		/// @brief Method's name
		std::string method_ = "";
	public:
		virtual void solve(const std::vector<std::function<T(const Matrix<T>&)>>& F, Matrix<T>& x) = 0;

		/**
		* @brief Set solver settings
		* @param setup: Solver settings
		*/
		virtual void setupSolver(const USsetup& setup) = 0;

		/**
		* @brief Get solver settings
		* @param setup[out]: Solver settings
		*/
		void getSolverSetup(USsetup& setup) const
		{
			setup = currentSetup_;
		};

		/**
		* @brief Get method name
		* @param mathod[out]: Solving method
		*/
		void getMethod(std::string& method) const
		{
			method = method_;
		}
	};
}