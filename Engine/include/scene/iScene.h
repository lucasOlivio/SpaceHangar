#pragma once

#include "components/iComponent.h"
#include "common/types.h"
#include <string>
#include <map>
#include <vector>

class iScene
{
public:
	iScene() {};
	virtual ~iScene() {};

	virtual void Clear() = 0;

	virtual EntityID GetNumEntities() = 0;

	virtual EntityID CreateEntity() = 0;
	virtual void DeleteEntity(EntityID entityID) = 0;

	// Get all parameters info from all the components
	// this entity has attached to it
	virtual std::vector<sComponentInfo> GetComponentsInfo(EntityID entityID) = 0;

	// Get the vector for the respective entities->component
	virtual bool GetMapComponents(std::string componentName,
								  std::map<EntityID, iComponent*>& componentsOut) = 0;

	// Get or add a component to/from the scene entity
	virtual iComponent* GetComponent(EntityID entityID, std::string componentName) = 0;
	virtual void SetComponent(EntityID entityID, std::string componentName, iComponent* componentIn) = 0;
};
