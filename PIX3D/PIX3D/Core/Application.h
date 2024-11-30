#pragma once
#include <string>

namespace PIX3D
{
	struct ApplicationSpecs
	{
		uint32_t Width = 800;
		uint32_t Height = 600;
		std::string Title = "PIX3D Application";
	};

	class Application
	{
	public:
		Application(const ApplicationSpecs& specs)
			: m_AppSpecs(specs)
		{}

		virtual ~Application() {}
		
		virtual void OnStart() {}
		virtual void OnUpdate() {}

		void SetWidth(uint32_t width) { m_AppSpecs.Width = width; }
		void SetHeight(uint32_t height) { m_AppSpecs.Height = height; }

		ApplicationSpecs GetApplicationSpecs() const { return m_AppSpecs; }
	private:
		ApplicationSpecs m_AppSpecs;
	};
}
