#pragma once

#include "Editor Widgets/LightningWidget.h"

using namespace PIX3D;

class PixEditor : public PIX3D::Application
{
public:
	virtual void OnStart() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnResize(uint32_t width, uint32_t height) override;
private:
	Camera3D Cam3D;
	StaticMesh Mesh;
	Transform MeshTransform;

	LightningWidget m_LightningWidget;
	bool m_ShowLightningWidget = false;

	bool RotateModel = false;
	bool RotateSkybox = false;

	bool ShowMouseCursor = true;
};
