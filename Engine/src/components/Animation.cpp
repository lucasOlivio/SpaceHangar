#include "components/Animation.h"
#include "common/utilsMat.h"
#include "common/opengl.h"
#include "common/constants.h"

const int FRAME_RATE = 30;  // Frames per second
const double FRAME_DURATION = 1.0 / FRAME_RATE;

AnimationComponent::AnimationComponent()
{
    this->m_currFrame = 0;
    this->m_elapsedTime = 0;
}

AnimationComponent::~AnimationComponent()
{
}

void AnimationComponent::m_UpdateSinValue()
{
    float deltaDim = myutils::CalculateSinWave(this->m_elapsedTime, this->m_amplitude, this->m_frequency, this->m_offset);

    sParameterInfo delta;
    delta.parameterFloatValue = deltaDim;
    this->m_pGameplayDirector->SendAction("adjustDim", this->GetEntityID(), delta);
}

void AnimationComponent::m_UpdateTransform(double deltaTime)
{
    if (this->m_elapsedTime >= this->m_triggerValue && this->m_triggerName != "")
    {
        sParameterInfo trigger;
        trigger.parameterStrValue = this->m_triggerName;
        this->m_pGameplayDirector->SendAction("triggerAnimation", this->GetEntityID(), trigger);
    }

    glm::vec3 deltaMove = this->m_direction * (this->m_ups * (float)deltaTime);

    sParameterInfo delta;
    delta.parameterVec3Value = deltaMove;
    this->m_pGameplayDirector->SendAction("moveTransform", this->GetEntityID(), delta);
}

void AnimationComponent::m_UpdateFrames()
{
    if (this->m_elapsedTime < FRAME_DURATION)
    {
        return;
    }

    // Update the animation index
    this->m_currFrame++;
    if (this->m_currFrame >= this->m_pMeshes.size()) {
        this->m_currFrame = 0;  // Loop the animation if needed
    }

    // Reset the elapsed time
    this->m_elapsedTime = 0.0;
}

void AnimationComponent::m_UpdateUniforms(uint shaderID, iShaderInfo* pShaderInfo)
{
    if (this->isWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Set if the mesh should calculate lightning in shader
    GLint doNotLightUL = pShaderInfo->GetUL(shaderID, "doNotLight");
    glUniform1f(doNotLightUL, this->doNotLight);
}

void AnimationComponent::Toggle()
{
    if (this->m_isActive)
    {
        // Can't change while animation is running
        return;
    }

    this->m_isActive = true;

    // If time already up then should go to otherway around
    if (this->m_duration > 0 && this->m_elapsedTime > this->m_duration)
    {
        this->m_direction = this->m_direction * -1.0f;
    }
    this->m_elapsedTime = 0;
}

void AnimationComponent::SetMesh(uint index, sMesh* pMesh)
{
    if (index > this->m_pMeshes.size())
    {
        return;
    }

    this->m_pMeshes[index] = pMesh;
}

void AnimationComponent::GetInfo(sComponentInfo& compInfoOut)
{
    compInfoOut.componentName = "animation";
    compInfoOut.componentParameters.clear();

    this->AddCompParInfo("type", "int", this->m_type, compInfoOut);
    this->AddCompParInfo("isActive", "bool", this->m_isActive, compInfoOut);
    this->AddCompParInfo("triggerName", "string", this->m_triggerName, compInfoOut);
    this->AddCompParInfo("triggerValue", "float", this->m_triggerValue, compInfoOut);
    this->AddCompParInfo("direction", "vec3", this->m_direction, compInfoOut);
    this->AddCompParInfo("duration", "float", this->m_duration, compInfoOut);
    this->AddCompParInfo("ups", "float", this->m_ups, compInfoOut);
    this->AddCompParInfo("amplitude", "float", this->m_amplitude, compInfoOut);
    this->AddCompParInfo("offset", "float", this->m_offset, compInfoOut);
    this->AddCompParInfo("frequency", "float", this->m_frequency, compInfoOut);
}

void AnimationComponent::SetParameter(sParameterInfo& parameterIn)
{
    using namespace myutils;

    if (parameterIn.parameterName == "type") {
        this->m_type = (eAnimType)parameterIn.parameterIntValue;
    }
    else if (parameterIn.parameterName == "isActive") {
        this->m_isActive = parameterIn.parameterBoolValue;
    }
    else if (parameterIn.parameterName == "elapsedTime") {
        this->m_elapsedTime = parameterIn.parameterFloatValue;
    }
    else if (parameterIn.parameterName == "triggerName") {
        this->m_triggerName = parameterIn.parameterStrValue;
    }
    else if (parameterIn.parameterName == "triggerValue") {
        this->m_triggerValue = parameterIn.parameterFloatValue;
    }
    else if (parameterIn.parameterName == "direction") {
        this->m_direction = parameterIn.parameterVec3Value;
    }
    else if (parameterIn.parameterName == "duration") {
        this->m_duration = parameterIn.parameterFloatValue;
    }
    else if (parameterIn.parameterName == "ups") {
        this->m_ups = parameterIn.parameterFloatValue;
    }
    else if (parameterIn.parameterName == "amplitude") {
        this->m_amplitude = parameterIn.parameterFloatValue;
    }
    else if (parameterIn.parameterName == "offset") {
        this->m_offset = parameterIn.parameterFloatValue;
    }
    else if (parameterIn.parameterName == "frequency") {
        this->m_frequency = parameterIn.parameterFloatValue;
    }

    return;
}

void AnimationComponent::Update(double deltaTime, uint shaderID, iShaderInfo* pShaderInfo)
{
    if (!this->m_isActive)
    {
        return;
    }

    if (this->m_duration > 0 && this->m_elapsedTime >= this->m_duration)
    {
        sParameterInfo ignore;
        this->m_pGameplayDirector->SendAction("resetLight", this->GetEntityID(), ignore);
        this->m_isActive = false;
        return;
    }

    this->m_elapsedTime += deltaTime;

    if (this->m_type == eAnimType::SPRITE)
    {
        this->m_UpdateFrames();

        this->m_UpdateUniforms(shaderID, pShaderInfo);
    }

    if (this->m_type == eAnimType::TRANSFORM)
    {
        this->m_UpdateTransform(deltaTime);
    }

    if (this->m_type == eAnimType::LIGHT_DIMMER)
    {
        this->m_UpdateSinValue();
    }
}

void AnimationComponent::Render()
{
    if (this->m_pMeshes.size() == 0)
    {
        return;
    }
    // Bind to the VAO and call opengl to draw all vertices
    sMesh* pCurrMesh = this->m_pMeshes[this->m_currFrame];
    glBindVertexArray(pCurrMesh->VAO_ID); //  enable VAO (and everything else)
    glDrawElements(GL_TRIANGLES,
        pCurrMesh->numberOfIndices,
        GL_UNSIGNED_INT,
        0);
    glBindVertexArray(0); 			  // disable VAO (and everything else)
}
