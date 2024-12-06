#pragma once
#include <PIX3D.h>

class DrawinApplication : public PIX3D::Application
{
	enum ApplicationRendererMode
	{
		SOFTWARE_RENDERER,
		HARDWARE_RENDERER
	};

	struct BrushDot
	{
		glm::vec2 position;
		float size;
		glm::vec4 color;
	};

public:
	DrawinApplication() = default;
	virtual ~DrawinApplication();

	virtual void OnStart() override;
	virtual void OnUpdate() override;
	virtual void OnResize(uint32_t width, uint32_t height) override;

private:
	void DrawCircleAroundMouse(std::vector<uint32_t>& pixels, const glm::vec2 & mousePos, uint32_t width, uint32_t height, uint32_t radius);
private:
	PIX3D::GL::GLFramebuffer m_Framebuffer;
	std::vector<BrushDot> m_Dots;
	glm::vec2 m_LastMousepos;
	float m_BrushSize = 2.0f;
	bool m_MouseOverDrawArea = false;
	glm::vec4 m_BrushColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	bool m_TakeScreenShoot = false;
	glm::vec2 m_MousePosition;
	ApplicationRendererMode m_RenderMode = HARDWARE_RENDERER;
};
