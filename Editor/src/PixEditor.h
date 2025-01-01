#pragma once

#include "Editor Widgets/LightningWidget.h"
#include "Editor Widgets/InspectorWidget.h"
#include "Editor Widgets/MaterialWidget.h"

using namespace PIX3D;

class PixEditor : public PIX3D::Application
{
public:
	virtual void OnStart() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnDestroy() override;
	virtual void OnResize(uint32_t width, uint32_t height) override;
private:
	PIX3D::Scene * m_Scene;
	
	LightningWidget* m_LightningWidget = nullptr;
	bool m_ShowLightningWidget = false;

	HierarchyWidget* m_HierarchyWidget = nullptr;
	bool m_ShowHierarchyWidget = false;

	InspectorWidget* m_InspectorWidget = nullptr;
	bool m_ShowInspectorWidget = false;

	MaterialWidget* m_MaterialWidget = nullptr;
	bool m_ShowMaterialWidget = false;

	bool ShowMouseCursor = true;
};
