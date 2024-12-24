
/*
* Copyright (c) 2024 Karim Hamdallah
*/

#include "SnakeGame.h"

int main()
{
    // Init Engine
    {
        PIX3D::EngineSpecs EngineSpecs;
        EngineSpecs.API = PIX3D::GraphicsAPI::OPENGL;

        PIX3D::Engine::Init(EngineSpecs);
    }

    // Run Application
    PIX3D::ApplicationSpecs specs;
    specs.Width = 750;
    specs.Height = 750;
    specs.Title = "SnakeGame";  // Embedded project name here
    
    PIX3D::Engine::CreateApplication<SnakeGame>(specs);

    // Destroy Engine
    PIX3D::Engine::Destroy();

    return 0;
}
