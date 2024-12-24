#include "GameState.h"

void GameStateMangaer::init(GameState* starting_state)
{
	m_Instance = this;
	m_CurrentGameState = starting_state;
}

void GameStateMangaer::OnStart()
{
	m_CurrentGameState->OnStart();
}

void GameStateMangaer::OnUpdate(float dt)
{
	m_CurrentGameState->OnUpdate(dt);
}

void GameStateMangaer::GoToState(GameState* state)
{
	// destroy old state

	m_CurrentGameState->OnDestroy();
	delete m_CurrentGameState;
	m_CurrentGameState = nullptr;

	// start new state

	m_CurrentGameState = state;
	m_CurrentGameState->OnStart();
}
