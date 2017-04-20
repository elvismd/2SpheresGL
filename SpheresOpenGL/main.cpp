#include "OpenGL.h"
#include "Input.h"
#include "FileLoader.h"
#include "Camera.h"

Camera camera(glm::vec3(0.0f, 2.5f, 18.0f));
int screenWidth = 800, screenHeight = 600;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
void camMovement()
{
	if (keys[GLFW_KEY_W]) camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S]) camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A]) camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D]) camera.ProcessKeyboard(RIGHT, deltaTime);
}

int main(int argc, char* argv[])
{
	InitGL();
	InitGLWindow("Spheres", screenWidth, screenHeight, false, false);

	InitInput();

	LoadShader("Data/Vertex.vert", "Data/Frag.frag");

	Material* material;
	material = (Material*)malloc(sizeof(Material));
	material->MShader = LoadShader("Data/Vertex.vert", "Data/Frag.frag");
	CompileShader(material->MShader);

	material->MMesh = (Mesh*)malloc(sizeof(Mesh));
	CreateSphereMesh(material->MMesh, 20, 20);

	while(!glfwWindowShouldClose(GLWindow))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		UpdateInput();
		camMovement();

		ClearWindow(41, 41, 61);

		//object 1
		{
			UseShaderProgram(material->MShader);
			glm::mat4 view;
			view = camera.GetViewMatrix();
			glm::mat4 projection;
			projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);

			SetUniformMatrix4fv(material->MShader, view, "view");
			SetUniformMatrix4fv(material->MShader, projection, "projection");
			glm::mat4 model;
			float x = -5 * cos((GLfloat)glfwGetTime() * 2);
			float z = -8 * sin((GLfloat)glfwGetTime() * 2);
			model = glm::translate(glm::mat4(1.0), glm::vec3(x, 0.0f, z));

			SetUniformMatrix4fv(material->MShader, model, "model");
			SetVertexArrayObject(material->MMesh->VAO);
			DrawElements(material->MMesh->indicesSize);
			ClearVertexArrayObject();
		}

		//object 2
		{
			UseShaderProgram(material->MShader);
			glm::mat4 view;
			view = camera.GetViewMatrix();
			glm::mat4 projection;
			projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);

			SetUniformMatrix4fv(material->MShader, view, "view");
			SetUniformMatrix4fv(material->MShader, projection, "projection");
			glm::mat4 model;
			float x = 5 * cos((GLfloat)glfwGetTime() * 2);
			float z = 8 * sin((GLfloat)glfwGetTime() * 2);
			model = glm::translate(glm::mat4(1.0), glm::vec3(x, 0.0f, z));

			SetUniformMatrix4fv(material->MShader, model, "model");
			SetVertexArrayObject(material->MMesh->VAO);
			DrawElements(material->MMesh->indicesSize);
			ClearVertexArrayObject();
		}

		RefreshWindow();
	}

	material->Clear();
	free(material);

	ShutdownGL();

	return 0;
}