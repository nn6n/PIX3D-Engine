#include "Camera2D.h"
#include <Engine/Engine.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PIX3D
{
	Camera2D::Camera2D()
	{
        auto specs = PIX3D::Engine::GetApplicationSpecs();
        m_ProjectionMatrix = glm::ortho(
            0.0f,                   // Left
            static_cast<float>(specs.Width),  // Right
            static_cast<float>(specs.Height), // Bottom (flipped)
            0.0f,                   // Top
            -1.0f, 1.0f             // Near and far planes
        );
	}
}
