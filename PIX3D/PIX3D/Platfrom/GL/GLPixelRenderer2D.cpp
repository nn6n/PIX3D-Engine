#include "GLPixelRenderer2D.h"
#include "GLCommands.h"
#include <glm/gtc/type_ptr.hpp>
#include <Engine/Engine.hpp>

namespace PIX3D
{
	namespace GL
	{
		void GLPixelRenderer2D::Init()
		{
			{// Smooth Circle
                std::vector<float> vertices =
                {
                    // positions         // corrdeinates for drawing in circle ndc
                     1.0f,  1.0f, 0.0f,   1.0f,  1.0f,   1.0f,  1.0f,   // top right
                     1.0f, -1.0f, 0.0f,   1.0f, -1.0f,   1.0f,  0.0f,   // bottom right
                    -1.0f, -1.0f, 0.0f,  -1.0f, -1.0f,   0.0f,  0.0f,  // bottom left
                    -1.0f,  1.0f, 0.0f,  -1.0f,  1.0f,   0.0f,  1.0f   // top left
                };

                std::vector<uint32_t> indices =
                {
                    0, 1, 3, // first triangle
                    1, 2, 3  // second triangle
                };

                // VBO
                s_QuadVertexBuffer.Create();
                s_QuadVertexBuffer.Fill(BufferData::CreatFrom<float>(vertices));

                // EBO
                s_QuadIndexBuffer.Create();
                s_QuadIndexBuffer.Fill(indices);
                
                // VAO
                s_QuadVertexArray.Create();
                {
                    GLVertexAttrib position;
                    position.ShaderLocation = 0;
                    position.Type = GLShaderAttribType::FLOAT;
                    position.Count = 3;
                    position.Offset = 0;


                    GLVertexAttrib coords;
                    coords.ShaderLocation = 1;
                    coords.Type = GLShaderAttribType::FLOAT;
                    coords.Count = 2;
                    coords.Offset = 3 * sizeof(float);

                    GLVertexAttrib uvs;
                    uvs.ShaderLocation = 2;
                    uvs.Type = GLShaderAttribType::FLOAT;
                    uvs.Count = 2;
                    uvs.Offset = 3 * sizeof(float) + 2 * sizeof(float);

                    s_QuadVertexArray.AddVertexAttrib(position);
                    s_QuadVertexArray.AddVertexAttrib(coords);
                    s_QuadVertexArray.AddVertexAttrib(uvs);
                }
                s_QuadVertexArray.AddVertexBuffer(s_QuadVertexBuffer, 7 * sizeof(float));
                s_QuadVertexArray.AddIndexBuffer(s_QuadIndexBuffer);

                s_QuadShader.LoadFromFile("../PIX3D/res/gl shaders/smooth_rounded_quad.vert", "../PIX3D/res/gl shaders/smooth_rounded_quad.frag");
			}
		}
        
        void GLPixelRenderer2D::Begin(const Camera2D& cam)
        {
            s_ProjectionMatrix = cam.GetProjectionMatrix();
        }

        void GLPixelRenderer2D::Begin(const Camera3D& cam)
        {
            s_ProjectionMatrix = cam.GetProjectionMatrix() * cam.GetViewMatrix();
        }
        
        void GLPixelRenderer2D::End()
        {
        }

        void GLPixelRenderer2D::Destory()
        {
            s_QuadVertexArray.Destroy();
            s_QuadVertexBuffer.Destroy();
            s_QuadIndexBuffer.Destroy();
            s_QuadShader.Destroy();
        }

        void GLPixelRenderer2D::DrawSmoothRoundedQuad(float x, float y, float size_x, float size_y, glm::vec4 color, float roundness, float smoothness)
        {
            roundness = glm::clamp(roundness, 0.01f, 1.0f);

            glm::mat4 proj =
                s_ProjectionMatrix *
                glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f)) *
                glm::scale(glm::mat4(1.0f), glm::vec3(size_x, size_y, 1.0f));

            s_QuadShader.Bind();
            s_QuadShader.SetMat4("u_Projection", proj);
            s_QuadShader.SetVec4("u_color", color);
            s_QuadShader.SetFloat("u_smoothness", smoothness);
            s_QuadShader.SetFloat("u_corner_radius", roundness);
            s_QuadShader.SetFloat("u_use_texture", 0.0);

            s_QuadVertexArray.Bind();
            GLCommands::DrawIndexed(Primitive::TRIANGLES, 6);
        }

        void GLPixelRenderer2D::DrawSmoothQuad(float x, float y, float size_x, float size_y, glm::vec4 color, float smoothness)
        {
            DrawSmoothRoundedQuad(x, y, size_x, size_y, color, 0.01f, smoothness);
        }

        void GLPixelRenderer2D::DrawSmoothCircle(float x, float y, float size, glm::vec4 color, float smoothness)
        {
            DrawSmoothRoundedQuad(x, y, size, size, color, 1.0f, smoothness);
        }

        void GLPixelRenderer2D::DrawTexturedQuad(GLTexture texture, float x, float y, float size_x, float size_y, glm::vec4 color)
        {
            glm::mat4 proj =
                s_ProjectionMatrix *
                glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f)) *
                glm::scale(glm::mat4(1.0f), glm::vec3(size_x, size_y, 1.0f));

            s_QuadShader.Bind();
            s_QuadShader.SetMat4("u_Projection", proj);
            s_QuadShader.SetVec4("u_color", color);
            s_QuadShader.SetFloat("u_smoothness", 0.01f);
            s_QuadShader.SetFloat("u_corner_radius", 0.01f);
            s_QuadShader.SetFloat("u_use_texture", 1.0);

            texture.Bind();

            s_QuadVertexArray.Bind();
            GLCommands::DrawIndexed(Primitive::TRIANGLES, 6);
        }
	}
}
