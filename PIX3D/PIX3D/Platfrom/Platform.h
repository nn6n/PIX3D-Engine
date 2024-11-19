#pragma once
#include <stdint.h>
#include <string>

namespace PIX3D
{
	class PlatformLayer
	{
	public:
		PlatformLayer() = default;
		~PlatformLayer() {}

		virtual void CreatWindow(uint32_t width, uint32_t height, const char* name) {}
		virtual void PollEvents() {}
		virtual void* GetNativeWindowHandel() { return nullptr; }

		virtual std::pair<uint32_t, uint32_t> GetWindowSize() { return { 0.0f, 0.0f }; }

		virtual bool IsKeyPressed(int keycode) { return false; }
		virtual bool IsKeyReleased(int keycode) { return false; }

		virtual bool IsMouseButtonPressed(int mousebutton) { return false; }
		virtual bool IsMouseButtonReleased(int mousebutton) { return false; }
	};
}
