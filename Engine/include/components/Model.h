#pragma once

#include "Component.h"
#include "common/drawInfo.h"
#include "EngineRenderer/iShaderInfo.h"
#include <string>

class ModelComponent : public Component
{
public:
	std::string name;
	std::string friendlyName;
	bool isWireframe;
	bool doNotLight;

	sMesh* pMeshInfo;

	void Update(double deltaTime, uint shaderID, iShaderInfo* pShaderInfo);
	void Render();

	virtual void GetInfo(sComponentInfo& compInfoOut);
	virtual void SetParameter(sParameterInfo& parameterIn);
};