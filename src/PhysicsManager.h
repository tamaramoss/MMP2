#pragma once

#include <list>

#include "EventBus.h"
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2World.h>
#include "SpriteRenderComponent.h"

class ColliderComponent;
class RigidBodyComponent;

class PhysicsManager : public b2ContactListener
{
public:
	struct Manifold
	{
		Vector2f normal;
		float penetration{};
		ColliderComponent* colliderA{};
		ColliderComponent* colliderB{};
	};

	// Box2D Callbacks
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

	void init();
	void shutdown();
	void update(float fDeltaTime);

	//Physic Globals
	static std::shared_ptr<b2World> get_b2_world();
	static b2Body* createB2Body(b2BodyDef& def);
	static void destroyB2Body(b2Body& def);


	static b2Vec2 s2b(const Vector2f& vec, bool scale = true);
	static Vector2f b2s(const b2Vec2& vec, bool scale = true);
	static Color b2s(const b2Color& color, int alpha = 255);
	static b2Color s2b(const Color& color, int alpha = 255);


	// 1 meter (box2d) is more or less 64 pixels (sfml) == pixels per meter
	static const float PhysicsManager::RATIO;

	// 64 pixels (sfml) are more or less 1 meter (box2d) == meters per pixel
	static const float PhysicsManager::UNRATIO;

	//formula to convert radians to degrees = (radians * (pi/180))
	static const float PhysicsManager::RADTODEG;

private:


	// maintain a list of listeners that the manager is subscribed to,
	// so that he can unsubscribe
	std::list<EventBus::ListenerId> mListeners;

	static std::shared_ptr<b2World> mWorld;

	std::vector<RigidBodyComponent*> mRigidbodies;
};
