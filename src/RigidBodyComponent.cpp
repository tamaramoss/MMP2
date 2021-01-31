#include "stdafx.h"

#include "RigidBodyComponent.h"

#include "GameObject.h"

#include "EventBus.h"
#include "PhysicsComponentEvents.h"
#include <Box2D/Dynamics/b2Body.h>
#include "SpriteRenderComponent.h"
#include "PhysicsManager.h"
#include "DebugDraw.h"
#include <Box2D/Box2D.h>

RigidBodyComponent::RigidBodyComponent(GameObject& gameObject, const b2BodyType type)
	: IGameComponent(gameObject)
	  , mBody(nullptr)
{
	b2BodyDef def;
	def.position = PhysicsManager::s2b(gameObject.getPosition());
	def.type = type;

	mBody = PhysicsManager::createB2Body(def);
	if (!mBody)
		err() << "Error creating rigidbody" << std::endl;

	EventBus::getInstance().fireEvent(std::make_shared<RigidBodyCreateEvent>(*this));
}

RigidBodyComponent::~RigidBodyComponent()
{
	PhysicsManager::destroyB2Body(*mBody);
	mBody = nullptr;
}


void RigidBodyComponent::addVelocity(const Vector2f& velocity) const
{
	auto vel = mBody->GetLinearVelocity();
	vel += PhysicsManager::s2b(velocity);
	mBody->SetLinearVelocity(vel);
}

void RigidBodyComponent::physicsUpdate(float deltaTime) const
{

	const auto pos = getB2Body()->GetPosition();

	mGameObject.setPosition(PhysicsManager::b2s(pos));
	mGameObject.setRotation(getB2Body()->GetAngle() * 180 / b2_pi);

	//Debug Draw
	auto fixture = mBody->GetFixtureList();
	b2AABB aabb;
	aabb.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);
	aabb.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);
	while (fixture != nullptr)
	{
		aabb.Combine(aabb, fixture->GetAABB(0));

		const auto convexshape = fixture->GetShape();
		if (convexshape->GetType() == b2Shape::e_polygon)
		{
			DebugDraw::getInstance().drawPolygonShape(*dynamic_cast<b2PolygonShape*>(convexshape), mGameObject,
				PhysicsManager::s2b(Color::Green));
		}

		fixture = fixture->GetNext();

		DebugDraw::getInstance().drawAABB(&aabb, Color::Green);
		DebugDraw::getInstance().DrawTransform(getB2Body()->GetTransform());
	}
	
}

b2Body* RigidBodyComponent::getB2Body() const
{
	return mBody;
}
