#include "EngineRenderer/VAOManager.h"
#include "common/opengl.h"
#include "common/utils.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <sstream>
#include <fstream>

VAOManager::VAOManager(iShaderInfo* pShaderInfo)
{
    this->m_pShaderInfo = pShaderInfo;
}

VAOManager::~VAOManager()
{
}

void VAOManager::DestroyVBO(std::string modelName)
{
    

    sMesh* pMesh = this->FindMeshByModelName(modelName);
    if (!pMesh)
    {
        //Model not loaded
        return;
    }
    glDeleteBuffers(1, &(pMesh->vertexBufferID));
    delete pMesh;
    this->m_mapModelNameToMesh.erase(modelName);

    return;
}

void VAOManager::SetBasePath(std::string basePath)
{
    this->m_basePath = basePath;
    return;
}

sMesh* VAOManager::LoadModelIntoVAO(std::string& fileName,
                                  int shaderID,
                                  bool bIsDynamicBuffer /*=false*/)

{
    

    // Load the model from file
    // Checking if the info already exist in our map
    sMesh* pMesh = this->FindMeshByModelName(fileName);
    if (pMesh)
    {
        // Info is already loaded
        return pMesh;
    }
    pMesh = new sMesh();
    pMesh->name = fileName;

    std::string fileAndPath = this->m_basePath + fileName;

    if (!this->m_LoadTheFile_Ply_XYZ_N_RGBA(fileAndPath, pMesh))
    {
        return nullptr;
    }

    // Create a VAO (Vertex Array Object), which will 
    //	keep track of all the 'state' needed to draw 
    //	from this buffer...

    // Ask OpenGL for a new buffer ID...
    glGenVertexArrays(1, &(pMesh->VAO_ID));
    // "Bind" this buffer:
    // - aka "make this the 'current' VAO buffer
    glBindVertexArray(pMesh->VAO_ID);

    // Now ANY state that is related to vertex or index buffer
    //	and vertex attribute layout, is stored in the 'state' 
    //	of the VAO... 
    glGenBuffers(1, &(pMesh->vertexBufferID));

    glBindBuffer(GL_ARRAY_BUFFER, pMesh->vertexBufferID);

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(sVertex) * pMesh->numberOfVertices, 
                 (GLvoid*)pMesh->pVertices,
                 (bIsDynamicBuffer ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));

    // Copy the index buffer into the video card, 
    // too create an index buffer.
    glGenBuffers(1, &(pMesh->indexBufferID));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->indexBufferID);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(uint) * pMesh->numberOfIndices,
                 (GLvoid*)pMesh->pIndices,
                 GL_STATIC_DRAW);

    // Set the vertex attributes.
    GLint vpos_location = this->m_pShaderInfo->GetAL(shaderID, "vPos");
    GLint vcol_location = this->m_pShaderInfo->GetAL(shaderID, "vCol");
    GLint vNormal_location = this->m_pShaderInfo->GetAL(shaderID, "vNormal");

    // Set the vertex attributes for this shader
    // vPos
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 4,	
                          GL_FLOAT, GL_FALSE,
                          sizeof(sVertex),
                          (void*)offsetof(sVertex, x));
    // vCol 
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 4,	
                          GL_FLOAT, GL_FALSE,
                          sizeof(sVertex),
                          (void*)offsetof(sVertex, r));

    glEnableVertexAttribArray(vNormal_location);
    glVertexAttribPointer(vNormal_location, 4,	
                          GL_FLOAT, GL_FALSE,
                          sizeof(sVertex),
                          (void*)offsetof(sVertex, nx));

    // Now that all the parts are set up, set the VAO to zero
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(vpos_location);
    glDisableVertexAttribArray(vcol_location);
    glDisableVertexAttribArray(vNormal_location);

    // Store the draw information into the map
    this->m_mapModelNameToMesh[pMesh->name] = pMesh;

    return pMesh;
}


// We don't want to return an int, likely
sMesh* VAOManager::FindMeshByModelName(std::string& filename)
{
    

    std::map< std::string /*model name*/,
        sMesh* /* info needed to draw*/ >::iterator
        itDrawInfo = this->m_mapModelNameToMesh.find(filename);

    // Find it? 
    if (itDrawInfo == this->m_mapModelNameToMesh.end())
    {
        // Nope
        return nullptr;
    }

    return itDrawInfo->second;
}

bool VAOManager::m_LoadTheFile_Ply_XYZ_N_RGBA(std::string theFileName, sMesh* pMesh)
{
    std::ifstream modelFile(theFileName.c_str());
    if (!modelFile.is_open())
    {
        std::string errorMsg = "Mesh file '" + theFileName + "' not found!";
        CheckEngineError(errorMsg.c_str());
        return false;
    }

    std::string temp;
    while (modelFile >> temp)
    {
        if (temp == "vertex")
        {
            break;
        }
    };
    modelFile >> pMesh->numberOfVertices;

    while (modelFile >> temp)
    {
        if (temp == "face")
        {
            break;
        }
    };
    modelFile >> pMesh->numberOfTriangles;

    pMesh->numberOfIndices = pMesh->numberOfTriangles * 3;

    while (modelFile >> temp)
    {
        if (temp == "end_header")
        {
            break;
        }
    };

    // This most closely matches the ply file for the ply files
    struct sVertexPlyFile
    {
        float x;
        float y;
        float z;
        float nx, ny, nz;
        float r, g, b, a;
    };

    struct sTrianglePlyFile
    {
        // Vertices of the triangles
        uint v0, v1, v2;
    };

    sVertexPlyFile* pTheVerticesFile = new sVertexPlyFile[pMesh->numberOfVertices];

    for (uint index = 0; index != pMesh->numberOfVertices; index++)
    {
        sVertexPlyFile tempVertex;
        modelFile >> tempVertex.x;                //std::cin >> a.x;
        modelFile >> tempVertex.y;                //std::cin >> a.y;
        modelFile >> tempVertex.z;                //std::cin >> a.z;

        modelFile >> tempVertex.nx;
        modelFile >> tempVertex.ny;
        modelFile >> tempVertex.nz;

        modelFile >> tempVertex.r;       tempVertex.r /= 255.0f;
        modelFile >> tempVertex.g;       tempVertex.g /= 255.0f;
        modelFile >> tempVertex.b;       tempVertex.b /= 255.0f;
        modelFile >> tempVertex.a;       tempVertex.a /= 255.0f;

        pTheVerticesFile[index] = tempVertex;
    }

    sTrianglePlyFile* pTheTriangles = new sTrianglePlyFile[pMesh->numberOfTriangles];

    // Loading triangles faces, by getting the index number of each vertex
    for (uint index = 0; index != pMesh->numberOfTriangles; index++)
    {
        sTrianglePlyFile tempTriangle;

        uint discard;
        modelFile >> discard;            // 3
        modelFile >> tempTriangle.v0;                //std::cin >> a.x;
        modelFile >> tempTriangle.v1;                //std::cin >> a.y;
        modelFile >> tempTriangle.v2;                //std::cin >> a.z;

        pTheTriangles[index] = tempTriangle;
    }

    pMesh->pVertices = new sVertex[pMesh->numberOfVertices];
    for (uint vertIndex = 0; vertIndex != pMesh->numberOfVertices; vertIndex++)
    {
        // Loading coordinates
        pMesh->pVertices[vertIndex].x = pTheVerticesFile[vertIndex].x;
        pMesh->pVertices[vertIndex].y = pTheVerticesFile[vertIndex].y;
        pMesh->pVertices[vertIndex].z = pTheVerticesFile[vertIndex].z;
        pMesh->pVertices[vertIndex].w = 1.0f;

        // Loading normals
        pMesh->pVertices[vertIndex].nx = pTheVerticesFile[vertIndex].nx;
        pMesh->pVertices[vertIndex].ny = pTheVerticesFile[vertIndex].ny;
        pMesh->pVertices[vertIndex].nz = pTheVerticesFile[vertIndex].nz;
        pMesh->pVertices[vertIndex].nw = 1.0f;

        // Loading vertex colors
        pMesh->pVertices[vertIndex].r = pTheVerticesFile[vertIndex].r;
        pMesh->pVertices[vertIndex].g = pTheVerticesFile[vertIndex].g;
        pMesh->pVertices[vertIndex].b = pTheVerticesFile[vertIndex].b;
        pMesh->pVertices[vertIndex].a = pTheVerticesFile[vertIndex].a;
    }

    // Allocate an array for all the indices (which is 3x the number of triangles)
    // Element array is an 1D array of integers
    pMesh->pIndices = new uint[pMesh->numberOfIndices];

    uint elementIndex = 0;
    for (uint triIndex = 0; triIndex != pMesh->numberOfTriangles; triIndex++)
    {
        pMesh->pIndices[elementIndex + 0] = pTheTriangles[triIndex].v0;
        pMesh->pIndices[elementIndex + 1] = pTheTriangles[triIndex].v1;
        pMesh->pIndices[elementIndex + 2] = pTheTriangles[triIndex].v2;

        elementIndex += 3;      // Next "triangle"
    }

    return true;
}

bool VAOManager::UpdateVAOBuffers(std::string& fileName,
                                  std::string& shaderProgram,
                                  sMesh* pUpdatedMesh)
{
    

    // This exists? 
    if (!this->FindMeshByModelName(fileName))
    {
        // Mesh not loaded
        return false;
    }

    // Set VBO
    glBindBuffer(GL_ARRAY_BUFFER, pUpdatedMesh->vertexBufferID);
    // Update data in buffer
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,  // Offset
                    sizeof(sVertex) * pUpdatedMesh->numberOfVertices,
                    (GLvoid*)pUpdatedMesh->pVertices);
    // Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Set index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pUpdatedMesh->indexBufferID);
    // Update data in buffer
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
                    0,  // Offset
                    sizeof(uint) * pUpdatedMesh->numberOfIndices,
                    (GLvoid*)pUpdatedMesh->pIndices);
    // Unbind index buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}
