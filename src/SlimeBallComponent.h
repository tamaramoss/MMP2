#pragma once
#include "IGameComponent.h"
#include "ColliderComponent.h"

class SlimeBallComponent : public IGameComponent
{
public:
	SlimeBallComponent(GameObject& gameObject);

	// Inherited via IGameComponent
	virtual bool init() override;
	virtual void update(float fDeltaTime) override;
	void onCollision(ColliderComponent& other);
};