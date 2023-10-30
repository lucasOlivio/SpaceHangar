#include "components/Transform.h"
#include "common/utils.h"
#include "common/utilsMat.h"
#include "common/opengl.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

void TransformComponent::SetOrientation(glm::vec3 value)
{
    this->m_qOrientation = glm::quat(glm::radians(value));
}

void TransformComponent::AdjustOrientation(glm::vec3 value)
{
	// To combine quaternion values, you multiply them together
	// Make a quaternion that represents that CHANGE in angle
	glm::quat qChange = glm::quat(value);

	// Multiply them together to get the change
	// Just like with matrix math
	this->m_qOrientation *= qChange;
}

void TransformComponent::Move(glm::vec3 deltaValue)
{
	this->m_oldPosition = this->m_position;
	this->m_position.x += deltaValue.x;
	this->m_position.y += deltaValue.y;
	this->m_position.z += deltaValue.z;
}

void TransformComponent::SetFramePosition()
{
	this->m_framePosition = this->m_position;
	return;
}

void TransformComponent::MoveTo(glm::vec3 value)
{
	this->m_oldPosition = this->m_position;
	this->m_position = value;
}

void TransformComponent::SetPosition(glm::vec3 value)
{
	this->m_oldPosition = this->m_position;
	this->m_position = value;
	this->m_initialPosition = value;
	return;
}

void TransformComponent::ResetPosition()
{
	this->m_position = this->m_initialPosition;
}

void TransformComponent::ResetFramePosition()
{
	this->m_position = this->m_framePosition;
}

void TransformComponent::SetOldPosition()
{
	glm::vec3 tempPosDelta = this->m_position - this->m_oldPosition;
	this->m_position = this->m_oldPosition;
	this->m_oldPosition = this->m_oldPosition - tempPosDelta;
	return;
}

glm::vec3 TransformComponent::GetPosition()
{
	return this->m_position;
}

glm::vec3 TransformComponent::GetOldPosition()
{
	return this->m_oldPosition;
}

glm::quat TransformComponent::GetQuatOrientation()
{
    return this->m_qOrientation;
}

glm::vec3 TransformComponent::GetOrientation()
{
	return glm::degrees(glm::eulerAngles(this->GetQuatOrientation()));
}

void TransformComponent::SetScale(float value)
{
	this->m_scale = value;
}

float TransformComponent::GetScale()
{
	return this->m_scale;
}

void TransformComponent::GetInfo(sComponentInfo& compInfoOut)
{
	using namespace myutils;

	compInfoOut.componentName = "transform";
	compInfoOut.componentParameters.clear();

	this->AddCompParInfo("position", "vec3", this->m_initialPosition, compInfoOut);
	this->AddCompParInfo("scale", "float", this->GetScale(), compInfoOut);
	this->AddCompParInfo("orientation", "vec3", this->GetOrientation(), compInfoOut);
}

void TransformComponent::SetParameter(sParameterInfo& parameterIn)
{
	using namespace myutils;

	if (parameterIn.parameterName == "position") {
		this->SetPosition(parameterIn.parameterVec3Value);
	}
	else if (parameterIn.parameterName == "orientation") {
		this->SetOrientation(parameterIn.parameterVec3Value);
	}
	else if (parameterIn.parameterName == "scale") {
		this->SetScale(parameterIn.parameterFloatValue);
	}

	return;
}

void TransformComponent::Update(uint shaderID, iShaderInfo* pShaderInfo)
{
	// Update model matrix on shader
	glm::mat4 matModel = glm::mat4(1.0);
	myutils::ApplyTransformInModelMat(this, matModel);
	int matModelUL = pShaderInfo->GetUL(shaderID, "matModel");
	glUniformMatrix4fv(matModelUL, 1, GL_FALSE, glm::value_ptr(matModel));

	// Also calculate and pass the "inverse transpose" for the model matrix
	glm::mat4 matModelIT = glm::inverse(glm::transpose(matModel));
	int matModelITUL = pShaderInfo->GetUL(shaderID, "matModel_IT");
	glUniformMatrix4fv(matModelITUL, 1, GL_FALSE, glm::value_ptr(matModelIT));

	return;
}