#pragma once
#include "GLShader.h"
#include "GLStorageBuffer.h"
#include "GLUniformBuffer.h"
#include <Graphics/Mesh.h>

namespace PIX3D
{
	namespace GL
	{
		class GLRenderer
		{

		public:
			static void Init();
			static void Destory();

			static void Begin(glm::mat4 cam_proj);
			static void RenderMesh(glm::mat4 model, const StaticMesh& mesh);
			static void End();

			inline static GLTexture GetDefaultTexture() { return s_DefaultTexture; }

		private:
			inline static GLShader s_Model3DShader;
			inline static GL::GLStorageBuffer s_MaterialStorageBuffer;
			inline static PIX3D::GL::GLUniformBuffer s_CameraUnifromBuffer;
			inline static GLTexture s_DefaultTexture;
		};
	}
}
