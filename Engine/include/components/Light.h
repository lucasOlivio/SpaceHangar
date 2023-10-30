#pragma once

#include "Component.h"
#include "common/opengl.h"
#include <glm/vec4.hpp>

class LightComponent : public Component
{
private:
	std::string m_ulBasePath;

	// Parameters uniform locations
	GLint m_position_UL;
	GLint m_diffuse_UL;
	GLint m_specular_UL;
	GLint m_atten_UL;
	GLint m_direction_UL;
	GLint m_params_UL;
	GLint m_status_UL;
public:
	enum LIGHT_TYPE {
		POINT,
		SPOT,
		DIRECTIONAL
	};

	// ctors dtors
	LightComponent();
	virtual ~LightComponent();

	// Getters
	glm::vec4 GetPosition();
	glm::vec4 GetDiffuse();
	glm::vec4 GetSpecular();
	glm::vec4 GetAtten();
	glm::vec4 GetDirection();
	glm::vec4 GetParams();
	bool GetStatus();

	// Setters
	void SetPosition(glm::vec4 newPosition);
	void SetDiffuse(glm::vec4 newDiffuse);
	void SetSpecular(glm::vec4 newSpecular);
	void SetAtten(glm::vec4 newAtten);
	void SetDirection(glm::vec4 newDirection);
	void SetParams(glm::vec4 newParams);
	void SetStatus(bool newStatus);

	void SetLinearAtten(float value);
	void ResetAtten();

	virtual void GetInfo(sComponentInfo& compInfoOut);
	virtual void SetParameter(sParameterInfo& parameterIn);

	// Setup the UL locations and all light configurations
	void SetupLight(GLint shaderProgramID, std::string ulBasePath);

	// Light parameters
	glm::vec4 position;
	glm::vec4 diffuse;   // Colour of the light (used for diffuse)
	glm::vec4 specular;  // rgb = highlight colour, w = power
	glm::vec4 atten;	 // x = constant, y = linear, z = quadratic, w = DistanceCutOff
	glm::vec4 initialAtten;
	glm::vec4 direction; // Spot, directional lights
	glm::vec4 params;	 // x = Light::LIGHT_TYPE, y = inner angle, z = outer angle, w = TBD
	bool status;	     // x = 0 for off, 1 for on
};