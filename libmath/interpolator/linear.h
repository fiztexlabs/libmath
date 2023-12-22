#pragma once
#include <libmath/interpolator/interpolator.h>
#include <libmath/solver/las/lassolver.h>
#include <libmath/solver/las/kholetsky.h>
#include <memory>

namespace math
{

	template <typename T>
	class Linear :
		public Interpolator<T>
	{
	private:
		/// @brief Internal linear solver
		std::unique_ptr<LASsolver<T>> solver{ std::unique_ptr<LASsolver<T>>(new Kholetsky<T>()) };
	public:
		Linear() {
			Interpolator<T>::method_ = "Linear";
		};

		virtual ~Linear() {};
	};
}