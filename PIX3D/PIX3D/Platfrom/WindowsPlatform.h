#pragma once
#include "Platform.h"

namespace PIX3D
{
	class WindowsPlatformLayer : public PlatformLayer
	{
	public:
		WindowsPlatformLayer() = default;
		virtual ~WindowsPlatformLayer() {}

		virtual void CreatWindow(uint32_t width, uint32_t height, const char* title) override;
		virtual void PollEvents() override;
		virtual void* GetNativeWindowHandel() override;

		virtual std::pair<uint32_t, uint32_t> GetWindowSize() override;

		virtual bool IsKeyPressed(int keycode) override;
		virtual bool IsKeyReleased(int keycode) override;
		
		virtual bool IsMouseButtonPressed(int mousebutton) override;
		virtual bool IsMouseButtonReleased(int mousebutton) override;

	private:
		void* m_NativeWindowHandel = nullptr;
	};
}
