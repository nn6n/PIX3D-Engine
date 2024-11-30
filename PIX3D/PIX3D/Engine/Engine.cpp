#include "Engine.h"
#include <Platfrom/WindowsPlatform.h>
#include <Platfrom/GL/GLGraphicsContext.h>
#include <GLFW/glfw3.h>
#include <Core/Input.h>

namespace PIX3D
{
	void Engine::Init(const EngineSpecs& specs)
	{
		s_EngineSpecs = specs;
	}


    void Engine::RunApplication(Application& app)
    {
		s_Application = &app;

		// Platform Layer

        #if defined(_WIN32) || defined(_WIN64)
        
        		s_Platform = new WindowsPlatformLayer();
				auto specs = app.GetApplicationSpecs();
				s_Platform->CreatWindow(specs.Width, specs.Height, specs.Title.c_str());
        #else
        #error This Engine Is Currently Supports Windows Platform Only!
        #endif

        // Graphics API
		switch (s_EngineSpecs.API)
		{
		case GraphicsAPI::OPENGL:
		    {
		    	s_GraphicsContext = new GL::GLGraphicsContext();
		    	s_GraphicsContext->Init(s_Platform->GetNativeWindowHandel());
		    }break;

		case GraphicsAPI::VULKAN:
		    {
		    	PIX_ASSERT_MSG(false, "Vulkan Not Supported Yet!");
		    }break;
		    
		case GraphicsAPI::NONE:
		    {
		    	PIX_ASSERT_MSG(false, "Failed to specify garphics api");
		    }
		}

		// Start

		app.OnStart();

		// Game Loop

		while (IsRunning())
		{
			{// Calculate DeltaTime
				double CurrentFrame = glfwGetTime();
				s_DeltaTime = CurrentFrame - s_LastTime;
				s_LastTime = CurrentFrame;
			}

			// Poll Events
			s_Platform->PollEvents();

			app.OnUpdate();

			s_GraphicsContext->SwapBuffers(s_Platform->GetNativeWindowHandel());

			Input::ResetInput();
		}
    }

    void Engine::Destroy()
    {
        delete s_Platform;
		delete s_Application;
		delete s_GraphicsContext;
    }

	bool Engine::IsRunning()
	{
		return !glfwWindowShouldClose((GLFWwindow*)s_Platform->GetNativeWindowHandel());
	}
}
