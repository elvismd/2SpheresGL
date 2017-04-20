#pragma once

#include "Mesh.h"
#include "Texture.h"

struct Shader
{
	unsigned int Program;
	char* FragmentSource;
	char* VertexSource;

	void Clear()
	{
		if (FragmentSource != NULL)
		{
			free(FragmentSource);
			FragmentSource = NULL;
		}
		if (VertexSource != NULL)
		{
			free(VertexSource);
			VertexSource = NULL;
		}
	}
};

struct Material
{
	Shader* MShader = NULL;
	Mesh* MMesh = NULL;
	Texture MTexture;

	void Clear()
	{
		if (MShader != NULL)
		{
			MShader->Clear();
			free(MShader);
			MShader = NULL;
		}
		if (sizeof(MMesh) == sizeof(Mesh))
		{
			free(MMesh);
			MMesh = NULL;
		}
	}
};