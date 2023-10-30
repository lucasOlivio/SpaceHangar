#pragma once

#include <glm/vec3.hpp>
#include "Component.h"

class CameraComponent : public Component
{
public:
	glm::vec3 cameraEye;
	glm::vec3 cameraTarget;
	glm::vec3 upVector;
	bool isActive; // TODO: Only one camera active

	virtual void GetInfo(sComponentInfo& compInfoOut);
	virtual void SetParameter(sParameterInfo& parameterIn);
};