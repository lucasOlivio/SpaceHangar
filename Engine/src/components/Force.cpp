#include "components/Force.h"

void ForceComponent::SetInverseMass(float value)
{
    this->m_inverseMass = value;
}

void ForceComponent::SetAcceleration(glm::vec3 value)
{
    this->m_acceleration = value;
}

void ForceComponent::SetVelocity(glm::vec3 value)
{
    this->m_velocity = value;
}

float ForceComponent::GetInverseMass()
{
    return this->m_inverseMass;
}

glm::vec3 ForceComponent::GetAcceleration()
{
    return this->m_acceleration;
}

glm::vec3 ForceComponent::GetVelocity()
{
    return this->m_velocity;
}

void ForceComponent::GetInfo(sComponentInfo& compInfoOut)
{
    compInfoOut.componentName = "force";
    compInfoOut.componentParameters.clear();

    this->AddCompParInfo("inverseMass", "float", this->GetInverseMass(), compInfoOut);
    this->AddCompParInfo("acceleration", "vec3", this->GetAcceleration(), compInfoOut);
    this->AddCompParInfo("velocity", "vec3", this->GetVelocity(), compInfoOut);

    return;
}

void ForceComponent::SetParameter(sParameterInfo& parameterIn)
{
    if (parameterIn.parameterName == "inverseMass") {
        this->SetInverseMass(parameterIn.parameterFloatValue);
    }
    else if (parameterIn.parameterName == "acceleration") {
        this->SetAcceleration(parameterIn.parameterVec3Value);
    }
    else if (parameterIn.parameterName == "velocity") {
        this->SetVelocity(parameterIn.parameterVec3Value);
    }

    return;
}
