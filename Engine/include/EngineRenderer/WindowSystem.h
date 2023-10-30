#pragma once

#include "iShaderInfo.h"
#include "common/types.h"
#include "common/opengl.h"
#include <glm/mat4x4.hpp>

class WindowSystem
{
private:
	GLFWwindow* m_pWindow;
	iShaderInfo* m_pShaderInfo;

	int m_width;
	int m_height;
	float m_ratio;
public:
	// ctors & dtors
	WindowSystem(iShaderInfo* pShaderInfo);
	~WindowSystem();

	// Where we really start/destroy everything that could go wrong
	bool Initialize(uint width, uint height, const std::string& windowName, GLFWkeyfun KeyCallback);
	void Destroy();

	// Clear all and bind opengl parameters
	void NewFrame(int shaderID);
	void UpdateUL(int shaderID);
	// Swap frames and buffers
	void EndFrame();

	bool WindowShouldClose();

	float GetCurrentWindowRatio();
};