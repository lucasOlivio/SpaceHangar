#pragma once

#include "Component.h"
#include "EnginePhysics/PhysicsProperties.hpp"
#include <glm/vec3.hpp>

class CollisionComponent : public Component
{
private:
	// TODO: Reestructure this so that we have an interface for shapes
	eShape m_eShape;
	void* m_pShape;

public:
	CollisionComponent();
	virtual ~CollisionComponent();

	// Get the shape object alread casted
	template<class T>
	T* GetShape();

	eShape Get_eShape();

	// Create the given shape using the parameters
	void SetShape(eShape shape, sParameterInfo vecParameterIn);

	virtual void GetInfo(sComponentInfo& compInfoOut);
	virtual void SetParameter(sParameterInfo& parameterIn);
};

// TODO: Restructure component and shapes to get rid of this
template<class T>
T* CollisionComponent::GetShape()
{
	return (T*)this->m_pShape;
}