#include "components/Collision.h"

CollisionComponent::CollisionComponent()
{
	this->m_eShape = eShape::UNKNOWN_OR_UNDEFINED;
	this->m_pShape = nullptr;
}

CollisionComponent::~CollisionComponent()
{
	if (this->m_pShape) 
	{
		delete this->m_pShape;
	}
}

void CollisionComponent::SetShape(eShape shape, sParameterInfo vecParameterIn)
{
	void* pShape = nullptr;
	// TODO: way to be more dynamic here
	if (shape == eShape::SPHERE)
	{
		pShape = new sSphere(vecParameterIn.parameterFloatValue);
	}
	else if (shape == eShape::MESH_OF_TRIANGLES_INDIRECT)
	{
		pShape = new sMeshOfTriangles_Indirect(vecParameterIn.parameterStrValue);
	}
	else
	{
		printf("Shape #%d not implemented yet...", shape);
		return;
	}

	if (this->m_pShape)
	{
		delete this->m_pShape;
	}

	this->m_pShape = pShape;
	this->m_eShape = shape;
}

eShape CollisionComponent::Get_eShape()
{
	return this->m_eShape;
}

void CollisionComponent::GetInfo(sComponentInfo& compInfoOut)
{
	compInfoOut.componentName = "collision";
	compInfoOut.componentParameters.clear();

	this->AddCompParInfo("shape", "int", this->m_eShape, compInfoOut);
	
	// TODO: way to be more dynamic here
	if (this->m_eShape == eShape::SPHERE)
	{
		sSphere* sphere = (sSphere*)this->m_pShape;
		this->AddCompParInfo("radius", "float", sphere->radius, compInfoOut);
	}
	else if (this->m_eShape == eShape::MESH_OF_TRIANGLES_INDIRECT)
	{
		sMeshOfTriangles_Indirect* meshIndirect = (sMeshOfTriangles_Indirect*)this->m_pShape;
		this->AddCompParInfo("meshName", "string", meshIndirect->meshName, compInfoOut);
	}

	return;
}

void CollisionComponent::SetParameter(sParameterInfo& parameterIn)
{
	if (parameterIn.parameterName == "shape") {
		this->m_eShape = (eShape)parameterIn.parameterIntValue;
	}
	else // Any other CollisionComponent parameter should come before here
	{
		// TODO: This can't depend on "shape" been setted first
		this->SetShape(this->m_eShape, parameterIn);
	}
}
