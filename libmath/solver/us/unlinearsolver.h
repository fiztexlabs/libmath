#pragma once

#include <libmath/matrix.h>
#include <libmath/boolean.h>
#include <libmath/math_settings.h>
#include <libmath/solver/las/lassolver.h>
#include <libmath/solver/las/kholetsky.h>
#include <libmath/solver/las/bicgstab.h>
#include <functional>
#include <vector>
#include <memory>

#ifdef MATH_OMP_DEFINE
#include <omp.h>
#endif

namespace math
{
	/**
	* @brief Types of stopping criteria.
	* - iterations: Stopping solver by target iterations
	* - tolerance: Stopping solver by target tolerance
	*/
	enum class USStoppingCriteriaType
	{
		iterations,
		tolerance
	};

	/**
	 * @brief Method for evaluating tolerance
	 */
	enum class USToleranceMethod
	{
		absolute,
		relative
	};

	/**
	* @brief Solver settings.
	*/
	struct USsetup
	{
		/// @brief Default constructor
		USsetup()
		{}

		/// @brief Initializer constructor
		USsetup(
			USStoppingCriteriaType criteria_,
			USToleranceMethod tol_method_,
			size_t max_iter_,
			size_t abort_iter_,
			real targetTolerance_,
			real diff_step_,
			int diff_scheme_,
			LASsolver<real>* linearSolver_
		) :
			criteria(criteria_),
			tol_method(tol_method_),
			max_iter(max_iter_),
			abort_iter(abort_iter_),
			targetTolerance(targetTolerance_),
			diff_step(diff_step_),
			diff_scheme(diff_scheme_)
		{
			delete linearSolver;
			linearSolver = linearSolver_->copy();
		}

		/// @brief Copy constructor
		USsetup(const struct USsetup& new_setup) :
			criteria(new_setup.criteria),
			tol_method(new_setup.tol_method),
			max_iter(new_setup.max_iter),
			abort_iter(new_setup.abort_iter),
			targetTolerance(new_setup.targetTolerance),
			diff_step(new_setup.diff_step),
			diff_scheme(new_setup.diff_scheme)
		{
			delete linearSolver;
			linearSolver = new_setup.linearSolver->copy();
		}

		~USsetup()
		{
			delete linearSolver;
			linearSolver = nullptr;

			//std::cout << "Delete ussteup" << std::endl;
		}

		/// @brief Stopping criteria
		USStoppingCriteriaType criteria = USStoppingCriteriaType::tolerance;

		/// @brief Tolerance evaluating method
		USToleranceMethod tol_method = USToleranceMethod::absolute;

		/// @brief Maximum number of internal iterations
		size_t max_iter = 100;

		/// @brief Maximum iterations, befor method will halted. Need to avoid infinity iterations for
		/// tolerance criteria, if oscillations occurs.
		/// @details abort_iter will be ignored if iterations stopping criteria choosen
		size_t abort_iter = 10 * max_iter;

		/// @brief Target tolerance for numerical method
		real targetTolerance = math::settings::DefaultSettings.targetTolerance;

		/// @brief Differential step, @f$ \Delta x = x_i-x_{i-1} @f$, @f$ \Delta x = x_i-x_{i-1} @f$
		real diff_step = 0.001 * math::settings::CurrentSettings.targetTolerance;

		/// @brief Differential scheme
		/// @see math::partialDerivate
		int diff_scheme = 1;

		/// @brief Internal linear system solver
		/// @see LASsolver
		//std::unique_ptr<LASsolver<real>> linearSolver = std::make_unique<BicGStab<real>>();
		LASsolver<real>* linearSolver = new BicGStab<real>();

		/// @brief assignment operator, which correctly copy LAS solver object
		struct USsetup& operator=(const struct USsetup& new_setup)
		{
			criteria = new_setup.criteria;
			tol_method = new_setup.tol_method;
			max_iter = new_setup.max_iter;
			abort_iter = new_setup.abort_iter;
			targetTolerance = new_setup.targetTolerance;
			diff_step = new_setup.diff_step;
			diff_scheme = new_setup.diff_scheme;
			delete linearSolver;
			linearSolver = new_setup.linearSolver->copy();

			return *this;
		}

	};

	/**
	* @brief Base class for unlinear solvers
	* @details Example of usage Secant solver for solving system of unlinear
	* equations and single unlinear equation (see us.example.cpp)
    * @code
	* #include <libmath/solver/us/secant.h>
	* #include <libmath/matrix.h>
	* #include <iostream>
	* #include <omp.h>
	* 
	* int main()
	* {
	* 	// solve system of unlinear equations
	* 
	* 	omp_set_num_threads(4);
	* 
	* 	// vector function F
	* 	std::vector<std::function<double(const math::Matrix<double>&)>> F;
	* 
	* 	F.push_back
	* 	(
	* 		[](const math::Matrix<double>& x)
	* 		{
	* 			return (pow(x(0, 0), 2.0) + pow(x(1, 0), 2.0) - x(2, 0) - 6.0);
	* 		}
	* 	);
	* 	F.push_back
	* 	(
	* 		[](const math::Matrix<double>& x)
	* 		{
	* 			return (x(0, 0) + x(1, 0) * x(2, 0) - 2.0);
	* 		}
	* 	);
	* 	F.push_back
	* 	(
	* 		[](const math::Matrix<double>& x)
	* 		{
	* 			return (x(0, 0) + x(1, 0) + x(2, 0) - 3.0);
	* 		}
	* 	);
	* 
	* 	// define initial values
	* 	math::Matrix<double> x =
	* 	{
	* 		{1.0},
	* 		{1.0},
	* 		{1.0}
	* 	};
	* 
	* 	// create Secant solver with default setup
	* 	math::Secant<double> secant_solver;
	* 
	* 	secant_solver.solve(F, x);
	* 
	* 	std::cout <<"Residual of function F[0] fot solving system of unlinear equations " << F[0](x) << std::endl;
	* 	std::cout <<"Residual of function F[1] fot solving system of unlinear equations " << F[1](x) << std::endl;
	* 	std::cout <<"Residual of function F[2] fot solving system of unlinear equations " << F[2](x) << std::endl;
	* 
	* 	// using US solver for solving single unlinear eqution
	* 
	* 	// single function f
	* 	std::function<double(const math::Matrix<double>&)> f(
	* 		[](const math::Matrix<double>& x)
	* 		{
	* 			return (pow(x(0, 0), 2.0) + pow(x(0, 0), 2.0) - x(0, 0) - 6.0);
	* 		}
	* 	);
	* 
	* 	// define x
	* 	math::Matrix<double> x = { {1.0} };
	* 
	* 	secant_solver.solve({ f }, x);
	* 
	* 	std::cout << "Residual of function f fot solving single unlinear equation " << f(x) << std::endl;
	* 
	* }
    * @endcode
	* Notice, that for child solver methods copy constructor must be defined, because setup structure
	* USsetup contain pointer to LASsolver interface
	*/
	template <typename T, typename = typename std::enable_if<isNumeric<T>>::type>
	class UnlinearSolver
	{
	protected:
		/// @brief Current solver settings
		USsetup currentSetup_;

		/// @brief Method's name
		std::string method_ = "";

		/**
		* @brief Service function for checking input settings
		*/
		void checkInputs(const struct USsetup& setup)
		{
			if (setup.criteria == USStoppingCriteriaType::tolerance)
			{
				if (setup.targetTolerance < 0.0)
				{
					throw(math::ExceptionInvalidValue(method_ + ": Invalid target tolerance. Tolerance must be positive number!"));
				}
				if (setup.targetTolerance == 0.0)
				{
					throw(math::Exception(method_ + ": Invalid target tolerance. Tolerance must be greater than 0!"));
				}
			}
		};
	public:
		/// @brief Default constructor
		UnlinearSolver() {}

		/// @brief Copy constructor
		UnlinearSolver(const UnlinearSolver<T>& solver)
		{
			currentSetup_ = solver.currentSetup_;
			method_ = solver.method_;
		}

		/// @brief Assigns operator
		UnlinearSolver<T>& operator=(const UnlinearSolver<T>& solver)
		{
			currentSetup_ = solver.currentSetup_;
			method_ = solver.method_;

			return *this;
		}

		virtual ~UnlinearSolver() {
			//std::cout << "Delete ussolver" << std::endl;
		};

		/**
		* @brief Method copy current US solver
		* @return new LASsolver
		*/
		virtual UnlinearSolver<T>* copy() = 0;

		/**
		 * @brief Find roots of system @f$ F(x) = 0 @f$
		 * @details For solving system of unlinear equations F must be vector of functions with
		 * size, greater, than 1. If size of vector F and matrix x equal to 1 - single equation
		 * @f$ F[0](x)=0 @f$ will be solved.
		 * @param[in] F: Vector of functions, defines system of unlinear equations of type @f$ f(\mathbf{x}) = 0 @f$
		 * @param[out] x: Column matrix of result roots. Initial value of x used as initial guess for numerical method
		 * @param[in] x_min: Vector of arguments lower bounds
		 * @param[in] x_max: Vector of arguments upper bounds
		 *
		 */
		virtual void solve(const std::vector<std::function<T(
			const Matrix<T>&)>>& F, 
			Matrix<T>& x,
            const Matrix<T> &x_min = Matrix<T>(),
            const Matrix<T> &x_max = Matrix<T>()) const = 0;

		/**
		* @brief Set solver settings
		* @param setup: Solver settings
		*/
		void setupSolver(const struct USsetup& setup)
		{
			checkInputs(setup);

			currentSetup_ = setup;
		};

		/**
		* @brief Get solver settings
		* @param setup[out]: Solver settings
		*/
		void getSolverSetup(struct USsetup& setup) const
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