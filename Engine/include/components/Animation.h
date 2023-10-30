#pragma once

#include "Component.h"
#include "common/drawInfo.h"
#include "EngineRenderer/iShaderInfo.h"
#include <vector>

enum eAnimType
{
	SPRITE,
	TRANSFORM,
	LIGHT_DIMMER
};

class AnimationComponent : public Component
{
private:
	eAnimType m_type;
	bool m_isActive;
	double m_elapsedTime;

	// Light dimmer animations
	float m_amplitude;
	float m_frequency;
	float m_offset;

	void m_UpdateSinValue();

	// Transform animations
	glm::vec3 m_direction;
	float m_duration;		   // How many seconds should run
	float m_ups;			   // Units/sec it will move
	std::string m_triggerName; // Other model to trigger
	float m_triggerValue;	   // Bellow what "range" should trigger

	void m_UpdateTransform(double deltaTime);

	// sprite animations
	int m_currFrame;
	std::vector <sMesh*> m_pMeshes;

	void m_UpdateFrames();
	void m_UpdateUniforms(uint shaderID, iShaderInfo* pShaderInfo);

public:
	AnimationComponent();
	virtual ~AnimationComponent();

	std::vector<std::string> models;
	std::string friendlyName;
	bool isWireframe;
	bool doNotLight;

	// Toggle animation back and forth
	void Toggle();

	void SetMesh(uint index, sMesh* pMesh);

	virtual void GetInfo(sComponentInfo& compInfoOut);
	virtual void SetParameter(sParameterInfo& parameterIn);

	void Update(double deltaTime, uint shaderID, iShaderInfo* pShaderInfo);

	void Render();
};