#pragma once
#include <Platfrom/Platform.h>
#include <Core/Input.h>
#include <Graphics/GraphicsContext.h>
#include <functional>
#include <memory>

#include <Graphics/Camera3D.h>
#include <Platfrom/GL/GLVertexArray.h>
#include <Platfrom/GL/GLUniformBuffer.h>
#include <Platfrom/GL/GLShader.h>

namespace PIX3D
{
	enum GraphicsAPI
	{
		NONE,
		OPENGL,
		VULKAN
	};

	using WindowSizeCallBackFuncPtrType = std::function<void(int, int)>;
	using KeyboardCallBackFuncPtrType = std::function<void(KeyCode)>;

	struct ApplicationSpecs
	{
		uint32_t Width = 800;
		uint32_t Height = 600;
		std::string Title = "PIX3D Application";
		GraphicsAPI API = OPENGL;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecs& config);
		void Run();

		static Application& Get() { return *m_Instance; }
		
		ApplicationSpecs GetApplicationSpecs() { return m_AppSpecs; }
		PlatformLayer* GetPlatformLayer() { return m_Platform.get(); }
		
		bool IsRunning();

		void SetWindowSizeCallBackFunc(const WindowSizeCallBackFuncPtrType& func) { m_WindowSizeCallBackFuncPtr = func; }
		void SetKeyboardCallBackFuncPtr(const KeyboardCallBackFuncPtrType& func) { m_KeyboardCallBackFuncPtr = func; }

		WindowSizeCallBackFuncPtrType GetWindowSizeCallBackFunc() { return m_WindowSizeCallBackFuncPtr; }
		KeyboardCallBackFuncPtrType GetKeyboardCallBackFuncPtr() { return m_KeyboardCallBackFuncPtr; }

		void SetWidth(uint32_t w) { m_AppSpecs.Width = w; }
		void SetHeight(uint32_t h) { m_AppSpecs.Height = h; }
	private:
		ApplicationSpecs m_AppSpecs;
		std::unique_ptr<PlatformLayer> m_Platform = nullptr;
		std::unique_ptr<GraphicsContext> m_GraphicsContext = nullptr;

		// CallBacks
		WindowSizeCallBackFuncPtrType m_WindowSizeCallBackFuncPtr = nullptr;
		KeyboardCallBackFuncPtrType m_KeyboardCallBackFuncPtr = nullptr;

		inline static Application* m_Instance = nullptr;

		Camera3D m_Camera;
		GL::GLVertexArray QuadVertexArray;
		GL::GLShader Shader;
		GL::GLUniformBuffer CameraUnifromBuffer;

	private:
		float m_DeltaTime = 0.0f;
		float m_LastTime = 0.0f;
	};
}
