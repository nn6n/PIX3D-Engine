#pragma once
#include <Platfrom/GL/GLRenderer.h>

namespace PIX3D
{
	struct Sprite
	{
		GL::GLTexture Texture = GL::GLRenderer::GetDefaultWhiteTexture();
		glm::vec4 Color = { 1.0, 1.0, 1.0, 1.0 };
		float TilingFactor = 1.0f;
	};
}
