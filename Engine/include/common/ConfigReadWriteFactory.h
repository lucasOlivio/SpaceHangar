#pragma once

#include "iConfigReadWrite.h"

class ConfigReadWriteFactory
{
public:
	// Gets the right config reader and writer accordingly to the file type
	static iConfigReadWrite* CreateConfigReadWrite(std::string fileExt);
};