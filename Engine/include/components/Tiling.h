#pragma once

#include "Component.h"
#include <glm/vec3.hpp>

class TilingComponent : public Component
{
private:
	glm::vec3 m_axis;
	glm::vec3 m_offset;
public:

	glm::vec3 GetAxis();
	glm::vec3 GetOffset();

	virtual void GetInfo(sComponentInfo& compInfoOut);
	virtual void SetParameter(sParameterInfo& parameterIn);
};
