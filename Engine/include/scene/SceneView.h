#pragma once

#include "iScene.h"

// TODO: Way to make a generic map iterator interface
// Viewer mediator for scene and its components
class SceneView
{
private:
	iScene* m_pScene;

	std::map<EntityID, iComponent*> m_map;
	std::map<EntityID, iComponent*>::iterator m_currIterator;
public:
	SceneView(iScene* pScene);
	~SceneView();

	// TODO: Way to get the component based on the class name, not by a string
	void First(std::string componentName);
	void Next();
	void End();
	bool IsDone();

	// Return components in a structured and common way parsed
	std::vector<sComponentInfo> GetComponentsInfo(EntityID entityID);
	EntityID GetNumEntities();

	EntityID CurrentKey();
	// Get the component already casted to its class
	template<typename Component>
	Component* CurrentValue();
	// Get Generic iComponent interface
	iComponent* GetComponent(EntityID entityID, std::string componentName);
	// Get a specific component from an entity on the scene
	template<typename Component>
	Component* GetComponent(EntityID entityID, std::string componentName);
};

#include "SceneView.tpp"