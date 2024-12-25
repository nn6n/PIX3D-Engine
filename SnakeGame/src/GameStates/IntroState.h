#pragma once

#include "GameState.h"

using namespace PIX3D;

class IntroState : public GameState
{
public:
	virtual void OnStart() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnDestroy() override;
private:
	Camera2D Cam2D;
	GL::GLTexture Title;
};
