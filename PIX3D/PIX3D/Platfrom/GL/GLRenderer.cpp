#include "GLRenderer.h"
#include "GLCommands.h"
#include "GLStaticMeshFactory.h"
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

namespace PIX3D
{
	namespace GL
	{
		void GLRenderer::Init()
		{
			// Default Albedo Texture
			{
				// RGBA8 Texture
				std::vector<uint8_t> data =
				{
					255, 0, 255, 255,
					255, 0, 255, 255,
					255, 0, 255, 255,
					255, 0, 255, 255
				};

				s_DefaultAlbedoTexture.LoadFromData(data.data(), 2, 2);
			}

			// Default Normal Texture
			{
				// RGBA8 Texture
				std::vector<uint8_t> data =
				{
					127, 127, 255, 255,
					127, 127, 255, 255,
					127, 127, 255, 255,
					127, 127, 255, 255
				};

				s_DefaultNormalTexture.LoadFromData(data.data(), 2, 2);
			}

			// Default White Texture
			{
				// RGBA8 Texture
				std::vector<uint8_t> data =
				{
					255, 255, 255, 255,
					255, 255, 255, 255,
					255, 255, 255, 255,
					255, 255, 255, 255
				};

				s_DefaultWhiteTexture.LoadFromData(data.data(), 2, 2);
			}

			// Default Black Texture
			{
				// RGBA8 Texture
				std::vector<uint8_t> data =
				{
					0, 0, 0, 255,
					0, 0, 0, 255,
					0, 0, 0, 255,
					0, 0, 0, 255
				};

				s_DefaultBlackTexture.LoadFromData(data.data(), 2, 2);
			}

			s_Model3DShader.LoadFromFile("../PIX3D/res/gl shaders/model3d.vert", "../PIX3D/res/gl shaders/model3d.frag");
			s_SkyBoxShader.LoadFromFile("../PIX3D/res/gl shaders/skybox.vert", "../PIX3D/res/gl shaders/skybox.frag");
			s_CameraUnifromBuffer.Create(0);
		}

		void GLRenderer::Destory()
		{
			s_Model3DShader.Destroy();
			s_CameraUnifromBuffer.Destroy();
		}

		void GLRenderer::Begin(Camera3D& cam)
		{
			// remove translation so skybox is always surrounding camera
			glm::mat4 SkyboxView = glm::mat4(glm::mat3(cam.GetViewMatrix()));

			s_Model3DShader.Bind();

			CameraUnifromBufferData data;
			data.CameraProj = cam.GetProjectionMatrix();
			data.CameraView = cam.GetViewMatrix();
			data.SkyboxView = SkyboxView;

			s_CameraUnifromBuffer.Update({ &data, sizeof(data) });

			s_CameraPosition = cam.GetPosition();
		}

		void GLRenderer::RenderMesh(const glm::mat4& model, StaticMesh& mesh, IBLMaps& ibl_maps)
		{
            s_Model3DShader.Bind();

			s_Model3DShader.SetMat4("model", model);

			s_Model3DShader.SetVec3("u_CameraPosition", s_CameraPosition);

			// IBL stuff
			glActiveTexture(GL_TEXTURE0 + 0);
			s_Model3DShader.SetInt("u_DiffuseIrradianceMap", 0);
			ibl_maps.IrradianceMap.Bind();

			glActiveTexture(GL_TEXTURE0 + 1);
			s_Model3DShader.SetInt("u_PrefilteredEnvMap", 1);
			ibl_maps.PrefilterMap.Bind();

			glActiveTexture(GL_TEXTURE0 + 2);
			s_Model3DShader.SetInt("u_BrdfConvolutionMap", 2);
			glBindTexture(GL_TEXTURE_2D, IBLCubemapsGenerator::GetBrdfLUTTextureHandle());

			// draw meshes
			auto submeshes = mesh.GetSubMeshes();
            for (size_t i = 0; i < submeshes.size(); i++)
            {
				auto submesh = submeshes[i];
			
				mesh.GetVertexArray().Bind();
				s_Model3DShader.SetFloat("u_MeshIndex", (float)(i));
				PIX3D::GL::GLCommands::DrawIndexedBaseVertex(PIX3D::GL::Primitive::TRIANGLES, submesh.IndicesCount, submesh.BaseIndex, submesh.BaseVertex);
            }
		}

		void GLRenderer::RenderHdrSkybox(const glm::mat4& model, const GLHdriCubemap& hdrcubemap)
		{
			GLStaticMeshData CubeMesh = GLStaticMeshGenerator::GenerateCube();

			// default depth buffer value is 1.0
			// skybox depth is 1.0 everywhere
			// need equality to make sure skybox passes depth test in default value places
			glDepthFunc(GL_LEQUAL);

			s_SkyBoxShader.Bind();
			s_SkyBoxShader.SetMat4("model", model);

			glActiveTexture(GL_TEXTURE0);
			s_SkyBoxShader.SetInt("u_CubemapTexture", 0);
			hdrcubemap.Bind();

			// draw mesh
			CubeMesh.VertexArray.Bind();
			GLCommands::DrawArrays(Primitive::TRIANGLES, CubeMesh.VerticesCount);

			glDepthFunc(GL_LESS); // go back to default depth comparison

		}

		void GLRenderer::End()
		{

		}
	}
}
