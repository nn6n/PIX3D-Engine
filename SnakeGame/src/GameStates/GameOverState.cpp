#include "GameOverState.h"
#include "GameplayState.h"

#define GREEN {0.1f, 0.5f, 0.1f, 1.0f}

void GameOverState::OnStart()
{
	Cam2D.Init();
	GameOver.LoadFromFile("assets/gameover.png");
	Grass.LoadFromFile("assets/grass.png");
}

void GameOverState::OnUpdate(float dt)
{
	// clear background
	{
		GL::GLCommands::ClearFlag(GL::ClearFlags::COLOR);
		GL::GLCommands::Clear(0.1f, 0.1f, 0.1f, 1.0f);
	}

	// rendering
	{
		GL::GLPixelRenderer2D::Begin(Cam2D);

		auto AppSpecs = Engine::GetApplicationSpecs();
		float width = (float)AppSpecs.Width;
		float height = (float)AppSpecs.Height;

		// render

		GL::GLPixelRenderer2D::DrawTexturedQuad(Grass, width * 0.5f, height * 0.5f, width, height, GREEN, Tiling);

		GameOverTextureSize = glm::mix(GameOverTextureSize, { 500.0f, 500.0f }, 5.0f * dt);

		GL::GLPixelRenderer2D::DrawTexturedQuad(GameOver, width * 0.5f, height * 0.5f, GameOverTextureSize.x, GameOverTextureSize.y, {1.0f, 1.0f, 1.0f, 1.0f});

		// end rendering
		GL::GLPixelRenderer2D::End();
	}

	// restart
	{
		if (Input::IsKeyPressed(KeyCode::Space))
			GameStateMangaer::Get().GoToState(new GameplayState());
	}
}

void GameOverState::OnDestroy()
{
	GameOver.Destroy();
	Grass.Destroy();
}
