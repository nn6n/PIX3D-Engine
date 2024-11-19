#include "Application.h"
#include <Platfrom/WindowsPlatform.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Core/Core.h>
#include <Platfrom/GL/GLCommands.h>
#include <Platfrom/GL/GLGraphicsContext.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PIX3D
{
	Application::Application(const ApplicationSpecs& specs)
		: m_AppSpecs(specs)
	{
		m_Instance = this;

#if defined(_WIN32) || defined(_WIN64)
		
		m_Platform = std::make_unique<WindowsPlatformLayer>();
		m_Platform->CreatWindow(specs.Width, specs.Height, specs.Title.c_str());
#else
#error This Engine Is Currently Supports Windows Platform Only!
#endif

		switch (specs.API)
		{
		case OPENGL:
		    {
			   m_GraphicsContext = std::make_unique<GL::GLGraphicsContext>();
			   m_GraphicsContext->Init(m_Platform->GetNativeWindowHandel());
		    }break;
		case VULKAN:
		    {
			   PIX_ASSERT_MSG(false, "Vulkan Not Supported Yet!");
		    }break;
		case NONE:
		    {
			   PIX_ASSERT_MSG(false, "Failed to specify garphics api");
		    }
		}

		// Camera Setup
		m_Camera = Camera3D(glm::vec3(0.0f, 0.0f, 5.0f));

		std::vector<glm::vec3> vertices =
		{
	        { 0.5f,  0.5f, 0.0f },  // top right
	        { 0.5f, -0.5f, 0.0f },  // bottom right
	        {-0.5f, -0.5f, 0.0f },  // bottom left
	        {-0.5f,  0.5f, 0.0f }  // top left 
		};
		std::vector<uint32_t> indices = 
		{
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};
		
		// VBO
		GL::GLVertexBuffer QuadVertexBuffer;
		QuadVertexBuffer.Create();
		QuadVertexBuffer.Fill(BufferData::CreatFrom(vertices), GL::GLVertexBufferUsage::STATIC_DRAW);

		// EBO
		GL::GLIndexBuffer QuadIndexBuffer;
		QuadIndexBuffer.Create();
		QuadIndexBuffer.Fill(indices);

		// VAO
		QuadVertexArray.Create();
		QuadVertexArray.AddVertexBuffer(QuadVertexBuffer, sizeof(glm::vec3));
		QuadVertexArray.AddIndexBuffer(QuadIndexBuffer);

		{
			GL::GLVertexAttrib PositionAttrib;
			PositionAttrib.ShaderLocation = 0;
			PositionAttrib.Count = 3;
			PositionAttrib.Type = GL::GLShaderAttribType::FLOAT;
			PositionAttrib.Offset = 0;

			QuadVertexArray.AddVertexAttrib(PositionAttrib);
		}

		Shader.LoadFromFile("res/gl shaders/pos_vertex.vert", "res/gl shaders/pos_fragment.frag");

		CameraUnifromBuffer.Create(0);
	}

	void Application::Run()
	{
		while (IsRunning())
		{
			{// Calculate DeltaTime
				double CurrentFrame = glfwGetTime();
				m_DeltaTime = CurrentFrame - m_LastTime;
				m_LastTime = CurrentFrame;
			}

			// Poll Events
			m_Platform->PollEvents();

			m_Camera.Update(m_DeltaTime);

			// Graphics
			GL::GLCommands::Clear(0.5f, 0.3f, 0.1f, 1.0f);
			GL::GLCommands::ClearFlag(GL::ClearFlags::COLOR);

			Shader.Bind();
			{

				glm::mat4 proj = m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix();

				BufferData CameraBuffer = { &proj, sizeof(glm::mat4) };
				CameraUnifromBuffer.Update(CameraBuffer);

				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))
					* glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

				Shader.SetMat4("model", model);
			}

			QuadVertexArray.Bind();
			GL::GLCommands::DrawIndexed(GL::Primitive::LINE_LOOP, 6);

			m_GraphicsContext->SwapBuffers(m_Platform->GetNativeWindowHandel());

			Input::ResetInput();
		}
	}

	bool Application::IsRunning()
	{
		return !glfwWindowShouldClose((GLFWwindow*)m_Platform->GetNativeWindowHandel());
	}
}
