#pragma once

#include <SFML/System/Vector2.hpp>

namespace MathUtil
{
	/// @addtogroup Vectors
	/// @{

	/// @brief Returns the length of the 2D vector.
	///
	template <typename T>
	T length(const Vector2<T>& vector);

	/// @brief Returns the square of @a vector's length.
	/// @details Suitable for comparisons, more efficient than length().
	template <typename T>
	T squaredLength(const Vector2<T>& vector);

	/// @brief Adapts @a vector so that its length is |@a newLength| after this operation.
	/// @details If @a newLength is less than zero, the vector's direction changes.
	/// @pre @a vector is no zero vector.
	template <typename T>
	void setLength(Vector2<T>& vector, T newLength);

	/// @brief Returns a vector with same direction as the argument, but with length 1.
	/// @pre @a vector is no zero vector.
	template <typename T>
	Vector2<T> unitVector(const Vector2<T>& vector);

	/// @brief Returns the polar angle.
	/// @details The vector (1,0) corresponds 0 degrees, (0,1) corresponds 90 degrees.
	/// @return Angle in degrees in the interval [-180,180].
	/// @pre @a vector is no zero vector.
	template <typename T>
	T polarAngle(const Vector2<T>& vector);

	/// @brief Sets the polar angle of the specified vector.
	/// @details The vector (1,0) corresponds 0 degrees, (0,1) corresponds 90 degrees.
	template <typename T>
	void setPolarAngle(Vector2<T>& vector, T newAngle);

	/// @brief Rotates the vector by the given angle (in degrees).
	/// @details The vector (1,0) corresponds 0 degrees, (0,1) corresponds 90 degrees.
	template <typename T>
	void rotate(Vector2<T>& vector, T angle);

	/// @brief Returns a copy of the vector, rotated by @a angle degrees.
	/// @details The vector (1,0) corresponds 0 degrees, (0,1) corresponds 90 degrees.
	template <typename T>
	Vector2<T> rotatedVector(const Vector2<T>& vector, T angle);

	/// @brief Returns a perpendicular vector.
	/// @details Returns @a vector turned by 90 degrees counter clockwise; (x,y) becomes (-y,x).
	///  For example, the vector (1,0) is transformed to (0,1).
	template <typename T>
	Vector2<T> perpendicularVector(const Vector2<T>& vector);

	// ---------------------------------------------------------------------------------------------------------------------------


	/// @brief Computes the signed angle from @a lhs to @a rhs.
	/// @return Angle in degrees in the interval [-180,180]. The angle determines how much you have to turn @a lhs
	///  until it points to the same direction as @a rhs.
	/// @pre Neither @a lhs nor @a rhs is a zero vector.
	template <typename T>
	T signedAngle(const Vector2<T>& lhs, const Vector2<T>& rhs);

	/// @brief Computes the dot product of two 2D vectors.
	///
	template <typename T>
	T dotProduct(const Vector2<T>& lhs, const Vector2<T>& rhs);

	/// @brief Computes the cross product of two 2D vectors (Z component only).
	/// @details Treats the operands as 3D vectors, computes their cross product and returns the result's Z component
	///  (X and Y components are always zero).
	template <typename T>
	T crossProduct(const Vector2<T>& lhs, const Vector2<T>& rhs);

	/// @brief Returns the component-wise product of @a lhs and @a rhs.
	/// @details Computes <i>(lhs.x*rhs.x, lhs.y*rhs.y)</i>. Component-wise multiplications are mainly used for scales.
	template <typename T>
	Vector2<T> cwiseProduct(const Vector2<T>& lhs, const Vector2<T>& rhs);

	/// @brief Returns the component-wise quotient of @a lhs and @a rhs.
	/// @details Computes <i>(lhs.x/rhs.x, lhs.y/rhs.y)</i>. Component-wise divisions are mainly used for scales.
	/// @pre Neither component of @a rhs is zero.
	template <typename T>
	Vector2<T> cwiseQuotient(const Vector2<T>& lhs, const Vector2<T>& rhs);

	/// @brief Returns the projection of @a vector onto @a axis.
	/// @param vector Vector to project onto another.
	/// @param axis Vector being projected onto. Need not be a unit vector, but may not have length zero.
	template <typename T>
	Vector2<T> projectedVector(const Vector2<T>& vector, const Vector2<T>& axis);

	/// @}
}

#include "VectorAlgebra2D.inl"
