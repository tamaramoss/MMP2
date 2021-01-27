#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>


namespace MathUtil
{
	/// @addtogroup Vectors
	/// @{

	/// @brief Returns the length of the 3D vector.
	///
	template <typename T>
	T length(const Vector3<T>& vector);

	/// @brief Returns the square of @a vector's length.
	/// @details Suitable for comparisons, more efficient than length().
	template <typename T>
	T squaredLength(const Vector3<T>& vector);

	/// @brief Returns a vector with same direction as the argument, but with length 1.
	/// @pre @a vector is no zero vector.
	template <typename T>
	Vector3<T> unitVector(const Vector3<T>& vector);

	/// @brief Returns the angle between the projection of @a vector onto the XY-plane and the X-axis.
	/// @details Example: The vector (1,0,z) corresponds 0 degrees, (0,1,z) corresponds 90 degrees, where z is arbitrary.
	/// @return Angle in degrees in the interval [-180,180].
	/// @pre @a vector is no zero vector.
	template <typename T>
	T polarAngle(const Vector3<T>& vector);

	/// @brief Returns the elevation angle of @a vector.
	/// @details The elevation angle is the angle between the vector and the XY-plane. It has the same sign as @a vector.z.
	/// @return Elevation angle in degrees in the interval [-90,90].
	/// @pre @a vector is no zero vector.
	template <typename T>
	T elevationAngle(const Vector3<T>& vector);

	// ---------------------------------------------------------------------------------------------------------------------------


	/// @brief Computes the dot product of two 3D vectors.
	///
	template <typename T>
	T dotProduct(const Vector3<T>& lhs, const Vector3<T>& rhs);

	/// @brief Computes the cross product of two 3D vectors.
	///
	template <typename T>
	Vector3<T> crossProduct(const Vector3<T>& lhs, const Vector3<T>& rhs);

	/// @brief Returns the component-wise product of @a lhs and @a rhs.
	/// @details Computes <i>(lhs.x*rhs.x, lhs.y*rhs.y, lhs.z*rhs.z)</i>.
	///  Component-wise multiplications are mainly used for scales.
	template <typename T>
	Vector3<T> cwiseProduct(const Vector3<T>& lhs, const Vector3<T>& rhs);

	/// @brief Returns the component-wise quotient of @a lhs and @a rhs.
	/// @details Computes <i>(lhs.x/rhs.x, lhs.y/rhs.y, lhs.z/rhs.z)</i>.
	///  Component-wise divisions are mainly used for scales.
	/// @pre Neither component of @a rhs is zero.
	template <typename T>
	Vector3<T> cwiseQuotient(const Vector3<T>& lhs, const Vector3<T>& rhs);

	// ---------------------------------------------------------------------------------------------------------------------------


	/// @brief Constructs a 3D vector from a 2D vector by setting the Z component to zero.
	///
	template <typename T>
	Vector3<T> toVector3(const Vector2<T>& vector2);

	/// @}
}

#include "VectorAlgebra3D.inl"
