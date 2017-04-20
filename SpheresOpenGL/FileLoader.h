#pragma once

#include "Shading.h"

#include <fstream>
#include <sstream>

const char* GetFileBuffer(const char* path)
{
	FILE    *file;
	char    *buffer;
	long    numbytes;

	file = fopen(path, "r");

	if (file == NULL) return nullptr;

	fseek(file, 0L, SEEK_END);
	numbytes = ftell(file);

	fseek(file, 0L, SEEK_SET);

	buffer = (char*)calloc(numbytes, sizeof(char));

	if (buffer == NULL) return nullptr;

	/* copy all the text into the buffer */
	fread(buffer, sizeof(char), numbytes, file);
	fclose(file);

	return buffer;
}

Shader* LoadShader(const char* vertexPath, const char* fragmentPath)
{
	Shader* shader;
	shader = (Shader*)malloc(sizeof(Shader));

	shader->VertexSource = (char*)GetFileBuffer(vertexPath);;
	shader->FragmentSource = (char*)GetFileBuffer(fragmentPath);
	
	return shader;
}