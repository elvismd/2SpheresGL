#pragma once

#include "Include.h"

struct Mesh
{
	unsigned int VAO = 0;
	unsigned int VBO;
	unsigned int EBO;
	int vertexCount;
	size_t indicesSize;
};