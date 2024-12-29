#include "Application.h"
#include <Graphics/Transform.h>
#include <Platfrom/GL/GLStaticMeshFactory.h>

#include <imgui.h>

namespace
{
	void RenderTransformComponent(PIX3D::Transform& transform, const std::string& label)
	{
		// Use a unique label or ID scope
		ImGui::PushID(label.c_str());

		ImGui::Text("%s", label.c_str());
		ImGui::Separator();

		// Position
		ImGui::Text("Position");
		ImGui::DragFloat3(("Position##" + label).c_str(), &transform.Position.x, 0.1f, -1000.0f, 1000.0f, "%.3f");

		// Rotation
		ImGui::Text("Rotation");
		ImGui::DragFloat3(("Rotation##" + label).c_str(), &transform.Rotation.x, 0.1f, -360.0f, 360.0f, "%.3f");

		// Scale
		ImGui::Text("Scale");
		ImGui::DragFloat3(("Scale##" + label).c_str(), &transform.Scale.x, 0.1f, 0.0f, 1000.0f, "%.3f");

		ImGui::Separator();

		// Pop the ID scope
		ImGui::PopID();
	}
}

void Application::OnStart()
{
	Cam3D.Init({ 0.0f, 0.0f, 20.0f });

	{
		PIX3D::Timer profiler("Model Loading");
		//Mesh.Load("res/otako/source/model.fbx", 0.1);
		Mesh.Load("res/helmet/DamagedHelmet.gltf", 5.0f);
	}

	{
		PIX3D::Timer profiler("Generate Hdr Cubemap Algrithm in GPU");
		Cubemap.LoadHdrToCubemapGPU("res/hdr/barcelona_rooftop.hdr", 1024);
	}


	{
		PIX3D::Timer profiler("Generate IBL Irradiance And Prefilter Maps Algrithm in GPU");
		IBLMaps = PIX3D::GL::IBLCubemapsGenerator::GenerateIBLMaps(Cubemap.GetHandle(), 32, 128);
	}

	MeshTransform.Scale = glm::vec3(1.0f);
}

void Application::OnUpdate(float dt)
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

	// update
	{
		if(RotateModel)
			MeshTransform.Rotation.y += 10 * dt;
		if(RotateSkybox)
			CubemapTransform.Rotation.y += 10.0f * dt;
	}

	// rendering
	{
		PIX3D::GL::GLRenderer::Begin(Cam3D);

		PIX3D::GL::GLRenderer::RenderMesh(MeshTransform, Mesh, IBLMaps);

		PIX3D::GL::GLRenderer::RenderHdrSkybox(CubemapTransform, Cubemap);

		PIX3D::GL::GLRenderer::End();
	}

	ImGui::Begin("Editor");
	ImGui::CollapsingHeader("model");
	ImGui::Checkbox("RotateModel", &RotateModel);
	ImGui::CollapsingHeader("skybox");
	ImGui::Checkbox("RotateSkybox", &RotateSkybox);
	PIX3D::GL::GLRenderer::RenderPostPorcessingSettingsImGui();
	ImGui::End();
}

void Application::OnResize(uint32_t width, uint32_t height)
{

}
