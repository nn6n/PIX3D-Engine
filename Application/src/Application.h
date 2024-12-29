#pragma once
#include <PIX3D.h>

class Application : public PIX3D::Application
{
public:
	virtual void OnStart() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnResize(uint32_t width, uint32_t height) override;
private:
	PIX3D::Camera3D Cam3D;
	PIX3D::StaticMesh Mesh;
	PIX3D::Transform MeshTransform;

	PIX3D::Transform CubemapTransform;
	PIX3D::GL::GLHdriCubemap Cubemap;
	PIX3D::GL::IBLMaps IBLMaps;

	bool RotateModel = false;
	bool RotateSkybox = false;

	bool ShowMouseCursor = true;
};
