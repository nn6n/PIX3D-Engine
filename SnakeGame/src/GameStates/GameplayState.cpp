#include "GameplayState.h"
#include "GameOverState.h"

#define GREEN {0.0f, 0.5f, 0.0f, 1.0f}
#define WHITE {1.0f, 1.0f, 1.0f, 1.0f}


void GameplayState::OnStart()
{
	Camera.Cam2D.Init();
	
	Snake.LoadAssets();
	Food.LoadAssets();

	Grass.LoadFromFile("assets/grass.png");
}

void GameplayState::OnUpdate(float dt)
{
	// calculate time
	Time += dt;

	// clear background
	{
		GL::GLCommands::ClearFlag(GL::ClearFlags::COLOR);
		GL::GLCommands::Clear(0.1f, 0.1f, 0.1f, 1.0f);
	}

	// update
	{
		if (!Snake.IsDead())
		{
			Snake.ProcessInput();
			Food.Update(Snake);

			if (Time >= 0.1f)
			{
				Snake.Update();
				Time = 0;
			}
		}
		else
		{
			if (!Camera.ShakeOnce)
			{
				Camera.StartShaking(0.2f, 300.0f);
				Camera.ShakeOnce = true;
			}
		}
	}


	// Camera Shaking
	{
		auto CameraPos = Camera.Cam2D.GetPosition();
		float CameraRot = Camera.Cam2D.GetRotaion();

		// shake code herer
		if (Camera.ShakeDuration > 0.0f)
		{
			// Generate random offset for shake
			float offsetX = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f; // Range: [-1.0f, 1.0f]
			float offsetY = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f; // Range: [-1.0f, 1.0f]

			CameraPos.x += offsetX * Camera.ShakeMagnitude * dt;
			CameraPos.y += offsetY * Camera.ShakeMagnitude * dt;

			// Reduce the shake duration over time
			Camera.ShakeDuration -= dt;
		}

		Camera.Cam2D.SetPosition(CameraPos);
		Camera.Cam2D.SetRotaion(CameraRot);
	}

	//rendering
	{
		GL::GLPixelRenderer2D::Begin(Camera.Cam2D);

		auto AppSpecs = Engine::GetApplicationSpecs();
		float width = (float)AppSpecs.Width;
		float height = (float)AppSpecs.Height;


		// render

		GL::GLPixelRenderer2D::DrawTexturedQuad(Grass, width * 0.5f, height * 0.5f, width * 1.5f, height * 1.5f, GREEN, Tiling);

		Snake.Draw();
		Food.Draw();

		// end rendering
		GL::GLPixelRenderer2D::End();
	}

	// Game Over State
	if (Camera.IsShakingFinished())
	{
		GameStateMangaer::Get().GoToState(new GameOverState());
	}
}

void GameplayState::OnDestroy()
{
	Snake.Destroy();
	Food.Destroy();
}

void GameplayState::_Camera::StartShaking(float duration, float magnitude)
{
	ShakeDuration = duration;
	ShakeMagnitude = magnitude;
}

bool GameplayState::_Camera::IsShakingFinished()
{
	return  ShakeOnce && ShakeDuration <= 0.0f;
}
