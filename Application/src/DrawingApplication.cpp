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
	
	if (m_RenderMode == HARDWARE_RENDERER)
	{

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
	}
	else if(m_RenderMode == SOFTWARE_RENDERER)
	{
		auto pixels = m_Framebuffer.GetPixelsUint32();

		auto FramebufferSpecs = m_Framebuffer.GetFramebufferSpecs();
		
		// clear
		for (size_t i = 0; i < FramebufferSpecs.Width * FramebufferSpecs.Height; i++)
		{
			pixels[i] = 0xFF000000;
		}

		for (auto& dot : m_Dots)
		{
			DrawCircleAroundMouse(pixels, dot.position, FramebufferSpecs.Width, FramebufferSpecs.Height, dot.size);
		}

		m_Framebuffer.SetPixelsUint32(pixels);
	}
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

		if(m_RenderMode == HARDWARE_RENDERER)
			ImGui::Image((ImTextureID)m_Framebuffer.GetColorAttachmentHandle(), DrawWidnowSize, { 0, 1 }, { 1, 0 });
		else if (m_RenderMode == SOFTWARE_RENDERER)
			ImGui::Image((ImTextureID)m_Framebuffer.GetColorAttachmentHandle(), DrawWidnowSize, { 0, 0 }, { 1, 1 });
		
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

void DrawinApplication::DrawCircleAroundMouse(std::vector<uint32_t>& pixels, const glm::vec2& mousePos, uint32_t width, uint32_t height, uint32_t radius)
{
	// Circle color (RGBA)
	uint32_t circleColor = 0xFF00FFFF; // Red with full alpha

	// Calculate the bounds of the rectangle around the mouse (with padding based on radius)
	int minX = std::max(0, static_cast<int>(mousePos.x) - static_cast<int>(radius));
	int maxX = std::min(static_cast<int>(width) - 1, static_cast<int>(mousePos.x) + static_cast<int>(radius));
	int minY = std::max(0, static_cast<int>(mousePos.y) - static_cast<int>(radius));
	int maxY = std::min(static_cast<int>(height) - 1, static_cast<int>(mousePos.y) + static_cast<int>(radius));

	// Iterate over the rectangle around the mouse position
	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			// Calculate the distance from the current pixel to the mouse position
			float dx = x - mousePos.x;
			float dy = y - mousePos.y;
			float distance = std::sqrt(dx * dx + dy * dy);

			// If the pixel is within the circle radius, color it
			if (distance <= radius)
			{
				pixels[y * width + x] = circleColor;
			}
		}
	}
}
