// Enable strings for rapidjson
#define RAPIDJSON_HAS_STDSTRING 1 

#include "scene/SceneParserJSON.h"
#include "common/ParserJSON.h"
#include "components.h" // TODO: Get rid of this
#include "components/ComponentBuilder.h"
#include "common/utils.h"
#include <sstream>
#include <iomanip>

SceneParserJSON::SceneParserJSON()
{
}

SceneParserJSON::~SceneParserJSON()
{
}

bool SceneParserJSON::ParseFromJsonObj(rapidjson::Value& jsonObject, iScene* pSceneOut)
{
    using namespace rapidjson;


    bool isValid = jsonObject.IsArray();
    if (!isValid)
    {
        printf("Error: File not valid, expected array of entities!\n");
        return false;
    }

    ParserJSON parser = ParserJSON();
    ComponentBuilder componentBuilder = ComponentBuilder(pSceneOut);

    // TODO: Redo here so we don't need to edit everytime there is a new component.
    // Components should know how to serialize and deserialize themselves
    // 
    // Go over each entity
    for (uint entityIndex = 0; entityIndex < jsonObject.Size(); entityIndex++)
    {
        // This will be our entity ID count, 
        // so we can keep track easily later how many entities we heave
        EntityID entityID = pSceneOut->CreateEntity();

        Value& entityObject = jsonObject[entityIndex];
        bool isValid = entityObject.IsObject();
        if (!isValid)
        {
            printf("Error: Entity #%d not valid, expected object of components!\n", entityIndex);
            return false;
        }

        // Inside EntityID iterate over each component
        for (Value::ConstMemberIterator entityMember = entityObject.MemberBegin(); entityMember != entityObject.MemberEnd(); ++entityMember)
        {
            sComponentInfo componentInfo = sComponentInfo();
            componentInfo.componentName = entityMember->name.GetString();

            Value& componentObject = jsonObject[entityIndex][entityMember->name];
            bool isValid = componentObject.IsObject();
            if (!isValid)
            {
                printf("Error: Entity #%d not valid, expected object of components!\n", entityIndex);
                return false;
            }

            sParameterInfo paramInfo;
            // Inside component go over each parameter, parsing the parameterInfo and sending to component
            for (Value::ConstMemberIterator compMember = componentObject.MemberBegin(); compMember != componentObject.MemberEnd(); ++compMember)
            {
                paramInfo.parameterName = compMember->name.GetString();
                Value& parameterObject = jsonObject[entityIndex][entityMember->name][compMember->name];

                bool isParsed = parser.GetValue(parameterObject, paramInfo);

                if (!isParsed)
                {
                    std::string errorMsg = "Error parsing parameter '" + paramInfo.parameterName + "' from component '" +
                                            componentInfo.componentName + "' entity #" + std::to_string(entityID);
                    CheckEngineError(errorMsg.c_str());
                    return false;
                }

                componentInfo.componentParameters.push_back(paramInfo);
            }

            // For every component we create it on the scene them update using the parameters info common structure
            iComponent* newComponent = componentBuilder.BuildComponent(componentInfo, entityID);
            if (!newComponent)
            {
                std::string errorMsg = "Component '" + componentInfo.componentName + "' from entity #" + std::to_string(entityID) + " doesn't exists";
                CheckEngineError(errorMsg.c_str());
                return false;
            }
        }
    }

    return true;
}

bool SceneParserJSON::ParseToJsonObj(iScene* pScene,
    rapidjson::Value& jsonObjectOut,
    rapidjson::Document::AllocatorType& allocator)
{
    using namespace rapidjson;
    using namespace myutils;

    ParserJSON parser = ParserJSON();

    jsonObjectOut.SetArray();
    for (EntityID entityID = 0; entityID < pScene->GetNumEntities(); ++entityID)
    {
        // Create a RapidJSON object for each entity
        Value entityObject;
        entityObject.SetObject();

        // Iterate over the components of the entity
        for (sComponentInfo component : pScene->GetComponentsInfo(entityID))
        {
            Value componentName(component.componentName, allocator);

            // Create a RapidJSON object for each component
            Value componentObject;
            componentObject.SetObject();

            for (sParameterInfo parameter : component.componentParameters)
            {
                Value jsonValue;
                bool isParsed = parser.SetValue(jsonValue, parameter, allocator);
                if (!isParsed)
                {
                    std::string errorMsg = "Error parsing parameter '" + parameter.parameterName + "' from component '" + component.componentName + "' entity #" + std::to_string(entityID);
                    CheckEngineError(errorMsg.c_str());
                    return false;
                }

                Value paramName(parameter.parameterName, allocator);
                componentObject.AddMember(paramName,
                    jsonValue, allocator);
            }

            // Add the componentObject to the entityObject
            entityObject.AddMember(componentName, componentObject, allocator);
        }

        // Add the entityObject to the JSON array
        jsonObjectOut.PushBack(entityObject, allocator);
    }

    return true;
}