#include "EngineRenderer/CameraSystem.h"
#include "common/opengl.h"
#include "components/Camera.h"
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

CameraSystem::CameraSystem(iShaderInfo* pShaderInfo, SceneView* pSceneView)
{
    this->m_pShaderInfo = pShaderInfo;
    this->m_pSceneView = pSceneView;
}

CameraSystem::~CameraSystem()
{
}

void CameraSystem::UpdateUL(int shaderID)
{
    // Update projection matrix in shader
    for (this->m_pSceneView->First("camera"); !this->m_pSceneView->IsDone(); this->m_pSceneView->Next())
    {
        CameraComponent* pCamera = this->m_pSceneView->CurrentValue<CameraComponent>();
        if (!pCamera->isActive)
        {
            continue;
        }

        glm::mat4 matView = glm::lookAt(pCamera->cameraEye,
                                        pCamera->cameraTarget,
                                        pCamera->upVector);
        int matViewUL = this->m_pShaderInfo->GetUL(shaderID, "matView");
        glUniformMatrix4fv(matViewUL, 1, GL_FALSE, glm::value_ptr(matView));

        return; // We only need one active camera
    }
}
