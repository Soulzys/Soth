#include "soth.h"
#include "shader.h"

static unsigned int _VBO;
static unsigned int _VAO;
static unsigned int _shaderProgramStub;

void LoadShaders()
{
	const char* _VertexShaderPath = "R:\\resources\\shader.vs";
	const char* _FragmentShaderPath = "R:\\resources\\shader.fs";

	ReadFileResult _VertexShaderFile = Win32_ReadFile(_VertexShaderPath);
	ReadFileResult _FragmentShaderFile = Win32_ReadFile(_FragmentShaderPath);

	//unsigned int _shaderProgramStub;

	Shader::CreateShader(_VertexShaderFile.Content, _FragmentShaderFile.Content, _shaderProgramStub);

	// TODO: need to handle this
	//Util_FreeFileMemory(_VertexShaderFile.Content);
	//Util_FreeFileMemory(_FragmentShaderFile.Content);


	float _vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};


	//unsigned int _VBO;
	//unsigned int _VAO;

	OpenGL::GenVertexArrays(1, &_VAO);
	OpenGL::GenBuffers(1, &_VBO);
	OpenGL::BindVertexArray(_VAO);

	OpenGL::BindBuffer(GL_ARRAY_BUFFER, _VBO);
	OpenGL::BufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

	OpenGL::VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	OpenGL::EnableVertexAttribArray(0);

	OpenGL::BindBuffer(GL_ARRAY_BUFFER, 0);
	OpenGL::BindVertexArray(0);
}

void UpdateGame()
{
	OpenGL::UseProgram(_shaderProgramStub);
	OpenGL::BindVertexArray(_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void ExitGame()
{
	OpenGL::DeleteVertexArrays(1, &_VAO);
	OpenGL::DeleteBuffers(1, &_VBO);
	OpenGL::DeleteProgram(_shaderProgramStub);
}