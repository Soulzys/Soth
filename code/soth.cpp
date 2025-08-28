#include "soth.h"
#include "shader.h"
#include <math.h>

void LoadShaders(GameState* State)
{
	if (!State) return;

	const char* _VertexShaderPath = "R:\\resources\\shader.vs";
	const char* _FragmentShaderPath = "R:\\resources\\shader.fs";

	ReadFileResult _VertexShaderFile = Win32_ReadFile(_VertexShaderPath);
	ReadFileResult _FragmentShaderFile = Win32_ReadFile(_FragmentShaderPath);

	unsigned int _shaderProgram;
	Shader::CreateShader(_VertexShaderFile.Content, _FragmentShaderFile.Content, _shaderProgram);

	// TODO: need to handle this
	//Util_FreeFileMemory(_VertexShaderFile.Content);
	//Util_FreeFileMemory(_FragmentShaderFile.Content);


	float _vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};


	unsigned int _VBO;
	unsigned int _VAO;

	OpenGL::GenVertexArrays(1, &_VAO);
	OpenGL::GenBuffers(1, &_VBO);
	OpenGL::BindVertexArray(_VAO);

	OpenGL::BindBuffer(GL_ARRAY_BUFFER, _VBO);
	OpenGL::BufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

	OpenGL::VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	OpenGL::EnableVertexAttribArray(0);

	OpenGL::BindBuffer(GL_ARRAY_BUFFER, 0);
	OpenGL::BindVertexArray(0);

	State->VBO           = (uint16)_VBO;
	State->VAO           = (uint16)_VAO;
	State->ShaderProgram = (uint16)_shaderProgram;
}

void UpdateGame(GameMemory* Memory)
{
	ASSERT(sizeof(GameState) <= Memory->PermanentStorageSize);

	GameState* _GameState = (GameState*)Memory->PermanentStorage;

	if (!Memory->IsInitialized)
	{
		LoadShaders(_GameState);
		Memory->IsInitialized = true;

		MatrixS4 _M(1.0f);
		//_M[0][0] = 1; _M[0][1] = 2; _M[0][2] = 4; _M[0][3] = 1;
		//_M[1][0] = 3; _M[1][1] = 1; _M[1][2] = 5; _M[1][3] = 1;
		//_M[2][0] = 2; _M[2][1] = 2; _M[2][2] = 1; _M[2][3] = 1;
		//_M[3][0] = 6; _M[3][1] = 5; _M[3][2] = 3; _M[3][3] = 1;

		// Translation
		//_M[0][0] = 1; _M[0][1] = 0; _M[0][2] = 0; _M[0][3] = 0;
		//_M[1][0] = 0; _M[1][1] = 1; _M[1][2] = 0; _M[1][3] = 0;
		//_M[2][0] = 0; _M[2][1] = 0; _M[2][2] = 1; _M[2][3] = 0;
		//_M[3][0] = 2; _M[3][1] = 3; _M[3][2] = 0; _M[3][3] = 1;

		// Rotate X
		_M[0][0] = (real32) cos(90); _M[0][1] = (real32)sin(90); _M[0][2] = 0; _M[0][3] = 0;
		_M[1][0] = (real32)-sin(90); _M[1][1] = (real32)cos(90); _M[1][2] = 0; _M[1][3] = 0;
		_M[2][0] = 0               ; _M[2][1] = 0              ; _M[2][2] = 1; _M[2][3] = 0;
		_M[3][0] = 0               ; _M[3][1] = 0              ; _M[3][2] = 0; _M[3][3] = 1;
		
		//_M.Translate(Vec3(2.0f, 1.0f, 3.0f));
		//_M.Scale(Vec3(2.0f, 1.0f, 3.0f));
		//_M.Scale(2.0f);

		Vec3 _V = Vec3(0.0f, 1.0f, 0.0f);
		_V = _V * _M;
		DebugLogVector(_V, 1.0f, 3);
		//DebugLogMatrixS4(_M);
	}


	OpenGL::UseProgram(_GameState->ShaderProgram);
	OpenGL::BindVertexArray(_GameState->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void ExitGame()
{
	// NOTE: do we even need this since we're exiting the game anyway ? 
	//
	//OpenGL::DeleteVertexArrays(1, &_VAO);
	//OpenGL::DeleteBuffers(1, &_VBO);
	//OpenGL::DeleteProgram(_shaderProgramStub);
}