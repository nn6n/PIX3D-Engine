#include "IntroState.h"
#include "GameplayState.h"

void IntroState::OnStart()
{
	Cam2D.Init();
	Title.LoadFromFile("assets/intro.png");
}

void IntroState::OnUpdate(float dt)
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

		GL::GLPixelRenderer2D::DrawTexturedQuad(Title, width * 0.5f, height * 0.5f, width, height, { 1.0f, 1.0f, 1.0f, 1.0f });

		// end rendering
		GL::GLPixelRenderer2D::End();
	}

	// start
	{
		if (Input::IsKeyPressed(KeyCode::Enter))
		{
			PlaySound(TEXT("assets/StartGame.wav"), NULL, SND_FILENAME | SND_ASYNC);
			GameStateMangaer::Get().GoToState(new GameplayState());
		}
	}
}

void IntroState::OnDestroy()
{
	Title.Destroy();
}
