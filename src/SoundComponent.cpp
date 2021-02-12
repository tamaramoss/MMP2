/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#include "stdafx.h"

#include "SoundComponent.h"

SoundComponent::SoundComponent(GameObject& gameObject) : IGameComponent(gameObject)
{
}

void SoundComponent::registerSound(std::string name, std::shared_ptr<sf::SoundBuffer> sound)
{
	mSounds[name] = std::move(sound);
}

bool SoundComponent::init()
{
	return true;
}

void SoundComponent::update(float fDeltaTime)
{
		
}

void SoundComponent::setSound(std::string name)
{
	SoundBuffer* sound = findSound(name);
	FF_ASSERT_MSG(sound != nullptr, "Sound could not be found");

	mSound.setBuffer(*sound);
	mSound.play();
}

sf::SoundBuffer* SoundComponent::getSound(std::string name)
{
	return findSound(name);
}

sf::SoundBuffer* SoundComponent::findSound(const std::string name)
{
	const auto sound = mSounds.find(name);
	if (sound != mSounds.end())
		return sound->second.get();
	return nullptr;
}

void SoundComponent::changeSound(sf::SoundBuffer* sound)
{
	if (sound != mCurrentSoundBuffer)
	{
		mCurrentSoundBuffer = sound;
	}
}
