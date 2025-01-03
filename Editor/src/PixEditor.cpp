#include "PixEditor.h"
#include <Graphics/Transform.h>

#include <imgui.h>

void PixEditor::OnStart()
{
	m_LayerManager.init(new LauncherLayer());
	m_LayerManager.OnStart();
}

void PixEditor::OnUpdate(float dt)
{
	m_LayerManager.OnUpdate(dt);
}

void PixEditor::OnDestroy()
{
}

void PixEditor::OnResize(uint32_t width, uint32_t height)
{

}
