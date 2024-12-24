#pragma once

#include "GameState.h"
#include "../Snake.hpp"

class GameplayState : public GameState
{
public:
	virtual void OnStart() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnDestroy() override;
private:
	Snake Snake;
	Food Food;

	GL::GLTexture Grass;

	float Time = 0.0f;
	float Tiling = 18.5f;

	struct _Camera
	{
		Camera2D Cam2D;

		float ShakeDuration;
		float ShakeMagnitude;
		bool ShakeOnce = false;

		void StartShaking(float duration, float magnitude);
		bool IsShakingFinished();
	}Camera;
};
