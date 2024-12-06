#include "DrawingApplication.h"
#include <imgui.h>

namespace
{
	glm::vec2 Lerp(const glm::vec2& start, const glm::vec2& end, float t)
	{
		return (1.0f - t) * start + t * end;
	}
}

void DrawinApplication::OnStart()
{
	{// Create Framebuffer
		auto ApplicationSpecs = PIX3D::Engine::GetApplicationSpecs();

		PIX3D::GL::GLFramebufferSpecification specs;
		specs.Width = ApplicationSpecs.Width;
		specs.Height = ApplicationSpecs.Height;
		specs.SamplesCount = 8;
		
		m_Framebuffer.Create(specs);
	}
}

void DrawinApplication::OnUpdate()
{
	// Logic

	m_MousePosition = PIX3D::Input::GetMousePosition();

	if (PIX3D::Input::IsMouseButtonPressed(PIX3D::MouseButtonLeft) && m_MouseOverDrawArea)
	{
		m_Dots.push_back({ m_MousePosition, m_BrushSize, m_BrushColor });

		// fill mouse delta area
		glm::vec2 mousedelta = m_MousePosition - m_LastMousepos;
		float distance = glm::length(mousedelta);
		int steps = (int)(distance / m_BrushSize);

		for (int i = 0; i <= steps; i++)
		{
			// Interpolate position
			float t = (float)i / steps;
			glm::vec2 position = Lerp(m_LastMousepos, m_MousePosition, t);

			m_Dots.push_back({ position, m_BrushSize, m_BrushColor });
		}

		m_LastMousepos = m_MousePosition;
	}
	else
	{
		m_LastMousepos = m_MousePosition;
	}



	// Garphics

	m_Framebuffer.Begin();

	PIX3D::GL::GLCommands::ClearFlag(PIX3D::GL::ClearFlags::COLOR_DEPTH);
	PIX3D::GL::GLCommands::Clear(0.2f, 0.2f, 0.2f, 1.0f);

	// draw dots


	PIX3D::GL::GLPixelBatchRenderer2D::Begin();
	
	PIX3D::GL::GLPixelBatchRenderer2D::DrawCircle_TopLeft({ m_MousePosition.x, m_MousePosition.y }, m_BrushSize, m_BrushColor);
	
	for (auto& dot : m_Dots)
	{
		PIX3D::GL::GLPixelBatchRenderer2D::DrawCircle_TopLeft(dot.position, dot.size, dot.color);
	}
	
	PIX3D::GL::GLPixelBatchRenderer2D::End();

	m_Framebuffer.End();

	// UI

	PIX3D::ImGuiLayer::StartDockSpace();


	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("files"))
		{
			if (ImGui::MenuItem("exit")) { PIX3D::Engine::CloseApplication(); }

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}



	ImGui::Begin("Editor");

	ImGui::SliderFloat("Brush Size", &m_BrushSize, 2.0f, 20.0f);
	ImGui::ColorEdit4("BrushColor", &m_BrushColor[0]);
	if (ImGui::Button("Clear"))
		m_Dots.clear();
	if (ImGui::Button("Take Screen Shoot"))
	{
		auto* platform = PIX3D::Engine::GetPlatformLayer();
		
		std::filesystem::path savepath = platform->SaveDialogue(PIX3D::FileDialougeFilter::PNG);

		auto specs = m_Framebuffer.GetFramebufferSpecs();
		platform->ExportImagePNG(savepath.string().c_str(), specs.Width, specs.Height, m_Framebuffer.GetPixels());
	}
	ImGui::Text("Pixel Batch Renderer Data:");
	ImGui::Text(std::format("Batch Count: {}", PIX3D::GL::GLPixelBatchRenderer2D::GetTotalBatchCount()).c_str());
	ImGui::Text(std::format("Draw Calls: {}", PIX3D::GL::GLPixelBatchRenderer2D::GetDrawCalls()).c_str());

	ImGui::Text(std::format("Frame Time: {} ms", PIX3D::Engine::GetDeltaTime() * 1000.0f).c_str());
	ImGui::Text(std::format("Fps: {}", PIX3D::Engine::GetFps()).c_str());

	ImGui::End();
	{
		ImGui::Begin("Debug");
		auto DrawAreaSize = ImGui::GetContentRegionAvail();
		ImGui::Image((ImTextureID)m_Framebuffer.GetColorAttachmentHandle(), DrawAreaSize, { 0, 1 }, { 1, 0 });
		ImGui::End();
	}
	{
		static bool opend = true;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Draw Area", &opend, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

		auto FramebufferSpecs = m_Framebuffer.GetFramebufferSpecs();
		auto DrawWidnowSize = ImGui::GetContentRegionAvail();

		if (DrawWidnowSize.x != FramebufferSpecs.Width || DrawWidnowSize.y != FramebufferSpecs.Height)
		{
			// Resize Framebuffer image
			m_Framebuffer.Resize(DrawWidnowSize.x, DrawWidnowSize.y);
		}

		{ // Correct Mouse Position
			auto windowpos = glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
			m_MousePosition += windowpos;
		}

		m_MouseOverDrawArea = ImGui::IsWindowHovered();

		ImGui::Image((ImTextureID)m_Framebuffer.GetColorAttachmentHandle(), DrawWidnowSize, { 0, 1 }, { 1, 0 });
		
		ImGui::End();
		ImGui::PopStyleVar();
	}

	PIX3D::ImGuiLayer::EndDockSpace();
}

void DrawinApplication::OnResize(uint32_t width, uint32_t height)
{

}

DrawinApplication::~DrawinApplication()
{

}
