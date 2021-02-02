#pragma once
#include <unordered_map>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "IGameComponent.h"


class SoundComponent : public IGameComponent
{
public:
	SoundComponent(GameObject& gameObject);

	void registerSound(std::string name, std::shared_ptr<sf::SoundBuffer> sound);
	
	bool init() override;
	void update(float fDeltaTime) override;

	void setSound(std::string name);

	sf::SoundBuffer* getCurrentSound() { return mCurrentSoundBuffer; }
	sf::SoundBuffer* getSound(std::string name);
private:
	sf::SoundBuffer* findSound(const std::string name);
	void changeSound(sf::SoundBuffer* sound);

	
	std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> mSounds;
	sf::SoundBuffer* mCurrentSoundBuffer = nullptr;
	sf::Sound mSound;
};
