#include "GLRenderpass.h"
#include "GLCommands.h"
#include <glad/glad.h>

namespace PIX3D
{
	namespace GL
	{

		void GLRenderpass::Init()
		{
            {// Smooth Circle
                std::vector<float> vertices =
                {
                    // positions        // texture coords
                    1.0f,  1.0f, 0.0f,  1.0f, 1.0f,   // top right
                    1.0f, -1.0f, 0.0f,  1.0f, 0.0f,   // bottom right
                   -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,   // bottom left
                   -1.0f,  1.0f, 0.0f,  0.0f, 1.0f    // top left 
                };

                std::vector<uint32_t> indices =
                {
                    0, 1, 3, // first triangle
                    1, 2, 3  // second triangle
                };

                // VBO
                s_ScreenQuadVertexBuffer.Create();
                s_ScreenQuadVertexBuffer.Fill(BufferData::CreatFrom<float>(vertices));

                // EBO
                s_ScreenQuadIndexBuffer.Create();
                s_ScreenQuadIndexBuffer.Fill(indices);

                // VAO
                s_ScreenQuadVertexArray.Create();
                {
                    GLVertexAttrib position;
                    position.ShaderLocation = 0;
                    position.Type = GLShaderAttribType::FLOAT;
                    position.Count = 3;
                    position.Offset = 0;


                    GLVertexAttrib texcoords;
                    texcoords.ShaderLocation = 1;
                    texcoords.Type = GLShaderAttribType::FLOAT;
                    texcoords.Count = 2;
                    texcoords.Offset = 3 * sizeof(float);

                    s_ScreenQuadVertexArray.AddVertexAttrib(position);
                    s_ScreenQuadVertexArray.AddVertexAttrib(texcoords);
                }
                s_ScreenQuadVertexArray.AddVertexBuffer(s_ScreenQuadVertexBuffer, 5 * sizeof(float));
                s_ScreenQuadVertexArray.AddIndexBuffer(s_ScreenQuadIndexBuffer);

                s_ScreenQuadShader.LoadFromFile("../PIX3D/res/gl shaders/screen_quad.vert", "../PIX3D/res/gl shaders/screen_quad.frag");
            }
		}

        void GLRenderpass::ShowScreenRectPass(GLFramebuffer& framebuffer)
        {
            // bind Texture
            glBindTexture(GL_TEXTURE_2D, framebuffer.GetColorAttachmentHandle());

            s_ScreenQuadShader.Bind();

            s_ScreenQuadVertexArray.Bind();
            GLCommands::DrawIndexed(Primitive::TRIANGLES, 6);
        }
	}
}
