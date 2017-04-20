#include "OpenGL.h"

GLFWwindow* GLWindow = nullptr;

#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

void _check_gl_error(const char *file, int line)
{
	GLenum err(glGetError());

	while (err != GL_NO_ERROR)
	{
		char* error = "";

		switch (err)
		{
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}
		//printf("GL Error: %s - %s : %d\n", error, file, line);

		err = glGetError();
	}

}

void __stdcall oglCallbackMsgs(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
{
	printf("OpenGL Message Callback:\n");

	printf(message);
	printf("\n");
	printf("Type: ");
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		printf("ERROR");
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		printf("DEPRECATED_BEHAVIOR");
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		printf("UNDEFINED_BEHAVIOR");
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		printf("PORTABILITY");
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		printf("PERFORMANCE");
		break;
	case GL_DEBUG_TYPE_OTHER:
		printf("OTHER");
		break;
	}

	printf("\n");

	printf("Id: %d\n", id);
	printf("Severity : ");
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_LOW:
		printf("LOW");
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		printf("MEDIUM");
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		printf("HIGH");
		break;
	}

	printf("\n");
}

static void glfw_error_callback(int error, const char* description)
{
	printf("OpenGL Error - %d : %s \n", error, description);
}

void InitGL()
{
	printf("Initializing OpenGL. \n");

	glfwSetErrorCallback(glfw_error_callback);
	glfwInit();
}

void InitGLWindow(const char * title, int width, int height, bool resizable, bool fullscreen)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, resizable ? GL_TRUE : GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	
	GLWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
	glfwMakeContextCurrent(GLWindow);

	//vSync ON
	glfwSwapInterval(1);

	if (GLWindow == NULL)
	{
		printf("Failed to create GLFW window. \n");
		glfwTerminate();
	}
	else printf("Success creating GLFW window. \n");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("Failed to initalize GLEW. \n");
	}
	else printf("Success initializing GLEW. \n");

	int fbwidth, fbheight;
	glfwGetFramebufferSize(GLWindow, &fbwidth, &fbheight);
	glViewport(0, 0, width, height);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(oglCallbackMsgs, nullptr);
	glDebugMessageControl(
		GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true
	);

	const char* glversion = (char*)(glGetString(GL_VERSION));
	const char* glrenderer = (char*)(glGetString(GL_RENDERER));
	const char* glvendor = (char*)(glGetString(GL_VENDOR));

	printf("OpenGL Info: \n");
	printf("{ \n");
	printf("  GL Renderer: %s \n", glrenderer);
	printf("  GL Version: %s \n", glversion);
	printf("  GL Vendor: %s \n", glvendor);
	printf("} \n");
}

void ShutdownGL()
{
	glfwTerminate();
}

void ClearWindow(float r, float g, float b)
{
	glClearColor(r / 255, g / 255, b / 255, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RefreshWindow()
{
	check_gl_error();
	glfwSwapBuffers(GLWindow);
}

void CompileShader(Shader* shader)
{
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	const GLchar* vSrc = shader->VertexSource;
	const GLchar* fSrc = shader->FragmentSource;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vSrc, NULL);
	glCompileShader(vertex);
	CheckShaderCompileErrors(vertex, "VERTEX");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fSrc, NULL);
	glCompileShader(fragment);
	CheckShaderCompileErrors(fragment, "FRAGMENT");

	shader->Program = glCreateProgram();

	glAttachShader(shader->Program, vertex);
	glAttachShader(shader->Program, fragment);

	glLinkProgram(shader->Program);
	CheckShaderCompileErrors(shader->Program, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);

}

void UseShaderProgram(Shader* shader)
{
	glUseProgram(shader->Program);
}

void CheckShaderCompileErrors(GLuint shader, const char * type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			printf("Program Linking Error of type: %s\n%s", type, infoLog);
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			printf("Program Linking Error of type: %s\n%s", type, infoLog);
		}
	}
}

void CreateMesh(Mesh* mesh, glm::vec3* position, glm::vec2* uv, glm::vec3* normals, int vertexCount, unsigned int* indices, int indicesCount)
{
	glGenVertexArrays(1, &mesh->VAO);
	glGenBuffers(1, &mesh->VBO);
	glGenBuffers(1, &mesh->EBO);

	kvec_t(float) data;
	kv_init(data);
	int dataSize = 0;
	for (int i = 0; i < vertexCount; i++)
	{
		kv_push(float, data, position[i].x);
		kv_push(float, data, position[i].y);
		kv_push(float, data, position[i].z);

		kv_push(float, data, uv[i].x);
		kv_push(float, data, uv[i].y);

		kv_push(float, data, normals[i].x);
		kv_push(float, data, normals[i].y);
		kv_push(float, data, normals[i].z);
	}

	glBindVertexArray(mesh->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	glBufferData(GL_ARRAY_BUFFER, data.n * sizeof(float), &data.a[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	size_t stride = 3 * sizeof(float);
	stride += 2 * sizeof(float);
	stride += 3 * sizeof(float);
	
	size_t offset = 0;

	//positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += 3 * sizeof(float);

	//uv 
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += 2 * sizeof(float);

	//normals 
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += 3 * sizeof(float);

	glBindVertexArray(0);

	free(position);
	free(uv);
	free(normals);
	free(indices);

	kv_destroy(data);
}

void CreateSphereMesh(Mesh* mesh, unsigned int xSegments, unsigned int ySegments)
{
	kvec_t(glm::vec3) positions;
	kv_init(positions);

	kvec_t(glm::vec2) uv;
	kv_init(uv);

	kvec_t(glm::vec3) normals;
	kv_init(normals);

	kvec_t(unsigned int) indices;
	kv_init(indices);

	int times = 0;
	for (unsigned int y = 0; y <= ySegments; ++y)
	{
		for (unsigned int x = 0; x <= xSegments; ++x)
		{
			float xSegment = (float)x / (float)ySegments;
			float ySegment = (float)y / (float)ySegments;

			float xPos = std::cos(xSegment * TWOPI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * TWOPI) * std::sin(ySegment * PI);

			kv_push(glm::vec3, positions, glm::vec3(xPos, yPos, zPos));
			kv_push(glm::vec2, uv, glm::vec2(xSegment, ySegment));
			kv_push(glm::vec3, normals, glm::vec3(xPos, yPos, zPos));
			times++;
		}
	}

	bool oddRow = false;
	for (int y = 0; y < ySegments; ++y)
	{
		if (!oddRow)
		{
			for (int x = 0; x <= xSegments; ++x)
			{
				kv_push(unsigned int, indices, y       * (xSegments + 1) + x);
				kv_push(unsigned int, indices, (y + 1) * (xSegments + 1) + x);
			}
		}
		else
		{
			for (int x = xSegments; x >= 0; --x)
			{
				kv_push(unsigned int, indices, (y + 1) * (xSegments + 1) + x);
				kv_push(unsigned int, indices, y       * (xSegments + 1) + x);
			}
		}
		oddRow = !oddRow;
	}
	mesh->indicesSize = indices.n;
	mesh->vertexCount = times;
	CreateMesh(mesh, positions.a, uv.a, normals.a, times, indices.a, indices.n);
}

Texture CreateTexture(const char* path)
{
	Texture texture;

	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	texture.Width = width;
	texture.Height = height;

	glGenTextures(1, &texture.Id);
	glBindTexture(GL_TEXTURE_2D, texture.Id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}

void SetActiveTexture(Shader* shader, const Texture& texture, const char * location)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.Id);
	glUniform1i(glGetUniformLocation(shader->Program, location), 0);
}

void SetUniformMatrix4fv(Shader* shader, const glm::mat4 & matrix, const char * location)
{
	GLint loc = glGetUniformLocation(shader->Program, location);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void SetVertexArrayObject(unsigned int vao)
{
	glBindVertexArray(vao);
}

void ClearVertexArrayObject()
{
	glBindVertexArray(0);
}

void DrawElements(size_t indicesSize)
{
	glDrawElements(GL_TRIANGLE_STRIP, indicesSize, GL_UNSIGNED_INT, 0);
}
