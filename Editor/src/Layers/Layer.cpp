#include "Layer.h"

void LayerManager::init(Layer* starting_layer)
{
	m_Instance = this;
	m_CurrentLayer = starting_layer;
}

void LayerManager::OnStart()
{
	m_CurrentLayer->OnStart();
}

void LayerManager::OnUpdate(float dt)
{
	m_CurrentLayer->OnUpdate(dt);
}

void LayerManager::GoToLayer(Layer* layer)
{
	// destroy old state

	m_CurrentLayer->OnDestroy();
	delete m_CurrentLayer;
	m_CurrentLayer = nullptr;

	// start new state

	m_CurrentLayer = layer;
	m_CurrentLayer->OnStart();
}
