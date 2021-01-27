#include "stdafx.h"

#include "GameObject.h"
#include "CameraRenderComponent.h"

CameraRenderComponent::CameraRenderComponent(
	GameObject& gameObject,
	RenderWindow& renderWindow,
	View view)
	: IRenderComponent(gameObject, renderWindow)
	  , mView(view)
	  , mInitialCenter(view.getCenter())
{
}

bool CameraRenderComponent::init()
{
	return true;
}

void CameraRenderComponent::draw()
{
}

void CameraRenderComponent::update(float deltaTime)
{
	const auto newCenter = mGameObject.getTransform().transformPoint(mInitialCenter);
	mView.setCenter(newCenter);

	auto view = mView;
	view.setCenter(floor(view.getCenter().x), floor(view.getCenter().y));
	mRenderWindow.setView(view);
}
