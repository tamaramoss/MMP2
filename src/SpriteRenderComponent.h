/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "IRenderComponent.h"

class SpriteRenderComponent : public IRenderComponent
{
public:
	using ptr = std::shared_ptr<SpriteRenderComponent>;

	SpriteRenderComponent(GameObject& gameObject,
	                      RenderWindow& renderWindow,
	                      std::string textureFilePath);

	bool init() override;

	void update(float deltaTime) override
	{
	}

	void draw() override;

	Sprite& getSprite() { return mSprite; }

	/// Move window over sprite texture
	//void setTextureRect(const IntRect &intRect);

private:
	std::string mTextureFilePath;
	Texture mTexture;
	Sprite mSprite;
};
