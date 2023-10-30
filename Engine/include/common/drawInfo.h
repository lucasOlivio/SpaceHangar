#pragma once

#include "common/types.h"
#include <string>

struct sVertex
{
	float x, y, z, w;
	float r, g, b, a;
	float nx, ny, nz, nw;
};

struct sMesh
{
	std::string name;

	uint VAO_ID;

	uint vertexBufferID;
	uint vertexBuffer_Start_Index;
	uint numberOfVertices;

	uint indexBufferID;
	uint indexBuffer_Start_Index;
	uint numberOfIndices;
	uint numberOfTriangles;

	// The index buffer (CPU side)
	uint* pIndices;
	sVertex* pVertices;
};