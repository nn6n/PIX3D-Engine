#include "PixEditor.h"
#include <Graphics/Transform.h>

#include <imgui.h>

void PixEditor::OnStart()
{
	Cam3D.Init({ 0.0f, 0.0f, 20.0f });
	m_LightningWidget.OnStart();

	{
		PIX3D::Timer profiler("Model Loading");
		//Mesh.Load("res/otako/source/model.fbx", 0.1);
		Mesh.Load("res/helmet/DamagedHelmet.gltf", 5.0f);
	}

	MeshTransform.Scale = glm::vec3(1.0f);
}

void PixEditor::OnUpdate(float dt)
{
	// update
	{
		Cam3D.Update(dt);


		if (PIX3D::Input::IsKeyPressed(PIX3D::KeyCode::RightShift))
			ShowMouseCursor = false;
		else if (PIX3D::Input::IsKeyPressed(PIX3D::KeyCode::Escape))
			ShowMouseCursor = true;

		PIX3D::Engine::GetPlatformLayer()->ShowCursor(ShowMouseCursor);
	}

	// rendering
	{
		PIX3D::GL::GLRenderer::Begin(Cam3D);

		PIX3D::GL::GLRenderer::RenderMesh(MeshTransform, Mesh, m_LightningWidget.m_IBLMaps);
		PIX3D::GL::GLRenderer::RenderHdrSkybox(m_LightningWidget.m_CubemapTransform, m_LightningWidget.m_Cubemap);

		PIX3D::GL::GLRenderer::End();
	}


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

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	// Render LightningWidget
	if(m_ShowLightningWidget)
		m_LightningWidget.OnRender(dt);
}

void PixEditor::OnResize(uint32_t width, uint32_t height)
{

}
