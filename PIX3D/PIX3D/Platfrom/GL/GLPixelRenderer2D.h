#pragma once
#include "GLVertexArray.h"
#include "GLShader.h"
#include <glm/glm.hpp>

namespace PIX3D
{
	namespace GL
	{
		class GLPixelRenderer2D
		{
		public:
			static void Init();
			static void Begin();
			static void End();
			static void Destory();

			static void DrawSmoothCircle_TopLeft(float x, float y, float size, glm::vec4 color);
		private:

			// Smooth Circle
			inline static GLVertexArray s_SmoothCircleVertexArray;
			inline static GLVertexBuffer s_SmoothCircleVertexBuffer;
			inline static GLIndexBuffer s_SmoothCircleIndexBuffer;
			inline static GLShader s_SmoothCircleShader;

			inline static glm::mat4 s_OrthographicMatrix;
		};
	}
}
