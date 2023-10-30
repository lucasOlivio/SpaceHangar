#pragma once

#include "iComponent.h"
#include "scene/iScene.h"
#include <string>

class ComponentBuilder
{
private:
	iScene* m_pScene;
public:
	ComponentBuilder(iScene* pScene);
	~ComponentBuilder() {};

	iComponent* BuildComponent(sComponentInfo& componentInfo, EntityID entityID);
};