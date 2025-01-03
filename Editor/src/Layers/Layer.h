#pragma once

#include <PIX3D.h>

class Layer
{
public:
	virtual void OnStart() = 0;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnDestroy() = 0;
private:
};


class LayerManager
{
public:
	void init(Layer* starting_layer);

	void OnStart();
	void OnUpdate(float dt);

	void GoToLayer(Layer* layer);

	inline static LayerManager& Get() { return *m_Instance; }

private:
	inline static LayerManager* m_Instance = nullptr;

	Layer* m_CurrentLayer = nullptr;
};
