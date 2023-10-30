#include "components/Light.h"
#include "common/utils.h"
#include <string>

// ctors dtors
LightComponent::LightComponent()
{
	this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);     // Center of everything
	this->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);		// White color
	this->specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);     // White LightComponent
	this->atten = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);		// No attenuation
	this->direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);	// Defaults to straight down
	this->params = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);	    // LightComponent_TYPE POINT
	this->status = false;	                                // Off

	this->m_position_UL = -1;
	this->m_diffuse_UL = -1;
	this->m_specular_UL = -1;
	this->m_atten_UL = -1;
	this->m_direction_UL = -1;
	this->m_params_UL = -1;
	this->m_status_UL = -1;
}

LightComponent::~LightComponent()
{
}

// Setup the UL locations and all LightComponent configurations
void LightComponent::SetupLight(GLint shaderProgramID, std::string ulBasePath)
{
	// Set dot for base path, the LightComponent must be a structure inside the shader so we can set all the parameters
	if (ulBasePath.back() != '.')
	{
		ulBasePath += ".";
	}
	this->m_ulBasePath = ulBasePath;
	this->m_position_UL = glGetUniformLocation(shaderProgramID, (this->m_ulBasePath + "position").c_str());
	this->m_diffuse_UL = glGetUniformLocation(shaderProgramID, (this->m_ulBasePath + "diffuse").c_str());
	this->m_specular_UL = glGetUniformLocation(shaderProgramID, (this->m_ulBasePath + "specular").c_str());
	this->m_atten_UL = glGetUniformLocation(shaderProgramID, (this->m_ulBasePath + "atten").c_str());
	this->m_direction_UL = glGetUniformLocation(shaderProgramID, (this->m_ulBasePath + "direction").c_str());
	this->m_params_UL = glGetUniformLocation(shaderProgramID, (this->m_ulBasePath + "params").c_str());
	this->m_status_UL = glGetUniformLocation(shaderProgramID, (this->m_ulBasePath + "status").c_str());

	glUniform4f(this->m_position_UL, this->position.x,
		this->position.y, this->position.z,
		this->position.w);
	glUniform4f(this->m_diffuse_UL, this->diffuse.r,
		this->diffuse.g, this->diffuse.b,
		this->diffuse.a);
	glUniform4f(this->m_specular_UL, this->specular.r,
		this->specular.g, this->specular.b,
		this->specular.a);
	glUniform4f(this->m_atten_UL, this->atten.x,
		this->atten.y, this->atten.z,
		this->atten.w);
	glUniform4f(this->m_direction_UL, this->direction.x,
		this->direction.y, this->direction.z,
		this->direction.w);
	glUniform4f(this->m_direction_UL, this->direction.x,
		this->direction.y, this->direction.z,
		this->direction.w);
	glUniform4f(this->m_params_UL, this->params.x,
		this->params.y, this->params.z,
		this->params.w);
	glUniform4f(this->m_status_UL, this->status, 0, 0, 0);
}

// Getters
glm::vec4 LightComponent::GetPosition()
{
	return this->position;
}

glm::vec4 LightComponent::GetDiffuse()
{
	return this->diffuse;
}

glm::vec4 LightComponent::GetSpecular()
{
	return this->specular;
}

glm::vec4 LightComponent::GetAtten()
{
	return this->atten;
}

glm::vec4 LightComponent::GetDirection()
{
	return this->direction;
}

glm::vec4 LightComponent::GetParams()
{
	return this->params;
}

bool LightComponent::GetStatus()
{
	return this->status;
}

// Setters
void LightComponent::SetPosition(glm::vec4 newPosition)
{
	this->position = newPosition;

	if (this->m_position_UL == -1)
	{
		return;
	}

	glUniform4f(this->m_position_UL, this->position.x,
		this->position.y, this->position.z,
		this->position.w);
}

void LightComponent::SetDiffuse(glm::vec4 newDiffuse)
{
	this->diffuse = newDiffuse;

	if (this->m_diffuse_UL == -1)
	{
		return;
	}

	glUniform4f(this->m_diffuse_UL, this->diffuse.r,
		this->diffuse.g, this->diffuse.b,
		this->diffuse.a);
}

void LightComponent::SetSpecular(glm::vec4 newSpecular)
{
	this->specular = newSpecular;

	if (this->m_specular_UL == -1)
	{
		return;
	}

	glUniform4f(this->m_specular_UL, this->specular.r,
		this->specular.g, this->specular.b,
		this->specular.a);
}

void LightComponent::SetAtten(glm::vec4 newAtten)
{
	this->atten = newAtten;

	if (this->m_atten_UL == -1)
	{
		return;
	}

	glUniform4f(this->m_atten_UL, this->atten.x,
		this->atten.y, this->atten.z,
		this->atten.w);
}

void LightComponent::SetDirection(glm::vec4 newDirection)
{
	this->direction = newDirection;

	if (this->m_direction_UL == -1)
	{
		return;
	}

	glUniform4f(this->m_direction_UL, this->direction.x,
		this->direction.y, this->direction.z,
		this->direction.w);
}

void LightComponent::SetParams(glm::vec4 newParams)
{
	this->params = newParams;

	if (this->m_params_UL == -1)
	{
		return;
	}

	glUniform4f(this->m_params_UL, this->params.x,
		this->params.y, this->params.z,
		this->params.w);
}

void LightComponent::SetStatus(bool newStatus)
{
	this->status = newStatus;

	if (this->m_status_UL == -1)
	{
		return;
	}

	glUniform4f(this->m_status_UL, this->status, 0, 0, 0);
}

void LightComponent::SetLinearAtten(float value)
{
	this->atten[1] = value;
	this->SetAtten(this->atten);
}

void LightComponent::ResetAtten()
{
	this->SetAtten(this->initialAtten);
}

void LightComponent::GetInfo(sComponentInfo& compInfoOut)
{
	using namespace myutils;

	compInfoOut.componentName = "light";
	compInfoOut.componentParameters.clear();

	this->AddCompParInfo("position", "vec4", this->position, compInfoOut);
	this->AddCompParInfo("diffuse", "vec4", this->diffuse, compInfoOut);
	this->AddCompParInfo("specular", "vec4", this->specular, compInfoOut);
	this->AddCompParInfo("atten", "vec4", this->initialAtten, compInfoOut);
	this->AddCompParInfo("direction", "vec4", this->direction, compInfoOut);
	this->AddCompParInfo("params", "vec4", this->params, compInfoOut);
	this->AddCompParInfo("status", "bool", this->status, compInfoOut);
}

void LightComponent::SetParameter(sParameterInfo& parameterIn)
{
	using namespace myutils;

	if (parameterIn.parameterName == "position") {
		this->SetPosition(parameterIn.parameterVec4Value);
	}
	else if (parameterIn.parameterName == "diffuse") {
		this->SetDiffuse(parameterIn.parameterVec4Value);
	}
	else if (parameterIn.parameterName == "specular") {
		this->SetSpecular(parameterIn.parameterVec4Value);
	}
	else if (parameterIn.parameterName == "atten") {
		this->SetAtten(parameterIn.parameterVec4Value);
		this->initialAtten = parameterIn.parameterVec4Value;
	}
	else if (parameterIn.parameterName == "direction") {
		this->SetDirection(parameterIn.parameterVec4Value);
	}
	else if (parameterIn.parameterName == "params") {
		this->SetParams(parameterIn.parameterVec4Value);
	}
	else if (parameterIn.parameterName == "status") {
		this->SetStatus(parameterIn.parameterBoolValue);
	}

	return;
}

