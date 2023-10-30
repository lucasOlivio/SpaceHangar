#pragma once

#include "common/types.h"
#include "components/iComponent.h"

// Mediator for all components for game actions
class iGameplayDirector
{
public:
	virtual ~iGameplayDirector() {};

	// Component can send a gameplay action passing itself and the entityID
	// to be handled accordingly
	virtual void SendAction(std::string action, EntityID entityID, sParameterInfo& parameterIn) = 0;

	// Check if game is playable
	virtual bool IsPlaying() = 0;
};
