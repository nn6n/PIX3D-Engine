#include "DrawingApplication.h"

namespace
{
	bool SerializePNGOnce = false;

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

	std::cout << "press space key to export framebuffer image\n";
}
void DrawinApplication::OnUpdate()
{
	glm::vec2 mousepos = PIX3D::Input::GetMousePosition();

	if (PIX3D::Input::IsMouseButtonPressed(PIX3D::MouseButtonLeft))
	{
		float size = 5.0f;
		m_Dots.push_back({ mousepos, size });

		// fill mouse delta area
		glm::vec2 mousedelta = mousepos - m_LastMousepos;
		float distance = glm::length(mousedelta);
		int steps = (int)(distance / size);

		for (int i = 0; i <= steps; i++)
		{
			// Interpolate position
			float t = (float)i / steps;
			glm::vec2 position = Lerp(m_LastMousepos, mousepos, t);

			m_Dots.push_back({ position, size });
		}

		m_LastMousepos = mousepos;
	}
	else
	{
		m_LastMousepos = mousepos;
	}




	m_Framebuffer.Begin();

	PIX3D::GL::GLCommands::ClearFlag(PIX3D::GL::ClearFlags::COLOR_DEPTH);
	PIX3D::GL::GLCommands::Clear(0.2f, 0.2f, 0.2f, 1.0f);

	PIX3D::GL::PixelRenderer2D::Begin();

	PIX3D::GL::PixelRenderer2D::DrawSmoothCircle_TopLeft(mousepos.x, mousepos.y, 5.0f, {1.0f, 0.0f, 0.0f, 0.2f});


	{// TODO:: Remove
		auto specs = PIX3D::Engine::GetApplicationSpecs();
		PIX3D::GL::PixelRenderer2D::DrawSmoothCircle_TopLeft(specs.Width * 0.5f, specs.Height * 0.5f, 100.0f, { 0.0f, 1.0f, 0.0f, 1.0f });
	}

	for (auto& dot : m_Dots)
	{
		PIX3D::GL::PixelRenderer2D::DrawSmoothCircle_TopLeft(dot.position.x, dot.position.y, dot.size, { 0.0f, 1.0f, 0.0f, 1.0f });
	}

	PIX3D::GL::PixelRenderer2D::End();

	m_Framebuffer.End();

	if (PIX3D::Input::IsKeyPressed(PIX3D::KeyCode::Space) && !SerializePNGOnce)
	{
		auto specs = m_Framebuffer.GetFramebufferSpecs();
		PIX3D::Engine::GetPlatformLayer()->ExportImagePNG("framebuffer.png", specs.Width, specs.Height, m_Framebuffer.GetPixels());
		SerializePNGOnce = true;
	}

	PIX3D::GL::GLRenderpass::ShowScreenRectPass(m_Framebuffer);
}

DrawinApplication::~DrawinApplication()
{

}
