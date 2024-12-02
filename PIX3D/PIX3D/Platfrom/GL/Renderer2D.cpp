#include "Renderer2D.h"
#include "GLCommands.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Engine/Engine.hpp>

namespace PIX3D
{
	namespace GL
	{
		void PixelRenderer2D::Init()
		{
			{// Smooth Circle
                std::vector<float> vertices =
                {
                    // positions       // corrdeinates for drawing in circle ndc
                     1.0f,  1.0f, 0.0f,  1.0f, 1.0f,   // top right
                     1.0f, -1.0f, 0.0f, -1.0f, 1.0f,   // bottom right
                    -1.0f, -1.0f, 0.0f, -1.0f, -1.0f,  // bottom left
                    -1.0f,  1.0f, 0.0f,  1.0f, -1.0f   // top left 
                };

                std::vector<uint32_t> indices =
                {
                    0, 1, 3, // first triangle
                    1, 2, 3  // second triangle
                };

                // VBO
                s_SmoothCircleVertexBuffer.Create();
                s_SmoothCircleVertexBuffer.Fill(BufferData::CreatFrom<float>(vertices));

                // EBO
                s_SmoothCircleIndexBuffer.Create();
                s_SmoothCircleIndexBuffer.Fill(indices);
                
                // VAO
                s_SmoothCircleVertexArray.Create();
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

                    s_SmoothCircleVertexArray.AddVertexAttrib(position);
                    s_SmoothCircleVertexArray.AddVertexAttrib(coords);
                }
                s_SmoothCircleVertexArray.AddVertexBuffer(s_SmoothCircleVertexBuffer, 5 * sizeof(float));
                s_SmoothCircleVertexArray.AddIndexBuffer(s_SmoothCircleIndexBuffer);

                s_SmoothCircleShader.LoadFromFile("../PIX3D/res/gl shaders/smooth_circle.vert", "../PIX3D/res/gl shaders/smooth_circle.frag");
			}
		}
        
        void PixelRenderer2D::Begin()
        {
            auto specs = PIX3D::Engine::GetApplicationSpecs();
            s_OrthographicMatrix = glm::ortho(
                0.0f,                   // Left
                static_cast<float>(specs.Width),  // Right
                static_cast<float>(specs.Height), // Bottom (flipped)
                0.0f,                   // Top
                -1.0f, 1.0f             // Near and far planes
            );
        }
        
        void PixelRenderer2D::End()
        {
        }

        void PixelRenderer2D::DrawSmoothCircle_TopLeft(float x, float y, float size, glm::vec4 color)
        {

            glm::mat4 proj =
                s_OrthographicMatrix *
                glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f)) *
                glm::scale(glm::mat4(1.0f), glm::vec3(size, size, size));

            s_SmoothCircleShader.Bind();
            s_SmoothCircleShader.SetMat4("u_Projection", proj);
            s_SmoothCircleShader.SetVec4("u_color", color);
            
            s_SmoothCircleVertexArray.Bind();
            GLCommands::DrawIndexed(Primitive::TRIANGLES, 6);
        }
	}
}
