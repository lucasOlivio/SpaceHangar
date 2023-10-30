#pragma once

#include "Component.h"
#include "common/types.h"
#include "EngineRenderer/iShaderInfo.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>

class TransformComponent : public Component
{
private:
	glm::quat m_qOrientation; // Rotation in quaternions
	glm::vec3 m_initialPosition;
	glm::vec3 m_oldPosition;
	glm::vec3 m_framePosition;
	glm::vec3 m_position;
	float m_scale;

public:
	// Convert orientation from degrees angles to quaternions
	void SetOrientation(glm::vec3 value);
	// Change orientation by the given amount
	void AdjustOrientation(glm::vec3 value);

	void Move(glm::vec3 deltaValue);
	// This won't set the initial position, only update the old position
	void MoveTo(glm::vec3 value);
	// This won't change initial nor old position
	void SetFramePosition();
	// This will set the initial position
	void SetPosition(glm::vec3 value);
	// This will reset to the initial position
	void ResetPosition();
	// Resets to the position from the beginning of the frame
	void ResetFramePosition();
	// Recalculate old position by difference with the current
	void SetOldPosition();
	glm::vec3 GetPosition();
	glm::vec3 GetOldPosition();

	glm::quat GetQuatOrientation();
	// Convert orientation from quaternions to degrees angles
	glm::vec3 GetOrientation();

	void SetScale(float value);
	float GetScale();

	void Update(uint shaderID, iShaderInfo* pShaderInfo);

	virtual void GetInfo(sComponentInfo& compInfoOut);
	virtual void SetParameter(sParameterInfo& parameterIn);
};