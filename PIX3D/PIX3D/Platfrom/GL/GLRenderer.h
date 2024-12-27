#pragma once
#include "GLShader.h"
#include "GLStorageBuffer.h"
#include "GLUniformBuffer.h"
#include "IBL/GLHdrCubemap.h"
#include <Graphics/Camera3D.h>
#include <Platfrom/GL/IBL/IBLCubemapsGenerator.h>
#include <Graphics/Mesh.h>

namespace PIX3D
{
	namespace GL
	{
		// uniform buffer
		struct CameraUnifromBufferData
		{
			glm::mat4 CameraProj;
			glm::mat4 CameraView;
			glm::mat4 SkyboxView;
		};

		class GLRenderer
		{

		public:
			static void Init();
			static void Destory();

			static void Begin(Camera3D& cam);
			static void RenderMesh(const glm::mat4& model, StaticMesh& mesh, IBLMaps& ibl_maps);
			static void RenderHdrSkybox(const glm::mat4& model, const GLHdriCubemap& hdrcubemap);
			static void End();

			inline static GLTexture GetDefaultAlbedoTexture() { return s_DefaultAlbedoTexture; }
			inline static GLTexture GetDefaultNormalTexture() { return s_DefaultNormalTexture; }
			
			inline static GLTexture GetDefaultWhiteTexture() { return s_DefaultWhiteTexture; }
			inline static GLTexture GetDefaultBlackTexture() { return s_DefaultBlackTexture; }

		private:
			inline static glm::vec3 s_CameraPosition;

			inline static GLShader s_Model3DShader;
			inline static GLShader s_SkyBoxShader;
			inline static PIX3D::GL::GLUniformBuffer s_CameraUnifromBuffer;

			inline static GLTexture s_DefaultAlbedoTexture;
			inline static GLTexture s_DefaultNormalTexture;
			inline static GLTexture s_DefaultWhiteTexture;
			inline static GLTexture s_DefaultBlackTexture;

			/*
			127, 127, 255
			*/
		};
	}
}
