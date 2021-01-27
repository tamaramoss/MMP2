#include "stdafx.h"

#include "GameObject.h"
#include "SpriteRenderComponent.h"
#include <utility>

SpriteRenderComponent::SpriteRenderComponent(
	GameObject& gameObject,
	RenderWindow& renderWindow,
	std::string textureFilePath)
	: IRenderComponent(gameObject, renderWindow)
	  , mTextureFilePath(std::move(textureFilePath))
{
}

bool SpriteRenderComponent::init()
{
	Image image;
	if (!image.loadFromFile(mTextureFilePath))
	{
		err() << "Could not load texture from " << mTextureFilePath << std::endl;
		return false;
	}
	image.createMaskFromColor(Color::Black);
	mTexture.loadFromImage(image);
	mSprite.setTexture(mTexture);

	return true;
}

void SpriteRenderComponent::draw()
{
	// TODO: adapt transform to have pixel precise movement

	mRenderWindow.draw(mSprite, mGameObject.getTransform());
}
