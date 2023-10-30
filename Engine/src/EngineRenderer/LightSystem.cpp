#include "EngineRenderer/LightSystem.h"
#include "components/Light.h"

LightSystem::LightSystem(iShaderInfo* pShaderInfo, SceneView* pSceneView)
{
	this->m_pShaderInfo = pShaderInfo;
	this->m_pSceneView = pSceneView;
}

LightSystem::~LightSystem()
{
}

bool LightSystem::Initialize(int shaderID)
{
    // Go over each lights setting the ULs and initializing them
    int lightIndex = 0;
    for (this->m_pSceneView->First("light"); !this->m_pSceneView->IsDone(); this->m_pSceneView->Next())
    {
        std::string ulBasePath = "theLights[" + std::to_string(lightIndex) + "].";

        LightComponent* pLight = this->m_pSceneView->CurrentValue<LightComponent>();
        pLight->SetupLight(shaderID, ulBasePath);
        lightIndex++;
    }

    return true;
}
