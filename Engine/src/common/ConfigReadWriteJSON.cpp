#include "common/ConfigReadWriteJSON.h"
#include "common/constants.h"
#include "scene/SceneParserJSON.h"
#include "common/utils.h"
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <fstream>

ConfigReadWriteJSON::ConfigReadWriteJSON()
{
}

ConfigReadWriteJSON::~ConfigReadWriteJSON()
{
}

bool ConfigReadWriteJSON::ReadScene(const std::string& filePath, iScene* pScene)
{
    using namespace rapidjson;

    // Load file stream into document object
    FILE* fp = 0;
    fopen_s(&fp, filePath.c_str(), "rb");

    if (!fp)
    {
        std::string errorMsg = "File '" + filePath + "' not found!";
        CheckEngineError(errorMsg.c_str());
        return false;
    }

    // TODO: Remove hardcoded value
    char readBuffer[MAX_LINE_LENGTH];
    FileReadStream fs(fp, readBuffer, sizeof(readBuffer));

    Document doc;
    doc.ParseStream(fs);

    fclose(fp);

    // Parse scene and load into structure
    SceneParserJSON sceneParser = SceneParserJSON();
    bool isParsed = sceneParser.ParseFromJsonObj(doc, pScene);
    if (!isParsed)
    {
        std::string errorMsg = "Error parsing file '" + filePath + "'";
        CheckEngineError(errorMsg.c_str());
        return false;
    }

    return true;
}

bool ConfigReadWriteJSON::WriteScene(const std::string& filePath, iScene* pScene)
{
    using namespace rapidjson;

    Document doc;        // rapidjson Document
    doc.SetArray();      // to store Array of objects
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    StringBuffer buf;
    PrettyWriter<StringBuffer> writer(buf);

    // Call the function to convert the iScene to a JSON document
    SceneParserJSON sceneParser = SceneParserJSON();
    bool isParsed = sceneParser.ParseToJsonObj(pScene, doc, allocator);
    if (!isParsed)
    {
        std::string errorMsg = "Error parsing file '" + filePath + "'";
        CheckEngineError(errorMsg.c_str());
        return false;
    }

    doc.Accept(writer);

    // Get the JSON string from the buffer
    std::string json = buf.GetString();

    // Write the JSON to a file
    std::ofstream of(filePath);
    of << json;

    if (!of.good()) {
        std::string errorMsg = "Error saving json to file '" + filePath + "'";
        CheckEngineError(errorMsg.c_str());
        return false;
    }

    // fclose(fp);

    printf("Saved scene!\n");
    return true;
}
