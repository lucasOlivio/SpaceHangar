#include "common/utils.h"
#include <sstream>
#include <glm/gtx/string_cast.hpp>
#include <iomanip>
#include <sstream>

void __CheckEngineError(const char* msg, const char* file, int line)
{
    printf("Egine error file '%s' line '%d': %s\n", file, line, msg);
    return;
}

std::string myutils::GetFileExtension(const std::string& fileName)
{
    size_t dotPos = fileName.find_last_of(".");
    if (dotPos != std::string::npos)
    {
        return fileName.substr(dotPos + 1);
    }
    return fileName; // Full string if no extension found
}

void myutils::SplitString(const std::string& stringIn,
    char delimiter,
    std::vector<std::string> vecOut)
{
    std::istringstream iss(stringIn.c_str());
    std::string token;

    vecOut.clear();
    while (std::getline(iss, token, delimiter)) {
        vecOut.push_back(token);
    }
}

template <typename T>
std::string myutils::ArrayToString(char delimiter, const std::vector<T>& arr) {
    std::ostringstream oss;

    if (arr.empty()) {
        return "";
    }

    oss << arr[0];
    for (size_t i = 1; i < arr.size(); ++i) {
        oss << delimiter << arr[i];
    }

    return oss.str();
}

std::string myutils::BoolToString(bool input) {
    return input ? "1" : "0";
}

int myutils::Sign(int val)
{
    return (0 < val) - (val < 0);
}

float myutils::GetRandFloat(float a, float b)
{
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

void myutils::WrapMinMax(int min, int max, int& numOut)
{
    if (numOut < min)
    {
        numOut = max;
    }
    else if (numOut > max)
    {
        numOut = min;
    }
}

void myutils::WrapMinMax(float min, float max, float& numOut)
{
    numOut = std::min(std::max(numOut, 0.0f), max - 1);
}

glm::vec4 myutils::StringToVec4(const std::string& glmstr) 
{
    glm::vec4 result;

    std::string values = glmstr.substr(glmstr.find('(') + 1, glmstr.find(')') - glmstr.find('(') - 1);
    std::stringstream ss(values);
    ss >> result.x >> result.y >> result.z >> result.w;

    return result;
}

glm::vec3 myutils::StringToVec3(const std::string& glmstr) 
{
    glm::vec3 result;

    std::string values = glmstr.substr(glmstr.find('(') + 1, glmstr.find(')') - glmstr.find('(') - 1);
    std::stringstream ss(values);
    ss >> result.x >> result.y >> result.z;

    return result;
}

std::string myutils::VecToString(glm::vec3 glmVec)
{
    std::string result = glm::to_string(glmVec);

    // Find and replace "vec3(" with "["
    size_t startPos = result.find("vec3(");
    if (startPos != std::string::npos) {
        result.replace(startPos, 5, "[");
    }

    // Find and replace ")" with "]"
    size_t endPos = result.find(")");
    if (endPos != std::string::npos) {
        result.replace(endPos, 1, "]");
    }

    return result;
}

std::string myutils::VecToString(glm::vec4 glmVec)
{
    std::string result = glm::to_string(glmVec);

    // Find and replace "vec3(" with "["
    size_t startPos = result.find("vec4(");
    if (startPos != std::string::npos) {
        result.replace(startPos, 5, "[");
    }

    // Find and replace ")" with "]"
    size_t endPos = result.find(")");
    if (endPos != std::string::npos) {
        result.replace(endPos, 1, "]");
    }

    return result;
}

double myutils::FormatFloatWithPrecision(float value, int decimalPlaces) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(decimalPlaces) << value;
    return std::stod(stream.str());
}