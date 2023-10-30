#include "scene/SceneView.h"

SceneView::SceneView(iScene* pScene)
	: m_pScene(pScene)
{
	this->m_map.clear();
	this->m_currIterator = this->m_map.begin();
}

SceneView::~SceneView()
{
}

void SceneView::First(std::string componentName)
{
	bool hasComponent = this->m_pScene->GetMapComponents(componentName, this->m_map);
	if (hasComponent)
	{
		this->m_currIterator = this->m_map.begin();
	}
	else
	{
		this->m_currIterator = this->m_map.end();
	}
}

void SceneView::Next()
{
	this->m_currIterator++;
}

void SceneView::End()
{
	this->m_currIterator = this->m_map.end();
}

bool SceneView::IsDone()
{
	if (this->m_currIterator == this->m_map.end())
	{
		return true;
	}
	return false;
}

std::vector<sComponentInfo> SceneView::GetComponentsInfo(EntityID entityID)
{
	return this->m_pScene->GetComponentsInfo(entityID);
}

EntityID SceneView::GetNumEntities()
{
	return this->m_pScene->GetNumEntities();
}

EntityID SceneView::CurrentKey()
{
	return this->m_currIterator->first;
}

iComponent* SceneView::GetComponent(EntityID entityID, std::string componentName)
{
	return this->m_pScene->GetComponent(entityID, componentName);
}