#include "components/Player.h"
#include "common/opengl.h"
#include "scene/iGameplayDirector.h"

glm::vec3 PlayerComponent::GetVelocity()
{
	return this->m_velocity;
}

glm::vec3 PlayerComponent::GetCurrentOrientation()
{
	return this->m_currentOrientation;
}

void PlayerComponent::GetInfo(sComponentInfo& compInfoOut)
{
	compInfoOut.componentName = "player";
	compInfoOut.componentParameters.clear();

	this->AddCompParInfo("velocity", "vec3", this->m_velocity, compInfoOut);
}

void PlayerComponent::SetParameter(sParameterInfo& parameterIn)
{
	if (parameterIn.parameterName == "velocity") {
		this->m_velocity = parameterIn.parameterVec3Value;
	}
}

void PlayerComponent::Notify(iEvent* pEvent)
{
	KeyEvent* pKeyEvent = dynamic_cast<KeyEvent*>(pEvent);
	this->PlayerActions(pKeyEvent->GetKeyInfo());
}

void PlayerComponent::SetPlaying(bool isPlaying)
{
	this->m_isPlaying = isPlaying;
}

void PlayerComponent::PlayerActions(sKeyInfo keyInfo)
{
	if (!this->m_isPlaying)
	{
		return;
	}

	system("cls");
	printf("Editor / Play mode: P\n");

	printf("Open/Close door:  C\n\n");

	// Animation
	//---------------------------------------------
	if (keyInfo.pressedKey == GLFW_KEY_C && (keyInfo.action == GLFW_PRESS))
	{
		sParameterInfo ignore; // TODO: Overload function to send action without parameter
		this->m_pGameplayDirector->SendAction("toggleAnimation", this->m_entityID, ignore);
	}
}
