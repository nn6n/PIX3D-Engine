#pragma once
#include <Core/Input.h>
#include <Core/Application.h>
#include <Platfrom/Platform.h>
#include <Core/Core.h>
#include <Graphics/GraphicsContext.h>
#include <memory>
#include <functional>

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

		static void Init(const EngineSpecs& specs);
		static void RunApplication(Application& app);
		static void Destroy();

	private:
		static bool IsRunning();

	public:
		inline static PlatformLayer* GetPlatformLayer() { return s_Platform; }
		inline static Application* GetApplication() { return s_Application; }

		// CallBacks
		
		inline static void SetWindowSizeCallBackFunc(const WindowSizeCallBackFuncPtrType& func) { s_WindowSizeCallBackFuncPtr = func; }
		inline static void SetKeyboardCallBackFuncPtr(const KeyboardCallBackFuncPtrType& func) { s_KeyboardCallBackFuncPtr = func; }
		
		inline static WindowSizeCallBackFuncPtrType GetWindowSizeCallBackFunc() { return s_WindowSizeCallBackFuncPtr; }
		inline static KeyboardCallBackFuncPtrType GetKeyboardCallBackFuncPtr() { return s_KeyboardCallBackFuncPtr; }


	private:
		// Engine Specs
		inline static EngineSpecs s_EngineSpecs;

		inline static PlatformLayer* s_Platform = nullptr;
		inline static GraphicsContext* s_GraphicsContext = nullptr;

		// Application
		inline static Application* s_Application = nullptr;
		
		// Timer
		inline static float s_DeltaTime = 0.0f;
		inline static float s_LastTime = 0.0f;

		// CallBacks
		inline static WindowSizeCallBackFuncPtrType s_WindowSizeCallBackFuncPtr = nullptr;
		inline static KeyboardCallBackFuncPtrType s_KeyboardCallBackFuncPtr = nullptr;
	};
}
