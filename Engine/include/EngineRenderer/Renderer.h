#pragma once

#include "ShaderManager.h"
#include "WindowSystem.h"
#include "CameraSystem.h"
#include "ModelSystem.h"
#include "LightSystem.h"
#include "scene/SceneView.h"

class Renderer
{
private:
	bool m_isRunning;
	bool m_isInitialized;

	uint m_currShaderID;
	ShaderManager* m_pShaderManager;

	WindowSystem* m_pWindowSystem;
	CameraSystem* m_pCameraSystem;
	ModelSystem* m_pModelSystem;
	LightSystem* m_pLightSystem;

	SceneView* m_pSceneView;

public:
	// ctors & dtors
	Renderer();
	~Renderer();

	// TODO: The scene should be changed so that the systems can get only the
	// componen they need, to avoid the whole thing been passed everywhere 

	// Initializes all the systems, if anything go wrong we should stop here
	bool Initialize(std::string baseShadersPath,
					std::string baseModelsPath,
					std::string shaderName,
					uint windowWidth, 
					uint windowHeight, 
					const std::string& windowName, 
					GLFWkeyfun KeyCallback,
					SceneView* pSceneView);
	void Destroy();

	// Go through the scene loading each model component to the VAO
	void LoadScene(std::string baseModelsPath);

	// Clear frame, set shader and configure glfw for a new frame
	void NewFrame();
	// Update respectives UL and render model
	void DrawModel(EntityID entityID, double deltaTime);
	// Swap buffers and proccess glfw pool events
	void EndFrame();

	bool IsRunning();
};