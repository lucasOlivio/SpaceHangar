#pragma once

#include "scene/SceneView.h"
#include "iShaderInfo.h"
#include <glm/mat4x4.hpp>

class CameraSystem
{
private:
	iShaderInfo* m_pShaderInfo;
	SceneView* m_pSceneView;
public:
	// ctors & dtors
	CameraSystem(iShaderInfo* pShaderInfo, SceneView* pSceneView);
	~CameraSystem();

	// Called each frame
	void UpdateUL(int shaderID);
};