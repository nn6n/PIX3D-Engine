#include "GLRenderer.h"
#include "GLCommands.h"
#include <glm/gtc/type_ptr.hpp>

namespace PIX3D
{
	namespace GL
	{
		void GLRenderer::Init()
		{
			s_DefaultTexture.LoadFromFile("../PIX3D/res/default_texture.jpg");

			s_Model3DShader.LoadFromFile("../PIX3D/res/gl shaders/model3d.vert", "../PIX3D/res/gl shaders/model3d.frag");
			s_CameraUnifromBuffer.Create(0);
			s_MaterialStorageBuffer.Create(1);
		}

		void GLRenderer::Destory()
		{
			s_Model3DShader.Destroy();
			s_CameraUnifromBuffer.Destroy();
			s_MaterialStorageBuffer.Destroy();
		}

		void GLRenderer::Begin(glm::mat4 cam_proj)
		{
			s_Model3DShader.Bind();
			s_CameraUnifromBuffer.Update({ glm::value_ptr(cam_proj), sizeof(glm::mat4) });
		}

		void GLRenderer::RenderMesh(glm::mat4 model, const StaticMesh& mesh)
		{
            s_Model3DShader.Bind();

			s_Model3DShader.SetMat4("model", model);

            // set material shader storage buffer data
            std::vector<uint64_t> TextureVramIDs;
            auto materials =  mesh.GetMaterials();            
            for(auto& mat : materials)
                TextureVramIDs.push_back(/*mat.BaseColorTexture.GetVramTextureID()*/s_DefaultTexture.GetVramTextureID());

            s_MaterialStorageBuffer.Update(BufferData::CreatFrom<uint64_t>(TextureVramIDs));


			auto submeshes = mesh.GetSubMeshes();
            for (size_t i = 0; i < submeshes.size(); i++)
            {
				auto submesh = submeshes[i];
			
				mesh.GetVertexArray().Bind();
				s_Model3DShader.SetFloat("u_MeshIndex", (float)(i));
				s_Model3DShader.SetFloat("u_TilinhFactor", 1.0f);
				PIX3D::GL::GLCommands::DrawIndexedBaseVertex(PIX3D::GL::Primitive::TRIANGLES, submesh.IndicesCount, submesh.BaseIndex, submesh.BaseVertex);
            }
		}

		void GLRenderer::End()
		{

		}
	}
}
