#pragma once

#include "iComponent.h"
#include "scene/iGameplayDirector.h"

// Mediator for dealing with components parameters in a centralized way
class Component : public iComponent
{
protected:
	// Used to communicate with everything via the component mediators
	EntityID m_entityID;
	
	// Calling actions in-game
	iGameplayDirector* m_pGameplayDirector;

	Component() {};

public:
	virtual ~Component() {};

	// TODO: Now we have one parse per type, is there a better way to do this?
	// Parse to component paramenter info and add to vector of parameters
	// String
	virtual void AddCompParInfo(const std::string& name,
								const std::string& type,
								const std::string& value,
								sComponentInfo& compInfoOut);
	// Integer
	virtual void AddCompParInfo(const std::string& name,
								const std::string& type,
								const int& value,
								sComponentInfo& compInfoOut);
	// Bool
	virtual void AddCompParInfo(const std::string& name,
								const std::string& type,
								const bool& value,
								sComponentInfo& compInfoOut);
	// Float
	virtual void AddCompParInfo(const std::string& name,
								const std::string& type,
								const float& value,
								sComponentInfo& compInfoOut);
	// glm::vec3
	virtual void AddCompParInfo(const std::string& name,
								const std::string& type,
								const glm::vec3& value,
								sComponentInfo& compInfoOut);
	// glm::vec4
	virtual void AddCompParInfo(const std::string& name,
								const std::string& type,
								const glm::vec4& value,
								sComponentInfo& compInfoOut);

	// vector<string>
	virtual void AddCompParInfo(const std::string& name,
								const std::string& type,
								const std::vector<std::string>& value,
								sComponentInfo& compInfoOut);

	virtual EntityID GetEntityID();
	virtual void SetEntityID(EntityID entityID);

	virtual void SetGameplayDirector(iGameplayDirector* pGameplayDirector);

	virtual void GetInfo(sComponentInfo& compInfoOut) = 0;
	virtual void SetParameter(sParameterInfo& parameterIn) = 0;
};
