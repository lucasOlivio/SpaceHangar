#include "scene/Scene.h"
#include "components.h"

Scene::Scene(iEvent* pKeyEvents)
{
    this->m_isPlaying = false;
    this->m_pKeyEvents = pKeyEvents;
    this->m_numEntities = 0;
    this->m_components.clear();
}

Scene::~Scene()
{
    this->m_components.clear();
}

void Scene::Clear()
{
    for (auto& pairComponent : this->m_components) {
        for (auto& pairEntityComp : pairComponent.second) {
            if (pairComponent.first == "player")
            {
                this->m_pKeyEvents->Dettach((PlayerComponent*)pairEntityComp.second);
            }
            delete pairEntityComp.second;
        }
        pairComponent.second.clear();
    }
    this->m_components.clear();
    this->m_numEntities = 0;
}

EntityID Scene::GetNumEntities()
{
    return this->m_numEntities;
}

EntityID Scene::CreateEntity()
{
    EntityID newEntityID = this->m_numEntities;
    this->m_numEntities++;
    return newEntityID;
}

void Scene::DeleteEntity(EntityID entityID)
{
    for (auto& pairComponent : this->m_components) {
        auto& innerMap = pairComponent.second;
        auto entityIter = innerMap.find(entityID);
        if (entityIter != innerMap.end()) {
            delete entityIter->second;
            innerMap.erase(entityIter);
        }
    }
}

bool Scene::GetMapComponents(std::string componentName, std::map<EntityID, iComponent*>& componentsOut)
{
    std::map<std::string,
        std::map<EntityID, iComponent*>>::iterator it;
    it = this->m_components.find(componentName);

    if (it == this->m_components.end())
    {
        // No components of this type
        return false;
    }

    componentsOut = it->second;
    return true;
}

iComponent* Scene::GetComponent(EntityID entityID, std::string componentName)
{
    std::map<EntityID, iComponent*> mapComponents;

    this->GetMapComponents(componentName, mapComponents);

    std::map<EntityID, iComponent*>::iterator it;
    it = mapComponents.find(entityID);

    if (it == mapComponents.end())
    {
        // Entity doesn't have this kind of component
        return nullptr;
    }

    return it->second;
}

void Scene::SetComponent(EntityID entityID, std::string componentName, iComponent* componentIn)
{
    // Check the entity already have this component
    iComponent* compToDelete = this->GetComponent(entityID, componentName);

    // If already have, delete it first
    if (compToDelete)
    {
        delete compToDelete;
    }

    // Now replace with the new component
    this->m_components[componentName][entityID] = componentIn;
    componentIn->SetGameplayDirector(this);

    if (componentName == "player")
    {
        PlayerComponent* pPlayer = (PlayerComponent*)componentIn;
        this->m_pKeyEvents->Attach(pPlayer);
        pPlayer->SetPlaying(this->IsPlaying());
    }

    return;
}

std::vector<sComponentInfo> Scene::GetComponentsInfo(EntityID entityID)
{
    std::vector<sComponentInfo> componentsInfo;

    // Go through each component type in the scene
    for (const auto& pairComponents : this->m_components)
    {
        // Check if user has this component
        const auto& itComponents = pairComponents.second.find(entityID);
        if (itComponents == pairComponents.second.end())
        {
            continue;
        }

        // Get info from component and attach to vector
        sComponentInfo componentInfo;
        itComponents->second->GetInfo(componentInfo);

        componentsInfo.push_back(componentInfo);
    }

    return componentsInfo;
}

void Scene::SendAction(std::string action, EntityID entityID, sParameterInfo& parameterIn)
{
    if (action == "moveTransform")
    {
        TransformComponent* pTransform = (TransformComponent*)this->GetComponent(entityID, "transform");
        if (pTransform)
        {
            pTransform->Move(parameterIn.parameterVec3Value);
        }
    }
    if (action == "toggleAnimation")
    {
        AnimationComponent* pAnimate = (AnimationComponent*)this->GetComponent(entityID, "animation");
        if (pAnimate)
        {
            pAnimate->Toggle();
        }
    }
    if (action == "adjustDim")
    {
        LightComponent* pLight = (LightComponent*)this->GetComponent(entityID, "light");
        if (pLight)
        {
            pLight->SetLinearAtten(parameterIn.parameterFloatValue);
        }
    }
    if (action == "resetLight")
    {
        LightComponent* pLight = (LightComponent*)this->GetComponent(entityID, "light");
        if (pLight)
        {
            pLight->ResetAtten();
        }
    }
    if (action == "triggerAnimation")
    {
        std::map<EntityID, iComponent*> mapModels;
        this->GetMapComponents("model", mapModels);
        int entityIDB = -1;
        for (std::pair<EntityID, iComponent*> modelPair : mapModels)
        {
            ModelComponent* pModel = (ModelComponent*)modelPair.second;
            if (pModel->friendlyName == parameterIn.parameterStrValue)
            {
                entityIDB = modelPair.first;
            }
        }

        if (entityIDB < 0)
        {
            return;
        }

        AnimationComponent* pAnimate = (AnimationComponent*)this->GetComponent(entityIDB, "animation");
        if (pAnimate)
        {
            pAnimate->Toggle();
        }
    }

    return;
}

bool Scene::IsPlaying()
{
    if (this->m_isPlaying)
    {
        return true;
    } 
    else
    {
        return false;
    }
}

void Scene::SetPlaying(bool isPlaying)
{
    this->m_isPlaying = isPlaying;
}
