#pragma once

#include <PIX3D.h>

class GameState
{
public:
	virtual void OnStart() = 0;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnDestroy() = 0;
private:
};


class GameStateMangaer
{
public:
	void init(GameState* starting_state);

	void OnStart();
	void OnUpdate(float dt);
	
	void GoToState(GameState* state);

	inline static GameStateMangaer& Get() { return *m_Instance; }

private:
	inline static GameStateMangaer* m_Instance = nullptr;

	GameState* m_CurrentGameState = nullptr;
};
