#pragma once

#include "iShaderInfo.h"
#include "common/types.h"
#include <string>
#include <vector>
#include <map>

class ShaderManager : public iShaderInfo
{
public:
	class Shader {
	public:
		Shader();
		~Shader();

		uint ID;
		std::vector<std::string> vecSource;
		bool isSourceMultiLine;
		std::string fileName;

		enum ShaderType
		{
			VERTEX_SHADER,
			FRAGMENT_SHADER,
			UNKNOWN
		};
		ShaderType shaderType;

		std::string GetShaderTypeString(void);
	};

	class ShaderProgram {
	private:
		std::map< std::string /*name of uniform variable*/,
			int /* uniform location ID */ >
			m_mapUniformNameToUniformLocation;
		std::map< std::string /*name of attribute variable*/,
			int /* attribute location ID */ >
			m_mapAttributeNameToAttributeLocation;
	public:
		ShaderProgram();
		~ShaderProgram();
		uint ID;	// ID from OpenGL
		std::string name;	// We give it this name

		// Returns -1 (just like OpenGL) if NOT found
		int GetUniformIDFromName(const char* name);
		// Returns -1 (just like OpenGL) if NOT found
		int GetAttributeIDFromName(const char* name);
		// Look up the uniform location and save it.
		bool LoadUniformLocation(const char* variableName);
		// Look up the attribute location and save it.
		bool LoadAttributeLocation(const char* variableName);

	};

	ShaderManager(std::string basePath);
	~ShaderManager();

	// Where we initialize all things that could go wrong
	bool AddShaderProgram(std::string shaderProgram);

	bool CreateProgramFromFile(std::string shaderProgram,
							   Shader& vertexShad,
							   Shader& fragShader);
	void SetBasePath(std::string basepath);

	// Clears last error
	std::string GetLastError(void);

	uint GetIDFromShaderProgramName(std::string& shaderProgram);

	// Used to load the uniforms. Returns NULL if not found.
	ShaderProgram* GetShaderProgramFromName(std::string& shaderProgram);
	ShaderProgram* GetShaderProgramFromID(uint shaderProgramID);

	// Use the shader program name to get the UL from cache or load from opengl
	virtual int GetUL(std::string& shaderProgram, const char* ulName);
	// Use the shader program name to get the AL from cache or load from opengl
	virtual int GetAL(std::string& shaderProgram, const char* alName);
	// Use the shader program id to get the UL from cache or load from opengl
	virtual int GetUL(int shaderID, const char* ulName);
	// Use the shader program id to get the AL from cache or load from opengl
	virtual int GetAL(int shaderID, const char* alName);

	// Bind opengl to this shader program
	virtual bool UseShaderProgram(uint ID);
	virtual bool UseShaderProgram(std::string& shaderProgram);

private:
	// Returns an empty string if it didn't work
	bool m_LoadSourceFromFile(Shader& shader, std::string& errorText);
	std::string m_basepath;

	bool m_CompileShaderFromSource(Shader& shader, std::string& error);
	// returns false if no error
	bool m_WasThereACompileError(uint shaderID, std::string& errorText);
	bool m_WasThereALinkError(uint progID, std::string& errorText);

	std::string m_lastError;

	std::map< uint /*ID*/, ShaderProgram > m_IDToShaderProgram;
	std::map< std::string /*name*/, uint /*ID*/ > m_shaderProgramNameToID;
};