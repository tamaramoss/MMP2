#include "stdafx.h"

#include "TransformAnimationConstantMotion.h"

void TransformAnimationConstantMotion::update(const float deltaTime,
                                              Transform& transform)
{
	transform.translate(m_direction * m_fSpeed * deltaTime);
}
