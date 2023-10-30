#include "components/Camera.h"
#include "common/utils.h"
#include <glm/gtx/string_cast.hpp>

void CameraComponent::GetInfo(sComponentInfo& compInfoOut)
{
	using namespace myutils;

	compInfoOut.componentName = "camera";
	compInfoOut.componentParameters.clear();

	this->AddCompParInfo("cameraEye", "vec3", this->cameraEye, compInfoOut);
	this->AddCompParInfo("cameraTarget", "vec3", this->cameraTarget, compInfoOut);
	this->AddCompParInfo("upVector", "vec3", this->upVector, compInfoOut);
	this->AddCompParInfo("isActive", "bool", this->isActive, compInfoOut);
}

void CameraComponent::SetParameter(sParameterInfo& parameterIn)
{
    using namespace myutils;

	if (parameterIn.parameterName == "cameraEye") {
        this->cameraEye = parameterIn.parameterVec3Value;
    }
    else if (parameterIn.parameterName == "cameraTarget") {
        this->cameraTarget = parameterIn.parameterVec3Value;
    }
    else if (parameterIn.parameterName == "upVector") {
        this->upVector = parameterIn.parameterVec3Value;
    }
    else if (parameterIn.parameterName == "isActive") {
        this->isActive = parameterIn.parameterBoolValue;
    }

    return;
}
