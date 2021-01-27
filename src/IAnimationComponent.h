#pragma once

#include "IGameComponent.h"

class IAnimationComponent : public IGameComponent
{
public:
	using ptr = std::shared_ptr<IAnimationComponent>;

	using IGameComponent::IGameComponent;
};
