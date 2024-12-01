/*
* Copyright (c) 2024 Karim Hamdallah
*/

#include <PIX3D.h>


int main()
{
	std::cout << "Hello, World!\n";

	// Init Engine
	{
		PIX3D::EngineSpecs EngineSpecs;
		EngineSpecs.API = PIX3D::GraphicsAPI::OPENGL;

		PIX3D::Engine::Init(EngineSpecs);
	}

	// Create Application
	PIX3D::Application* DrawingApp = nullptr;
	{
		PIX3D::ApplicationSpecs AppSpecs;
		AppSpecs.Width = 800;
		AppSpecs.Height = 600;
		AppSpecs.Title = "Drawing Application";

		DrawingApp = new PIX3D::Application(AppSpecs);
	}

	// Run Application
	PIX3D::Engine::RunApplication(*DrawingApp);

	// Destroy Engine
	PIX3D::Engine::Destroy();

	return 0;
}
