#pragma once

#include "TransformAnimationComponent.h"

/// Performs a constant translation with the given speed and direction.
/// Currently does not support rotation.
class TransformAnimationConstantMotion : public ITransformAnimation
{
public:
	/// \param direction movement direction
	/// \param movement speed
	/// \param condition optional parameter. if set the movement is
	/// only performed when the condition returns true. Can be a
	/// lambda expression.
	TransformAnimationConstantMotion(const Vector2f& direction,
	                                 float speed)
		: m_direction(direction)
		  , m_fSpeed(speed)
	{
	}

	void update(float deltaTime, Transform& transform) override;

private:
	Vector2f m_direction;
	float m_fSpeed;
};
