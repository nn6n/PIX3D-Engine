#pragma once
#include "GLVertexArray.h"
#include "GLFramebuffer.h"
#include "GLShader.h"

namespace PIX3D
{
	namespace GL
	{
		class GLRenderpass
		{
		public:
			static void Init();
			static void Destroy();
			static void ShowScreenRectPass(GLFramebuffer& framebuffer);
			static void ShowScreenRectPass(uint32_t texturehandle);
		private:
			// Screen Quad Pass
			inline static GLVertexArray s_ScreenQuadVertexArray;
			inline static GLVertexBuffer s_ScreenQuadVertexBuffer;
			inline static GLIndexBuffer s_ScreenQuadIndexBuffer;
			inline static GLShader s_ScreenQuadShader;
		};
	}
}
