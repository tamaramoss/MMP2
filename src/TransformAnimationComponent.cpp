#include "stdafx.h"

#include "TransformAnimationComponent.h"
#include <utility>
#include <utility>

#include "GameObject.h"

TransformAnimationComponent::TransformAnimationComponent(
	GameObject& gameObject,
	Transform& transform,
	ITransformAnimation::ptr animation)
	: IAnimationComponent(gameObject)
	  , mTransform(transform)
	  , mAnimation(std::move(std::move(animation)))
{
}

bool TransformAnimationComponent::init()
{
	if (mAnimation == nullptr)
	{
		FF_ERROR_MSG("TransformAnimation must not be nullptr")
		return false;
	}
	return true;
}

void TransformAnimationComponent::update(const float deltaTime)
{
	FF_ASSERT_MSG(mAnimation != nullptr, "Animation must not be nullptr");

	mAnimation->update(deltaTime, mTransform);
}
