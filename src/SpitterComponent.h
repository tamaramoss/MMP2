/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

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

	void spitSlime();
	
	void setMirrored(bool mirrored);
	void setSlimeKinematic(bool kinematic);
	void setSpitterIndex(int index);
	int getSpitterIndex();
	void setPlayer(std::shared_ptr<GameObject> player) { mPlayer = player; }


private:
	std::shared_ptr<GameObject> mPlayer;
	int mSpitterIndex = 0;
	std::vector<GameObject::ptr> mSlimes;
	int mCurrentIndex = 0;
	int mTotalSlimes = 0;
	float mTimeInterval = 2.f;
	float mTimer = 2.f; // starts at 2 so the spitter immediately spits when hitting the trigger
	bool mSpittingAtm = false;
	bool mIsMirrored = false;
	bool mSlimeIsKinematic = false;
};

