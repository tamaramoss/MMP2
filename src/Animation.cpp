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
#include "Animation.h"

Animation::Animation(std::string textureFilePath) : mTextureFilePath(textureFilePath)
{
	Image image;
	if (!image.loadFromFile(mTextureFilePath))
	{
		err() << "Could not load texture from " << mTextureFilePath << std::endl;
	}
	//image.createMaskFromColor(Color::Black);
	mTexture.loadFromImage(image);
	mSprite.setTexture(mTexture);
}

void Animation::addFrame(sf::IntRect rect)
{
    mFrames.push_back(rect);
}

std::size_t Animation::getSize() const
{
    return mFrames.size();
}

const sf::IntRect& Animation::getFrame(std::size_t n) const
{
    return mFrames[n];
}


