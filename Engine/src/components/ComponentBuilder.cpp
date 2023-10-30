#include "components/ComponentBuilder.h"
#include "components.h"

ComponentBuilder::ComponentBuilder(iScene* pScene)
	: m_pScene(pScene)
{
}

iComponent* ComponentBuilder::BuildComponent(sComponentInfo& componentInfo, EntityID entityID)
{
	iComponent* newComponent = nullptr;

	if (componentInfo.componentName == "model")
	{
		newComponent = new ModelComponent();
	}
	else if (componentInfo.componentName == "transform")
	{
		newComponent = new TransformComponent();
	}
	else if (componentInfo.componentName == "camera")
	{
		newComponent = new CameraComponent();
	}
	else if (componentInfo.componentName == "light")
	{
		newComponent = new LightComponent();
	}
	else if (componentInfo.componentName == "force")
	{
		newComponent = new ForceComponent();
	}
	else if (componentInfo.componentName == "collision")
	{
		newComponent = new CollisionComponent();
	}
	else if (componentInfo.componentName == "animation")
	{
		newComponent = new AnimationComponent();
	}
	else if (componentInfo.componentName == "tiling")
	{
		newComponent = new TilingComponent();
	}
	else if (componentInfo.componentName == "player")
	{
		newComponent = new PlayerComponent();
	}
	else
	{
		printf("Error: Invalid component '%s'\n", componentInfo.componentName.c_str());
		return nullptr;
	}

	// Every created component we already set the entityID it belongs and sends to scene
	newComponent->SetEntityID(entityID);
	for (sParameterInfo parameter : componentInfo.componentParameters)
	{
		newComponent->SetParameter(parameter);
	}
	this->m_pScene->SetComponent(entityID, componentInfo.componentName, newComponent);

	return newComponent;
}
