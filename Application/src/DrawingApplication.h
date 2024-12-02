#pragma once
#include <PIX3D.h>

class DrawinApplication : public PIX3D::Application
{
	struct BrushDot
	{
		glm::vec2 position;
		float size;
	};

public:
	DrawinApplication() = default;
	virtual ~DrawinApplication();

	virtual void OnStart() override;
	virtual void OnUpdate() override;
private:
	PIX3D::GL::GLFramebuffer m_Framebuffer;
	std::vector<BrushDot> m_Dots;
	glm::vec2 m_LastMousepos;
};
