#pragma once

#include "common/types.h"
#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

// Parsed info to be used decoupled
struct sParameterInfo
{
	std::string parameterName;
	std::string parameterType;
	// TODO: Is there a better way to do this?
	std::string parameterStrValue;
	int parameterIntValue;
	bool parameterBoolValue;
	float parameterFloatValue;
	glm::vec3 parameterVec3Value;
	glm::vec4 parameterVec4Value;
	std::vector<std::string> parameterVecStrValue;
};

struct sComponentInfo
{
	std::string componentName;
	std::vector<sParameterInfo> componentParameters;
};

class iGameplayDirector;

class iComponent
{
public:
	virtual ~iComponent() {};

	virtual EntityID GetEntityID() = 0;
	virtual void SetEntityID(EntityID entityID) = 0;
	virtual void SetGameplayDirector(iGameplayDirector* pGameplayDirector) = 0;

	virtual void GetInfo(sComponentInfo& compInfoOut) = 0;
	virtual void SetParameter(sParameterInfo& parameterIn) = 0;
};
