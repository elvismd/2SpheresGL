#pragma once

#include "Include.h"
#include "Shading.h"
#include "Texture.h"

void InitGL();
void InitGLWindow(const char* title, int width, int height, bool resizable, bool fullscreen);
void ShutdownGL();
void ClearWindow(float r = 0.05f, float g = 0.05f, float b = 0.05f);
void RefreshWindow();

void CompileShader(Shader* shader);
void UseShaderProgram(Shader* shader);
void CheckShaderCompileErrors(GLuint shader, const char* type);

void CreateMesh(Mesh* mesh, glm::vec3* position, glm::vec2* uv, glm::vec3* normals, int vertexCount, unsigned int* indices, int indicesCount);
void CreateSphereMesh(Mesh* mesh, unsigned int xSegments, unsigned int ySegments);
Texture CreateTexture(const char* path);

void SetActiveTexture(Shader* shader, const Texture& texture, const char * location);
void SetUniformMatrix4fv(Shader* shader, const glm::mat4& matrix, const char* location);

void SetVertexArrayObject(unsigned int vao);
void ClearVertexArrayObject();
void DrawElements(size_t indicesSize);

extern GLFWwindow* GLWindow;