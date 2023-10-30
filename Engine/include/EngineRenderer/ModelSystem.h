#pragma once

#include "scene/SceneView.h"
#include "VAOManager.h"
#include "iShaderInfo.h"
#include <glm/mat4x4.hpp>

class ModelSystem
{
private:
	VAOManager* m_pVAOManager;
	iShaderInfo* m_pShaderInfo;
	SceneView* m_pSceneView;

public:
	// ctors & dtors
	ModelSystem(iShaderInfo* pShaderInfo, SceneView* pSceneView);
	~ModelSystem();

	// Where we really start/destroy everything that could go wrong
	bool LoadModels(int shaderID);
	bool LoadModels(std::string basePath, int shaderID);
	void Destroy();
};