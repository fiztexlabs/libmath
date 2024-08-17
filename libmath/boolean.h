#pragma once
#include <type_traits>
#include <cmath>
#include <limits>
#include <libmath/math_settings.h>
#include <algorithm>

namespace math
{
	/**
	* @defgroup Boolean Boolean operations
	* @{
	* @brief Define boolean methods
	*/
	/**
	* @brief Comparsion of two numbers with different types
	* @details To compare A and B calculate diff = fabs(A-B). If diff is smaller than n% of max(abs(A),abs(B)) then A and B can be considered equal.
	* n defined as largest machine epsilon of types T and T1.
	* Compared values must be signed values!
	* Example of using in C++:
	* @code
	* #include <includes/matlib.h>
	* #include <iostream>
	* 
	* int main()
	* {
	*     float A = 1.e5;
	*     double B = 1.e5;
	*     std::cout << math::isEqual(A, B) << std::endl;
	* }
	* @endcode
	* @param A: First number
	* @param B: Second number
	* @param eps: Tolerance (libmath targetTolerance by default)
	* @return flag: True if A is equal to B
	* @todo Comparsion of unsigned values (e.g. size_t) - std::abs doesn't work with unsigned values
	*/
	template<typename T, typename T1>
	bool isEqual(T A, T1 B, long double eps = math::settings::CurrentSettings.targetTolerance, ToleranceMode tolerance_representation = ToleranceMode::absolute)
	{
		typedef typename std::conditional<((std::is_floating_point<T>::value&&
			std::is_floating_point<T1>::value) ? sizeof(T) <= sizeof(T1) :
			std::is_floating_point<T1>::value),
			T1, T>::type RudeType;
		
		RudeType diff = static_cast<RudeType>(0.0);

		if (tolerance_representation == ToleranceMode::absolute)
		{
			diff = std::abs(static_cast<RudeType>(A) - static_cast<RudeType>(B));
		}
		if (tolerance_representation == ToleranceMode::relative)
		{
			if ((static_cast<RudeType>(A) == 0.0) && (static_cast<RudeType>(B) != 0.0))
			{
				diff = std::abs((static_cast<RudeType>(A) - static_cast<RudeType>(B)) / static_cast<RudeType>(B));
			}
			if ((static_cast<RudeType>(B) == 0.0) && (static_cast<RudeType>(A) != 0.0))
			{
				diff = std::abs((static_cast<RudeType>(B) - static_cast<RudeType>(A)) / static_cast<RudeType>(A));
			}
			if ((static_cast<RudeType>(B) == 0.0) && (static_cast<RudeType>(A) == 0.0))
			{
				return true;
			}
			else
			{
				RudeType diff1 = std::abs((static_cast<RudeType>(A) - static_cast<RudeType>(B)) / static_cast<RudeType>(B));
				RudeType diff2 = std::abs((static_cast<RudeType>(B) - static_cast<RudeType>(A)) / static_cast<RudeType>(A));

				diff = std::max(diff1, diff2);
			}
;
		}


		//RudeType largest = (abs(B) > abs(A)) ? abs(B) : abs(A);

		if (diff <= static_cast<RudeType>(eps))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/// @brief check T for numeric types
	template<typename T>
	constexpr bool isNumeric =
		std::is_same<T, int>::value ||
		std::is_same<T, int const>::value ||
		std::is_same<T, long int>::value ||
		std::is_same<T, long long>::value ||
		std::is_same<T, unsigned long long>::value ||
		std::is_same<T, long int const>::value ||
		std::is_same<T, unsigned int>::value ||
		std::is_same<T, unsigned int const>::value ||
		std::is_same<T, float>::value ||
		std::is_same<T, float const>::value ||
		std::is_same<T, double>::value ||
		std::is_same<T, double const>::value ||
		std::is_same<T, long double>::value ||
		std::is_same<T, long double const>::value;

	/**
	* @}
	*/
}
