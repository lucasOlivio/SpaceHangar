#pragma once

#include <glm/vec3.hpp>
#include "Component.h"

class ProjectileComponent : public Component
{
private:
	glm::vec3 m_velocity;
	iComponent* m_pHit;

public:

	virtual void GetInfo(sComponentInfo& compInfoOut);
	virtual void SetParameter(sParameterInfo& parameterIn);
};