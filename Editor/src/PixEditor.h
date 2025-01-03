#pragma once

#include "Editor Widgets/LightningWidget.h"
#include "Editor Widgets/InspectorWidget.h"
#include "Editor Widgets/MaterialWidget.h"
#include "Layers/LauncherLayer.h"

using namespace PIX3D;

class PixEditor : public PIX3D::Application
{
public:
	virtual void OnStart() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnDestroy() override;
	virtual void OnResize(uint32_t width, uint32_t height) override;
	virtual void OnKeyPressed(uint32_t key) override;

	LayerManager m_LayerManager;
};
