#include "SnakeGame.h"
#include "GameStates/IntroState.h"

void SnakeGame::OnStart()
{
	// disable depth test for 2D
	Engine::GetGraphicsContext()->EnableDepthTest(false);

	StateManager.init(new IntroState());
	StateManager.OnStart();
}

void SnakeGame::OnUpdate(float dt)
{
	StateManager.OnUpdate(dt);
}

void SnakeGame::OnResize(uint32_t width, uint32_t height)
{
}
