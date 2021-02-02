#pragma once
#include "GameObject.h"
#include "IGameComponent.h"
#include <vector>

class SpitterComponent : public IGameComponent
{
public:
	SpitterComponent(GameObject& gameObject);
	
	bool init() override;
	void update(float fDeltaTime) override;
	void startSpitting();
	void stopSpitting();
	void addSlime(GameObject::ptr slime);
	void setTimeInterval(float sec);
	
private:
	std::vector<GameObject::ptr> mSlimes;
	int mCurrentIndex;
	int mTotalSlimes;
	float mTimeInterval;
	bool mSpittingAtm = false;

	bool mSlimeIsNonKinematic;
};

