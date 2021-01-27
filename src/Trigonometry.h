#pragma once

#include <cmath>

namespace MathUtil
{
	/// @addtogroup Math
	/// @{

	/// @struct thor::TrigonometricTraits
	/// @brief Trigonometric traits template
	/// @details This template can be specialized in order to implement trigonometric constants and functions
	///  for arbitrary types. Some of the definitions are required by the vector algebra functions. Thor supplies
	///  predefined specializations for float, double and long double.@n@n
	/// To define your own traits, specialize the template inside namespace @a thor. The full specialization
	///  shall contain the following public static methods. You don't have to define all of them, depending on
	///  the functionality you need. For example, computing the vector length requires sqrt(), while the polar
	///  angle requires arcTan2(). @n
	/// @arg T sin(T deg)
	///	@arg T cos(T deg)
	///	@arg T tan(T deg)
	///	@arg T arcSin(T value)
	///	@arg T arcCos(T value)
	///	@arg T arcTan2(T valY, T valX)
	///	@arg T sqrt(T value)
	///	@arg T %pi()
	///	@arg T radToDeg(T rad)
	///	@arg T degToRad(T deg)
	///
	/// @attention All trigonometric functions take and return degrees, @b NOT radians.
	template <typename T>
	struct TrigonometricTraits
	{
	};

	// Trigonometric traits: Specialization for float
	template <>
	struct TrigonometricTraits<float>
	{
		using Type = float;

		static Type sin(Type deg) { return std::sin(degToRad(deg)); }
		static Type cos(Type deg) { return std::cos(degToRad(deg)); }
		static Type tan(Type deg) { return std::tan(degToRad(deg)); }
		static Type arcSin(Type value) { return radToDeg(asin(value)); }
		static Type arcCos(Type value) { return radToDeg(acos(value)); }
		static Type arcTan2(Type valY, Type valX) { return radToDeg(atan2(valY, valX)); }
		static Type sqrt(Type value) { return std::sqrt(value); }

		static Type pi() { return 3.141592653589793238462643383f; }
		static Type radToDeg(Type rad) { return 180 / pi() * rad; }
		static Type degToRad(Type deg) { return pi() / 180 * deg; }
	};

	// Trigonometric traits: Specialization for double
	template <>
	struct TrigonometricTraits<double>
	{
		using Type = double;

		static Type sin(Type deg) { return std::sin(degToRad(deg)); }
		static Type cos(Type deg) { return std::cos(degToRad(deg)); }
		static Type tan(Type deg) { return std::tan(degToRad(deg)); }
		static Type arcSin(Type value) { return radToDeg(asin(value)); }
		static Type arcCos(Type value) { return radToDeg(acos(value)); }
		static Type arcTan2(Type valY, Type valX) { return radToDeg(atan2(valY, valX)); }
		static Type sqrt(Type value) { return std::sqrt(value); }

		static Type pi() { return 3.141592653589793238462643383; }
		static Type radToDeg(Type rad) { return 180 / pi() * rad; }
		static Type degToRad(Type deg) { return pi() / 180 * deg; }
	};

	// Trigonometric traits: Specialization for long double
	template <>
	struct TrigonometricTraits<long double>
	{
		using Type = long double;

		static Type sin(Type deg) { return std::sin(degToRad(deg)); }
		static Type cos(Type deg) { return std::cos(degToRad(deg)); }
		static Type tan(Type deg) { return std::tan(degToRad(deg)); }
		static Type arcSin(Type value) { return radToDeg(asin(value)); }
		static Type arcCos(Type value) { return radToDeg(acos(value)); }
		static Type arcTan2(Type valY, Type valX) { return radToDeg(atan2(valY, valX)); }
		static Type sqrt(Type value) { return std::sqrt(value); }

		static Type pi() { return 3.141592653589793238462643383l; }
		static Type radToDeg(Type rad) { return 180 / pi() * rad; }
		static Type degToRad(Type deg) { return pi() / 180 * deg; }
	};

	// ---------------------------------------------------------------------------------------------------------------------------


	/// @brief Converts radians to degrees.
	///
	template <typename T>
	T toDegree(T radian)
	{
		return TrigonometricTraits<T>::radToDeg(radian);
	}

	/// @brief Converts degrees to radians.
	///
	template <typename T>
	T toRadian(T degree)
	{
		return TrigonometricTraits<T>::degToRad(degree);
	}

	/// @brief The number %Pi (3.1415...)
	/// @details Pi has the same value as TrigonometricTraits<float>::pi().
	extern const float Pi;

	/// @}
}
