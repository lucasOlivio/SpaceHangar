#include "EngineRenderer/Renderer.h"
#include "common/utils.h"
#include "components/Animation.h"
#include "components/Model.h"
#include "components/Transform.h"
#include "components/Tiling.h"
#include "components/Light.h"

Renderer::Renderer()
{
	this->m_isRunning = false;
	this->m_isInitialized = false;
	
	this->m_currShaderID = -1;
	this->m_pShaderManager = nullptr;
	
	this->m_pWindowSystem = nullptr;
	this->m_pCameraSystem = nullptr;
	this->m_pModelSystem = nullptr;
	this->m_pLightSystem = nullptr;

	this->m_pSceneView = nullptr;
}

Renderer::~Renderer()
{
	delete this->m_pShaderManager;

	delete this->m_pWindowSystem;
	delete this->m_pCameraSystem;
	delete this->m_pModelSystem;
	delete this->m_pLightSystem;
}

bool Renderer::Initialize(std::string baseShadersPath,
						  std::string baseModelsPath,
						  std::string shaderName,
						  uint windowWidth,
						  uint windowHeight,
						  const std::string& windowName,
						  GLFWkeyfun KeyCallback,
						  SceneView* pSceneView)
{
	if (this->m_isInitialized)
	{
		return true;
	}

	this->m_pSceneView = pSceneView;

	printf("Initializing engine renderer...\n");
	this->m_pShaderManager = new ShaderManager(baseShadersPath);
	// All systems that update info in shaders must have the shader info
	// To set their respectives uniforms and attributes
	this->m_pWindowSystem = new WindowSystem(this->m_pShaderManager);
	this->m_pCameraSystem = new CameraSystem(this->m_pShaderManager, pSceneView);
	this->m_pModelSystem = new ModelSystem(this->m_pShaderManager, pSceneView);
	this->m_pLightSystem = new LightSystem(this->m_pShaderManager, pSceneView);

	bool isWSInitialized = this->m_pWindowSystem->Initialize(windowWidth, 
															 windowHeight, 
															 windowName, 
															 KeyCallback);
	if (!isWSInitialized)
	{
		CheckEngineError("initializing Window system");
		return false;
	}

	printf("Creating shaders...\n");
	// Creates main shader program
	bool isShaderCreated = this->m_pShaderManager->AddShaderProgram(shaderName);
	if (!isShaderCreated)
	{
		CheckEngineError("creating shader program");
		return false;
	}
	this->m_currShaderID = this->m_pShaderManager->GetIDFromShaderProgramName(shaderName);
	this->m_pShaderManager->UseShaderProgram(this->m_currShaderID);

	// Setting up lights
	bool isLSInitialized = this->m_pLightSystem->Initialize(this->m_currShaderID);
	if (!isLSInitialized)
	{
		CheckEngineError("setting up lights");
		return false;
	}

	// Loading models into VAO and getting IDs
	this->LoadScene(baseModelsPath);

	this->m_isInitialized = true;
	this->m_isRunning = true;

    return true;
}

void Renderer::Destroy()
{
	if (!this->m_isInitialized)
	{
		return;
	}

	this->m_pWindowSystem->Destroy();

	this->m_isInitialized = false;

	return;
}

void Renderer::LoadScene(std::string baseModelsPath)
{
	this->m_pModelSystem->LoadModels(baseModelsPath, this->m_currShaderID);
	this->m_pLightSystem->Initialize(this->m_currShaderID);
}

void Renderer::NewFrame()
{
	if (!this->m_isInitialized)
	{
		return;
	}

	this->m_pShaderManager->UseShaderProgram(this->m_currShaderID);
	this->m_pWindowSystem->NewFrame(this->m_currShaderID);

	return;
}

void Renderer::DrawModel(EntityID entityID, double deltaTime)
{
	if (!this->m_isInitialized)
	{
		return;
	}

	this->m_pWindowSystem->UpdateUL(this->m_currShaderID);
	this->m_pCameraSystem->UpdateUL(this->m_currShaderID);

	// TODO: Remove code duplication for all these components
	TilingComponent* pTiling = this->m_pSceneView->GetComponent<TilingComponent>(entityID, "tiling");
	TransformComponent* pTransform = this->m_pSceneView->GetComponent<TransformComponent>(entityID, "transform");
	AnimationComponent* pAnimation = this->m_pSceneView->GetComponent<AnimationComponent>(entityID, "animation");
	ModelComponent* pModel = this->m_pSceneView->GetComponent<ModelComponent>(entityID, "model");
	LightComponent* pLight = this->m_pSceneView->GetComponent<LightComponent>(entityID, "light");

	// Default we only draw 1 time in each axis
	glm::vec3 axis = glm::vec3(1.0, 1.0, 1.0);
	glm::vec3 offset = glm::vec3(0.0, 0.0, 0.0);
	// If has tiling then we draw X times per axis based on the offset
	if (pTiling)
	{
		axis = pTiling->GetAxis();
		offset = pTiling->GetOffset();
	}

	if (pAnimation)
	{
		pAnimation->Update(deltaTime, this->m_currShaderID, this->m_pShaderManager);
	}

	if (pTransform)
	{
		pTransform->SetFramePosition();
	}

	// Now go for each axis tiling to draw adding the offset
	for (int x = 0; x < axis[0]; x++)
	{
		for (int y = 0; y < axis[1]; y++)
		{
			for (int z = 0; z < axis[2]; z++)
			{
				if (pTransform)
				{
					if (pLight)
					{
						pTransform->SetPosition(pLight->GetPosition() + pLight->GetDirection());
					}
					glm::vec3 delta = offset;
					delta.x = offset.x * x;
					delta.y = offset.y * y;
					delta.z = offset.z * z;
					pTransform->Move(delta);
					pTransform->Update(this->m_currShaderID, this->m_pShaderManager);
				}

				if (pModel)
				{
					pModel->Update(deltaTime, this->m_currShaderID, this->m_pShaderManager);
					pModel->Render();
				}

				// Come back to initial frame place
				if (pTransform)
				{
					pTransform->ResetFramePosition();
				}
			}
		}
	}

	return;
}

void Renderer::EndFrame()
{
	if (!this->m_isInitialized)
	{
		return;
	}

	this->m_pWindowSystem->EndFrame();

	return;
}

bool Renderer::IsRunning()
{
	if (this->m_isRunning
		&& !this->m_pWindowSystem->WindowShouldClose())
	{
		return true;
	}
	else
	{
		return false;
	}
}
