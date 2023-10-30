#pragma once

#include "common/drawinfo.h"
#include "iShaderInfo.h"
#include <string>
#include <map>

class VAOManager
{
private:
	iShaderInfo* m_pShaderInfo;

	std::string m_basePath;
	std::map< std::string    /*model name*/,
		      sMesh* /* info needed to draw*/ >
		      m_mapModelNameToMesh;

	bool m_LoadTheFile_Ply_XYZ_N_RGBA(std::string theFileName, sMesh* drawInfo);
public:
	VAOManager(iShaderInfo* pShaderInfo);
	~VAOManager();

	void DestroyVBO(std::string modelName);

	sMesh* LoadModelIntoVAO(std::string& fileName, int shaderID, bool bIsDynamicBuffer);

	bool UpdateVAOBuffers(std::string& fileName,
						  std::string& shaderProgram,
						  sMesh* pUpdatedMesh);

	sMesh* FindMeshByModelName(std::string& filename);

	void SetBasePath(std::string basePath);
};

