/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#include "stdafx.h"

#include "PhysicsManager.h"

#include <iostream>

#include "RigidBodyComponent.h"
#include "PhysicsComponentEvents.h"
#include <Box2D/Dynamics/b2World.h>
#include "SpriteRenderComponent.h"
#include <Box2D/Dynamics/b2Fixture.h>
#include "GameObjectManager.h"
#include <Box2D/Box2D.h>
#include "ColliderComponent.h"

const float PhysicsManager::RATIO(30.0f);
const float PhysicsManager::UNRATIO(1.f / RATIO);

std::shared_ptr<b2World> PhysicsManager::mWorld = std::make_shared<b2World>(b2Vec2(0.f, 9.8f / 200.f)); //~0 gravity

void PhysicsManager::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}

void PhysicsManager::BeginContact(b2Contact* contact)
{
	auto colliderA = static_cast<ColliderComponent*>(contact->GetFixtureA()->GetUserData());
	auto colliderB = static_cast<ColliderComponent*>(contact->GetFixtureB()->GetUserData());
	if (colliderA != nullptr && colliderB != nullptr)
	{
		colliderA->onCollision(*colliderB);
		colliderB->onCollision(*colliderA);
	}
}

void PhysicsManager::EndContact(b2Contact* contact)
{
}


void PhysicsManager::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
}

void PhysicsManager::init()
{
	mWorld->SetContactListener(this);
	// subscribe to events to catch creation events of physics objects
	{
		const auto id = EventBus::getInstance().addListener(
			RigidBodyCreateEvent::mEventType, 
			[this](IEvent::ptr event)
			{
				auto rigidbodyEvent = std::static_pointer_cast<RigidBodyCreateEvent>(event);
				mRigidbodies.push_back(
					&rigidbodyEvent->getData());
			});
		mListeners.push_back(id);
	}
}

void PhysicsManager::shutdown()
{
	for (auto it = mListeners.begin(); it != mListeners.end(); ++it)
		EventBus::getInstance().removeListener(*it);

	std::cout << "Remove physcs" << std::endl;
	
	mListeners.clear();
	mRigidbodies.clear();
}

void PhysicsManager::update(const float deltaTime)
{
	mWorld->Step(deltaTime, 8, 3);

	for (int i = 0; i < mRigidbodies.size(); ++i)
	{
		if (mRigidbodies.at(i)->getB2Body() == nullptr)
		{
			mRigidbodies.erase(mRigidbodies.begin() + i--);
			continue;
		}

		mRigidbodies.at(i)->physicsUpdate(deltaTime);
	}
}

std::shared_ptr<b2World> PhysicsManager::get_b2_world()
{
	return mWorld;
}

b2Body* PhysicsManager::createB2Body(b2BodyDef& def)
{
	return mWorld->CreateBody(&def);
}

void PhysicsManager::destroyB2Body(b2Body& def)
{
	mWorld->DestroyBody(&def);
}

b2Vec2 PhysicsManager::s2b(const Vector2f& vec, const bool scale)
{
	return scale ? b2Vec2(vec.x * UNRATIO, vec.y * UNRATIO) : b2Vec2(vec.x, vec.y);
}

Vector2f PhysicsManager::b2s(const b2Vec2& vec, const bool scale)
{
	return scale ? Vector2f(vec.x, vec.y) * RATIO : Vector2f(vec.x, vec.y);
}

Color PhysicsManager::b2s(const b2Color& color, const int alpha)
{
	return {
		static_cast<Uint8>(color.r * 255), static_cast<Uint8>(color.g * 255), static_cast<Uint8>(color.b * 255),
		static_cast<Uint8>(alpha)
	};
}

b2Color PhysicsManager::s2b(const Color& color, const int alpha)
{
	return {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f};
}
