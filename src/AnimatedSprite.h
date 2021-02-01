////////////////////////////////////////////////////////////
//
// Copyright (C) 2014 Maximilian Wagenbach (aka. Foaly) (foaly.f@web.de)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
// you must not claim that you wrote the original software.
// If you use this software in a product, an acknowledgment
// in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
// and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#pragma once


#include <unordered_map>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

#include "Animation.h"
#include "SpriteRenderComponent.h"

class AnimatedSprite : public IRenderComponent, public sf::Transformable
{
public:
	explicit AnimatedSprite(GameObject& gameObject, sf::RenderWindow& window,float frameTime = 0.2f, bool paused = false, bool looped = true);

    bool init() override;
	void registerAnimation(const std::string& name, std::shared_ptr<Animation> animation);

    /// Changes to the given state
    void setAnimation(const std::string& animationName);

    void update(float deltaTime) override;
    void draw() override;


    void setFrameTime(float time);
    void play();
    void pause();
    void stop();
    void setLooped(bool looped);

    Animation* getCurrentAnimation();
    Animation* getAnimation(std::string animationName);
    sf::FloatRect getLocalBounds() const;
    sf::FloatRect getGlobalBounds() const;
    bool isLooped() const;
    bool isPlaying() const;
    float getFrameTime() const;
    void setFrame(int newFrame, bool resetTime = true);
	

private:
    Animation* findState(const std::string& animationName);
    void changeState(Animation* animation);
    Animation* mCurrentAnimation = nullptr;
    float mFrameTime;
    float mCurrentTime;
    std::size_t mCurrentFrame;
    bool mIsPaused;
    bool mIsLooped;

    std::unordered_map<std::string, std::shared_ptr<Animation>> mAnimations;
};
