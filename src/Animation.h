/// modified by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

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
#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

class Animation
{
public:
    using ptr = std::shared_ptr<Animation>;

    Animation(std::string textureFilePath);

    void addFrame(sf::IntRect rect);
    std::size_t getSize() const;
    const sf::IntRect& getFrame(std::size_t n) const;
    sf::Sprite& getSprite() { return mSprite; }
	
private:
    std::vector<sf::IntRect> mFrames;

    std::string mTextureFilePath;
    Texture mTexture;
    Sprite mSprite;
};
