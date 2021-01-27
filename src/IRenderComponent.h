#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "IGameComponent.h"

class IRenderComponent : public IGameComponent
{
public:
	using ptr = std::shared_ptr<IRenderComponent>;
	using weak_ptr = std::weak_ptr<IRenderComponent>;

	IRenderComponent(GameObject& gameObject, RenderWindow& renderWindow)
		: IGameComponent(gameObject)
		  , mRenderWindow(renderWindow)
	{
	}

	virtual void draw() = 0;

protected:
	RenderWindow& mRenderWindow;
};
