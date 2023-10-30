#pragma once

#include "Component.h"

class ForceComponent : public Component
{
private:
	float m_inverseMass;
	glm::vec3 m_acceleration;
	glm::vec3 m_velocity;
public:
	// Setters
	void SetInverseMass(float value);
	void SetAcceleration(glm::vec3 value);
	void SetVelocity(glm::vec3 value);

	// Getters
	float GetInverseMass();
	glm::vec3 GetAcceleration();
	glm::vec3 GetVelocity();

	virtual void GetInfo(sComponentInfo& compInfoOut);
	virtual void SetParameter(sParameterInfo& parameterIn);
};

