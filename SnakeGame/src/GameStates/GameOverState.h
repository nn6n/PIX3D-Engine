#pragma once

#include "GameState.h"

using namespace PIX3D;

class GameOverState : public GameState
{
public:
	virtual void OnStart() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnDestroy() override;
private:
	Camera2D Cam2D;
	GL::GLTexture GameOver;
	glm::vec2 GameOverTextureSize;

	GL::GLTexture Grass;
	float Tiling = 18.5f;
};
