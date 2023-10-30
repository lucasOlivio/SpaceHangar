#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

void __CheckEngineError(const char* msg, const char* file, int line);
#define CheckEngineError(msg) __CheckEngineError(msg, __FILE__, __LINE__)

namespace myutils
{
    // Get string after last "." in file name
    std::string GetFileExtension(const std::string& fileName);

    // Split string based on delimiter
    void SplitString(const std::string& stringIn,
                     char delimiter,
                     std::vector<std::string> vecOut);

    // Convert array to a string separated by delimiter
    template <typename T>
    std::string ArrayToString(char delimiter, const std::vector<T>& arrIn);

    // Converts bool to strings "0" or "1"
    std::string BoolToString(bool input);

    // Get only sign of number (1 or -1)
    int Sign(int val);

    // Pseudo random float between a and b
    float GetRandFloat(float a, float b);

    // Wrap number between min and max (both end inclusives)
    void WrapMinMax(int min, int max, int& numOut);
    void WrapMinMax(float min, float max, float& numOut);

    void ModifyVec3(unsigned int axis, 
                    int orientation,
                    float changeStep,
                    glm::vec3& vecParameterOut);

    // Converts the glm vec string from "string_cast" back to glm::vec
    glm::vec4 StringToVec4(const std::string& glmstr);
    glm::vec3 StringToVec3(const std::string& glmstr);

    // Converts glm to an array string [x, y, z]
    std::string VecToString(glm::vec3 glmVec);
    std::string VecToString(glm::vec4 glmVec);

    double FormatFloatWithPrecision(float value, int decimalPlaces);
}
