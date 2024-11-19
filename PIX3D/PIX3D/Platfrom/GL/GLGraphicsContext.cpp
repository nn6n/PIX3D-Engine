#include "GLGraphicsContext.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Core/Core.h>

namespace PIX3D
{
	namespace GL
	{
		void GLGraphicsContext::Init(void* NativeWindowHandle)
		{
			// Make Context
			glfwMakeContextCurrent((GLFWwindow*)NativeWindowHandle);

			// Init Glad
			PIX_ASSERT_MSG(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLAD");

			// Set Viewport
			int w, h;
			glfwGetWindowSize((GLFWwindow*)NativeWindowHandle, &w, &h);
			glViewport(0, 0, w, h);
		}

		void GLGraphicsContext::SwapBuffers(void* NativeWindowHandle)
		{
			glfwSwapBuffers((GLFWwindow*)NativeWindowHandle);
		}
	}
}
