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
#include "stdafx.h"
#include "AnimationComponent.h"

#include "GameObject.h"

AnimationComponent::AnimationComponent(GameObject& gameObject, sf::RenderWindow& window, float frameTime, bool paused,
	bool looped)
    : IRenderComponent(gameObject, window),
    mCurrentAnimation(nullptr), mFrameTime(frameTime), mCurrentFrame(0), mIsPaused(paused), mIsLooped(looped)
{
}

bool AnimationComponent::init()
{
    return true;
}

void AnimationComponent::registerAnimation(const std::string& name, std::shared_ptr<Animation> animation)
{
    if (mAnimations.empty())
        mCurrentAnimation = animation.get();
	
    mAnimations[name] = std::move(animation);
}

void AnimationComponent::setAnimation(const std::string& animationName)
{
    Animation* animation = findState(animationName);
    FF_ASSERT_MSG(animation != nullptr, "State could not be found");

    mCurrentAnimation = animation;
    mCurrentFrame = 0;
    setFrame(mCurrentFrame);

    mIsPaused = false;

}

void AnimationComponent::setFrameTime(float time)
{
    mFrameTime = time;
}

void AnimationComponent::play()
{
    mIsPaused = false;
}

void AnimationComponent::pause()
{
    mIsPaused = true;
}

void AnimationComponent::stop()
{
    mIsPaused = true;
    mCurrentFrame = 0;
    setFrame(mCurrentFrame);
}


void AnimationComponent::setLooped(bool looped)
{
    mIsLooped = looped;
}

Animation* AnimationComponent::getCurrentAnimation()
{
    return mCurrentAnimation;
}

Animation* AnimationComponent::getAnimation(std::string animationName)
{
    return findState(animationName);
}

sf::FloatRect AnimationComponent::getLocalBounds() const
{
    sf::IntRect rect = mCurrentAnimation->getFrame(mCurrentFrame);

    float width = static_cast<float>(std::abs(rect.width));
    float height = static_cast<float>(std::abs(rect.height));

    return sf::FloatRect(0.f, 0.f, width, height);
}

sf::FloatRect AnimationComponent::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}

bool AnimationComponent::isLooped() const
{
    return mIsLooped;
}

bool AnimationComponent::isPlaying() const
{
    return !mIsPaused;
}

float AnimationComponent::getFrameTime() const
{
    return mFrameTime;
}

void AnimationComponent::setFrame(int newFrame, bool resetTime)
{
    mCurrentFrame = newFrame;
    if (resetTime)
        mCurrentTime = 0;

    mCurrentAnimation->getSprite().setTextureRect(mCurrentAnimation->getFrame(mCurrentFrame));
}

Animation* AnimationComponent::findState(const std::string& animationName)
{
    const auto animation = mAnimations.find(animationName);
    if (animation != mAnimations.end())
        return animation->second.get();
    return nullptr;
}

void AnimationComponent::changeState(Animation* animation)
{
    if (animation != mCurrentAnimation)
    {
        mCurrentAnimation = animation;
    }
}



void AnimationComponent::update(float deltaTime)
{	
    // if not paused and we have a valid animation
    if (!mIsPaused && mCurrentAnimation)
    {
        // add delta time
        mCurrentTime += deltaTime;

        // if current time is bigger then the frame time advance one frame
        if (mCurrentTime >= mFrameTime)
        {
            // reset time, but keep the remainder
            mCurrentTime = fmod(mCurrentTime, mFrameTime);

            // get next Frame index
            if (mCurrentFrame + 1 < mCurrentAnimation->getSize())
                mCurrentFrame++;
            else
            {
                // animation has ended
                if (!mIsLooped)
                {
                    mIsPaused = true;
                }
                else
                {
                    mCurrentFrame = 0; // reset to start
                }



            }

            // set the current frame, not reseting the time
            setFrame(mCurrentFrame, false);
        }
    }
}

void AnimationComponent::draw()
{
    mCurrentAnimation->getSprite().setTextureRect(mCurrentAnimation->getFrame(mCurrentFrame));
    mRenderWindow.draw(mCurrentAnimation->getSprite(), mGameObject.getTransform());
}
