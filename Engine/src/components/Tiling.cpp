#include "components/Tiling.h"
#include "common/utils.h"
#include <glm/gtx/string_cast.hpp>

glm::vec3 TilingComponent::GetAxis()
{
    return this->m_axis;
}

glm::vec3 TilingComponent::GetOffset()
{
    return this->m_offset;
}

void TilingComponent::GetInfo(sComponentInfo& compInfoOut)
{
    using namespace myutils;

    compInfoOut.componentName = "tiling";
    compInfoOut.componentParameters.clear();

    this->AddCompParInfo("axis", "vec3", this->m_axis, compInfoOut);
    this->AddCompParInfo("offset", "vec3", this->m_offset, compInfoOut);
}

void TilingComponent::SetParameter(sParameterInfo& parameterIn)
{
    using namespace myutils;

    if (parameterIn.parameterName == "axis") {
        this->m_axis = parameterIn.parameterVec3Value;
    }
    else if (parameterIn.parameterName == "offset") {
        this->m_offset = parameterIn.parameterVec3Value;
    }

    return;
}
