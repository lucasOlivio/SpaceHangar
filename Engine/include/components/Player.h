#pragma once

#include "Component.h"
#include "events/KeyEvent.h"
#include "events/iListener.h"
#include <glm/vec3.hpp>

class PlayerComponent : public Component,
						public iListener
{
private:
	// Velocity for each axis the entity can walk
	glm::vec3 m_velocity;
	// Current orientation the entity should be facing
	glm::vec3 m_currentOrientation;

	bool m_isPlaying;
public:
	glm::vec3 GetVelocity();
	glm::vec3 GetCurrentOrientation();

	virtual void GetInfo(sComponentInfo& compInfoOut);
	virtual void SetParameter(sParameterInfo& parameterIn);

	// On key callback we trigger the proper player action
	virtual void Notify(iEvent* pEvent);

	void SetPlaying(bool isPlaying);

	void PlayerActions(sKeyInfo keyInfo);
};