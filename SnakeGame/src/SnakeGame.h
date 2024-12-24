#pragma once
#include "GameStates/GameState.h"

class SnakeGame : public PIX3D::Application
{
public:
	virtual void OnStart() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnResize(uint32_t width, uint32_t height) override;
private:
	GameStateMangaer StateManager;
};
