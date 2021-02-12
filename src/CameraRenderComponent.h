/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#pragma once

#include <SFML/Graphics.hpp>

#include "IRenderComponent.h"

/// Updates the camera/view of the renderwindow. This new camera
/// will be used until another CameraRenderComponent changes it.
/// For positioning the translation component of the gameobject 
/// transform is used.
/// Currently does not support rotation.
class CameraRenderComponent : public IRenderComponent
{
public:
	using ptr = std::shared_ptr<CameraRenderComponent>;

	/// \param gameObject \param renderWindow \param view the camera specifications.
	CameraRenderComponent(GameObject& gameObject,
	                      RenderWindow& renderWindow,
	                      View view);

	bool init() override;
	virtual void update(float deltaTime) override;
	void draw() override;

private:
	/// the current camera specifications
	View mView;
	Vector2f mInitialCenter;
};
