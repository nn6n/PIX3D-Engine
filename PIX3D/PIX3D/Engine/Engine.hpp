#pragma once
#include <Core/Input.h>
#include <Core/Application.h>
#include <Platfrom/Platform.h>
#include <Platfrom/WindowsPlatform.h>
#include <Platfrom/GL/GLGraphicsContext.h>
#include <Core/Core.h>
#include <Core/Input.h>
#include <Graphics/GraphicsContext.h>
#include <memory>
#include <functional>
#include <GLFW/glfw3.h>

namespace PIX3D
{
	using WindowSizeCallBackFuncPtrType = std::function<void(int, int)>;
	using KeyboardCallBackFuncPtrType = std::function<void(KeyCode)>;

	enum class GraphicsAPI
	{
		NONE,
		OPENGL,
		VULKAN
	};

	struct EngineSpecs
	{
		GraphicsAPI API = GraphicsAPI::OPENGL;
	};

	class Engine
	{
	public:

		static void Init(const EngineSpecs& specs)
		{
			s_EngineSpecs = specs;
		}

		template <typename T>
		static void CreateApplication(const ApplicationSpecs& specs)
		{
			// Create Application
			s_Application = new T();
			s_AppSpecs = specs;

			// Platform Layer

            #if defined(_WIN32) || defined(_WIN64)
            
            			s_Platform = new WindowsPlatformLayer();
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

			s_Application->OnStart();

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

				s_Application->OnUpdate();

				s_GraphicsContext->SwapBuffers(s_Platform->GetNativeWindowHandel());

				Input::ResetInput();
			}
		}
		static void Destroy()
		{
			delete s_Platform;
			delete s_Application;
			delete s_GraphicsContext;
		}

	private:
		static bool IsRunning()
		{
			return !glfwWindowShouldClose((GLFWwindow*)s_Platform->GetNativeWindowHandel());
		}

	public:
		inline static PlatformLayer* GetPlatformLayer() { return s_Platform; }
		inline static Application* GetApplication() { return s_Application; }

		// CallBacks
		
		inline static void SetWindowSizeCallBackFunc(const WindowSizeCallBackFuncPtrType& func) { s_WindowSizeCallBackFuncPtr = func; }
		inline static void SetKeyboardCallBackFuncPtr(const KeyboardCallBackFuncPtrType& func) { s_KeyboardCallBackFuncPtr = func; }
		
		inline static WindowSizeCallBackFuncPtrType GetWindowSizeCallBackFunc() { return s_WindowSizeCallBackFuncPtr; }
		inline static KeyboardCallBackFuncPtrType GetKeyboardCallBackFuncPtr() { return s_KeyboardCallBackFuncPtr; }


		inline static ApplicationSpecs GetApplicationSpecs() { return s_AppSpecs; }

	private:
		// Engine Specs
		inline static EngineSpecs s_EngineSpecs;

		inline static PlatformLayer* s_Platform = nullptr;
		inline static GraphicsContext* s_GraphicsContext = nullptr;

		// Application
		inline static Application* s_Application = nullptr;
		inline static ApplicationSpecs s_AppSpecs;
		
		// Timer
		inline static float s_DeltaTime = 0.0f;
		inline static float s_LastTime = 0.0f;

		// CallBacks
		inline static WindowSizeCallBackFuncPtrType s_WindowSizeCallBackFuncPtr = nullptr;
		inline static KeyboardCallBackFuncPtrType s_KeyboardCallBackFuncPtr = nullptr;
	};
}