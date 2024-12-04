#pragma once
#include <PIX3D.h>

class DrawinApplication : public PIX3D::Application
{
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
	PIX3D::GL::GLFramebuffer m_Framebuffer;
	std::vector<BrushDot> m_Dots;
	glm::vec2 m_LastMousepos;
	float m_BrushSize = 2.0f;
	bool m_MouseOverDrawArea = false;
	glm::vec4 m_BrushColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	bool m_TakeScreenShoot = false;
	glm::vec2 m_MousePosition;
};
