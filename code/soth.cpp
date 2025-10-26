#include "soth.h"
#include "shader.h"
#include <math.h>

void GameInputController::CopyState(GameInputController* Input)
{
	if (!Input) return;

	for (int KeyIndex = 0; KeyIndex < ARRAY_COUNT(Input->Keys); KeyIndex++)
	{
		Keys[KeyIndex].State = Input->Keys[KeyIndex].State;
	}
}

void LoadShaders(GameState* State, OpenGL* OpenGL)
{
	if (!State) return;

	const char* _VertexShaderPath = "R:\\resources\\shader.vs";
	const char* _FragmentShaderPath = "R:\\resources\\shader.fs";

	ReadFileResult _VertexShaderFile = Win32_ReadFile(_VertexShaderPath);
	ReadFileResult _FragmentShaderFile = Win32_ReadFile(_FragmentShaderPath);

	unsigned int _shaderProgram;
	Shader::CreateShader(_VertexShaderFile.Content, _FragmentShaderFile.Content, _shaderProgram, OpenGL);

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

	//OpenGL::GenVertexArrays(1, &_VAO);
	OpenGL->GenVertexArrays(1, &_VAO);
	OpenGL->GenBuffers(1, &_VBO);
	OpenGL->BindVertexArray(_VAO);

	OpenGL->BindBuffer(GL_ARRAY_BUFFER, _VBO);
	OpenGL->BufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

	OpenGL->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	OpenGL->EnableVertexAttribArray(0);

	OpenGL->BindBuffer(GL_ARRAY_BUFFER, 0);
	OpenGL->BindVertexArray(0);

	State->VBO           = (uint16)_VBO;
	State->VAO           = (uint16)_VAO;
	State->ShaderProgram = (uint16)_shaderProgram;
}

void UpdateGame(GameMemory* Memory, GameInputController* Input)
{
	ASSERT(sizeof(GameState) <= Memory->PermanentStorageSize);

	GameState* _GameState = (GameState*)Memory->PermanentStorage;

	if (!Memory->IsInitialized)
	{
		LoadShaders(_GameState, Memory->OpenGL);

		_GameState->PlayerPos = MatrixS4(1.f);

		//MatrixS4 _M(1.0f);
		//_M.Scale(2.0f);
		//MatrixS4 _T(1.f);
		//_M.Transpose();
		//DebugLogMatrixS4(_M);

		Memory->IsInitialized = true;
	}



	// Parse inputs
	//
	if (Input->MoveLeft.State == DOWN)
	{
		_GameState->PlayerPos.MoveX(-1.f);
	}
	if (Input->MoveRight.State == DOWN)
	{
		_GameState->PlayerPos.MoveX(1.f);
	}

	MatrixS4 _Model(1.0f);
	_Model.Scale(100.0f);
	MatrixS4 _Projection = GetOrthographicProjectionMatrix(-(real32)g_ClientSize.Width/2, (real32)g_ClientSize.Width/2, -(real32)g_ClientSize.Height/2, (real32)g_ClientSize.Height/2, 0.0f, 100.0f);
	

	Memory->OpenGL->UseProgram(_GameState->ShaderProgram);
	Shader::SetUniform_MatrixS4(_GameState->ShaderProgram, "uni_projection" , _Projection/*.Transpose()*/.GetPointer(), Memory->OpenGL);
	Shader::SetUniform_MatrixS4(_GameState->ShaderProgram, "uni_view"       , _GameState->PlayerPos      /*.Transpose()*/.GetPointer(), Memory->OpenGL);
	Shader::SetUniform_MatrixS4(_GameState->ShaderProgram, "uni_model"      , _Model     /*.Transpose()*/.GetPointer(), Memory->OpenGL);
	Memory->OpenGL->BindVertexArray(_GameState->VAO);
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