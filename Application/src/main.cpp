
/*
* Copyright (c) 2024 Karim Hamdallah
*/

#include "DrawingApplication.h"

int main()
{
    // Init Engine
    PIX3D::EngineSpecs EngineSpecs;
    EngineSpecs.API = PIX3D::GraphicsAPI::OPENGL;
    PIX3D::Engine::Init(EngineSpecs);

    // Run Application
    PIX3D::ApplicationSpecs specs;
    specs.Width = 800;
    specs.Height = 600;
    specs.Title = "DrawingApplication";
    
    PIX3D::Engine::CreateApplication<DrawinApplication>(specs);

    // Destroy Engine
    PIX3D::Engine::Destroy();

    return 0;
}
