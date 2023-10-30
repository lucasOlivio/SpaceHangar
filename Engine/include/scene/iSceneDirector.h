#pragma once

#include "components/iComponent.h"
#include <string>

class iSceneDirector
{
public:
	virtual ~iSceneDirector() {};

	virtual void SetRunning(bool isRunning) = 0;
	virtual void ChangeMode() = 0;

	virtual void SaveScene() = 0;
	virtual void SaveScene(std::string filePath) = 0;

	virtual void LoadScene() = 0;
	virtual void LoadScene(std::string filePath) = 0;
};
