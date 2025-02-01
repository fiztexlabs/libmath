#pragma once
#include <libmath/math_settings.h>
#include <libmath/boolean.h>
#include <type_traits>

namespace math
{
	template <typename T, typename = typename std::enable_if<isNumeric<T>>::type>
	class Triangulator
	{

	};
}