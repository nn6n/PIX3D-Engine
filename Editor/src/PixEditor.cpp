#include "PixEditor.h"
#include <Graphics/Transform.h>

#include <imgui.h>

void PixEditor::OnStart()
{
	// init scene
	m_Scene = new PIX3D::Scene("IBL Scene");
	m_Scene->OnStart();

	// init editor lightning widget
	m_LightningWidget = new LightningWidget(m_Scene);

	// init editor hierarchy widget
	m_HierarchyWidget= new HierarchyWidget(m_Scene);

	// init editor inspector widget
	m_InspectorWidget = new InspectorWidget(m_Scene, m_HierarchyWidget);

	// load assets
	{
		PIX3D::Timer profiler("Model Loading");
		//Mesh.Load("res/otako/source/model.fbx", 0.1);
		//Mesh.Load("res/helmet/DamagedHelmet.gltf", 5.0f);
		
		MeshTransform.Scale = glm::vec3(1.0f);
	}


}

void PixEditor::OnUpdate(float dt)
{
	// update
	{
		m_Scene->OnUpdate(dt);

		if (PIX3D::Input::IsKeyPressed(PIX3D::KeyCode::RightShift))
			ShowMouseCursor = false;
		else if (PIX3D::Input::IsKeyPressed(PIX3D::KeyCode::Escape))
			ShowMouseCursor = true;

		PIX3D::Engine::GetPlatformLayer()->ShowCursor(ShowMouseCursor);
	}

	// render
	m_Scene->OnRender();


	// MenuBar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Window"))
		{
			ImGui::PushStyleColor(ImGuiCol_Text, m_ShowLightningWidget ? ImVec4(1.0f, 0.0f, 0.0f, 1.0f) : ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
			if (ImGui::MenuItem("Lightning"))
			{
				m_ShowLightningWidget = !m_ShowLightningWidget;
			}
			ImGui::PopStyleColor();

			ImGui::PushStyleColor(ImGuiCol_Text, m_ShowHierarchyWidget ? ImVec4(1.0f, 0.0f, 0.0f, 1.0f) : ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
			if (ImGui::MenuItem("Hierarchy"))
			{
				m_ShowHierarchyWidget = !m_ShowHierarchyWidget;
			}
			ImGui::PopStyleColor();

			ImGui::PushStyleColor(ImGuiCol_Text, m_ShowInspectorWidget? ImVec4(1.0f, 0.0f, 0.0f, 1.0f) : ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
			if (ImGui::MenuItem("Inspector"))
			{
				m_ShowInspectorWidget = !m_ShowInspectorWidget;
			}
			ImGui::PopStyleColor();

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	// Render LightningWidget
	if(m_ShowLightningWidget)
		m_LightningWidget->OnRender();

	// Render HierarchyWidget
	if (m_ShowHierarchyWidget)
		m_HierarchyWidget->OnRender();

	// Render InspectorWidget
	if (m_ShowInspectorWidget)
		m_InspectorWidget->OnRender();
}

void PixEditor::OnDestroy()
{
	delete m_LightningWidget;
	m_LightningWidget = nullptr;

	delete m_Scene;
	m_Scene = nullptr;
}

void PixEditor::OnResize(uint32_t width, uint32_t height)
{

}
