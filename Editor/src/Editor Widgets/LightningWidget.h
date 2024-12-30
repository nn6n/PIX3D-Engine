#pragma once
#include <PIX3D.h>

using namespace PIX3D;

class LightningWidget
{
public:
	LightningWidget() = default;
	~LightningWidget() {}

	void OnStart();
	void OnRender(float dt);

public:
	GL::GLHdriCubemap m_Cubemap;
	Transform m_CubemapTransform;
	GL::IBLMaps m_IBLMaps;

	int m_EnvironmentMapSize = 1024;

	friend class GLRenderer;
};
