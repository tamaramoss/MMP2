#pragma once

#include "IAnimationComponent.h"

#include <SFML/Graphics.hpp>

/// Interface for specific animation implementations
class ITransformAnimation
{
public:
	using ptr = std::shared_ptr<ITransformAnimation>;

	virtual ~ITransformAnimation() = default;

	/// \param fDeltaTime
	/// \param transform the transform to be manipulated
	virtual void update(float deltaTime, Transform& transform) = 0;
};

/// The component controlling transform animations. The specific
/// animation pattern can be handed in via an implementation of
/// TransformAnimation. 
class TransformAnimationComponent : public IAnimationComponent
{
public:

	///
	using ptr = std::shared_ptr<TransformAnimationComponent>;

	/// \param gameObject the game object holding this component
	/// \param transform the transformation component, which is modified
	/// by this component
	TransformAnimationComponent(GameObject& gameObject,
	                            Transform& transform,
	                            ITransformAnimation::ptr animation);

	bool init() override;
	void update(float deltaTime) override;

private:
	Transform& mTransform;

	ITransformAnimation::ptr mAnimation;
};
