#include "components/Model.h"
#include "common/utils.h"
#include "common/opengl.h"

void ModelComponent::GetInfo(sComponentInfo& compInfoOut)
{
	using namespace myutils;

	compInfoOut.componentName = "model";
	compInfoOut.componentParameters.clear();

	this->AddCompParInfo("name", "string", this->name, compInfoOut);
	this->AddCompParInfo("friendlyName", "string", this->friendlyName, compInfoOut);
	this->AddCompParInfo("isWireframe", "bool", this->isWireframe, compInfoOut);
	this->AddCompParInfo("doNotLight", "bool", this->doNotLight, compInfoOut);
}

void ModelComponent::SetParameter(sParameterInfo& parameterIn)
{
    using namespace myutils;

    if (parameterIn.parameterName == "name") {
        this->name = parameterIn.parameterStrValue;
    }
    else if (parameterIn.parameterName == "friendlyName") {
        this->friendlyName = parameterIn.parameterStrValue;
    }
    else if (parameterIn.parameterName == "isWireframe") {
        this->isWireframe = parameterIn.parameterBoolValue;
    }
    else if (parameterIn.parameterName == "doNotLight") {
        this->doNotLight = parameterIn.parameterBoolValue;
    }

    return;
}

void ModelComponent::Update(double deltaTime, uint shaderID, iShaderInfo* pShaderInfo)
{
    // Update uniforms
    //-------------------------------------------
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

void ModelComponent::Render()
{
    // Bind to the VAO and call opengl to draw all vertices
    glBindVertexArray(this->pMeshInfo->VAO_ID); //  enable VAO (and everything else)
    glDrawElements(GL_TRIANGLES,
        this->pMeshInfo->numberOfIndices,
        GL_UNSIGNED_INT,
        0);
    glBindVertexArray(0); 			  // disable VAO (and everything else)

    return;
}