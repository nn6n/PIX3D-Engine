#pragma once
#include <glm/glm.hpp>

namespace PIX3D
{
	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D() {}

		inline glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }

	private:
		glm::mat4 m_ProjectionMatrix;
	};
}
