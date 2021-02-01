#pragma once

#include "IGameComponent.h"
#include <Box2D/Dynamics/b2Body.h>

class RigidBodyComponent : public IGameComponent
{
public:

	RigidBodyComponent(GameObject& gameObject, b2BodyType type);
	~RigidBodyComponent() override;


	void addVelocity(const Vector2f& velocity) const;

	bool init() override { return true; }

	void update(float deltaTime) override
	{
	};

	void physicsUpdate(float deltaTime) const;

	b2Body* getB2Body() const;


private:
	b2Body* mBody;
};
