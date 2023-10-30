#pragma once

#include "scene/iScene.h"
#include <string>
#include <rapidjson/document.h>

class SceneParserJSON
{
public:
	SceneParserJSON();
	~SceneParserJSON();

	bool ParseFromJsonObj(rapidjson::Value& jsonObject, iScene* pSceneOut);
	bool ParseToJsonObj(iScene* pScene,
						rapidjson::Value& jsonObjectOut,
						rapidjson::Document::AllocatorType& allocator);
};