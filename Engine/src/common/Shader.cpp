#include "EngineRenderer/ShaderManager.h"
#include "common/opengl.h"

ShaderManager::Shader::Shader()
{
	this->ID = 0;
	this->shaderType = Shader::UNKNOWN;
	return;
}

ShaderManager::Shader::~Shader()
{
	return;
}

std::string ShaderManager::Shader::GetShaderTypeString(void)
{
	switch (this->shaderType)
	{
	case Shader::VERTEX_SHADER:
		return "VERTEX_SHADER";
		break;
	case Shader::FRAGMENT_SHADER:
		return "FRAGMENT_SHADER";
		break;
	case Shader::UNKNOWN:
	default:
		return "UNKNOWN_SHADER_TYPE";
		break;
	}
	// Should never reach here...
	return "UNKNOWN_SHADER_TYPE";
}

// Look up the uniform inside the shader, then save it, if it finds it
bool ShaderManager::ShaderProgram::LoadUniformLocation(const char* variableName)
{
	// 
	GLint uniLocation = glGetUniformLocation(this->ID, variableName);
	// Did it find it (not -1)
	if (uniLocation == -1)
	{	// Nope.
		return false;
	}
	// Save it
	this->m_mapUniformNameToUniformLocation[variableName] = uniLocation;

	return true;
}

bool ShaderManager::ShaderProgram::LoadAttributeLocation(const char* variableName)
{
	// 
	GLint attrLocation = glGetAttribLocation(this->ID, variableName);
	// Did it find it (not -1)
	if (attrLocation == -1)
	{	// Nope.
		return false;
	}
	// Save it
	this->m_mapAttributeNameToAttributeLocation[variableName] = attrLocation;

	return true;
}

ShaderManager::ShaderProgram::ShaderProgram()
{
}

ShaderManager::ShaderProgram::~ShaderProgram()
{
}

// Look up the uniform location and save it.
int ShaderManager::ShaderProgram::GetUniformIDFromName(const char* name)
{
	std::map< std::string /*name of uniform variable*/,
		int /* uniform location ID */ >::iterator
		itUniform = this->m_mapUniformNameToUniformLocation.find(name);
	if (itUniform == this->m_mapUniformNameToUniformLocation.end())
	{
		// Not in map yet, so load to map cache
		bool ulFound = this->LoadUniformLocation(name);		
		if (!ulFound)
		{
			// OpenGL uniform not found value
			return -1; 
		}

		itUniform = this->m_mapUniformNameToUniformLocation.find(name);
	}

	return itUniform->second;		// second if the "int" value
}

int ShaderManager::ShaderProgram::GetAttributeIDFromName(const char* name)
{
	std::map< std::string /*name of attribute variable*/,
		int /* attribute location ID */ >::iterator
		itAttribute = this->m_mapAttributeNameToAttributeLocation.find(name);
	if (itAttribute == this->m_mapAttributeNameToAttributeLocation.end())
	{
		// Not in map yet, so load to map cache
		bool ulFound = this->LoadAttributeLocation(name);
		if (!ulFound)
		{
			// OpenGL attribute not found value
			return -1;
		}

		itAttribute = this->m_mapAttributeNameToAttributeLocation.find(name);
	}

	return itAttribute->second;		// second if the "int" value
}
