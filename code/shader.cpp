#pragma once

#include "shader.h"
#include "soth.h"

bool Shader::CreateShader(void* VertexShaderCode, void* FragmentShaderCode, unsigned int& ShaderId, GameMemory* Memory)
{
	if (!VertexShaderCode || !FragmentShaderCode) return false;

	int _HasSucceeded;
	const int16 _LogsSize = 512;
	char _Logs[_LogsSize];

	unsigned int          _VertexShader = Memory->OpenGL->CreateShader(GL_VERTEX_SHADER);
	Memory->OpenGL->ShaderSource (_VertexShader, 1, &(const char*)VertexShaderCode, nullptr);
	Memory->OpenGL->CompileShader(_VertexShader);
	Memory->OpenGL->GetShaderiv  (_VertexShader, GL_COMPILE_STATUS, &_HasSucceeded);
	if (!_HasSucceeded)
	{
		Memory->OpenGL->GetShaderInfoLog(_VertexShader, _LogsSize, nullptr, _Logs);
		Memory->DebugPlatformLogMessage("ERROR: -------- SHADER:VERTEX:COMPILATION_FAILED\n");
		Memory->DebugPlatformLogMessage(_Logs);
	}

	unsigned int          _FragmentShader = Memory->OpenGL->CreateShader(GL_FRAGMENT_SHADER);
	Memory->OpenGL->ShaderSource (_FragmentShader, 1, &(const char*)FragmentShaderCode, nullptr);
	Memory->OpenGL->CompileShader(_FragmentShader);
	Memory->OpenGL->GetShaderiv  (_FragmentShader, GL_COMPILE_STATUS, &_HasSucceeded);
	if (!_HasSucceeded)
	{
		Memory->OpenGL->GetShaderInfoLog(_FragmentShader, _LogsSize, nullptr, _Logs);
		Memory->DebugPlatformLogMessage("ERROR: -------- SHADER:FRAGMENT:COMPILATION_FAILED\n");
		Memory->DebugPlatformLogMessage(_Logs);
	}

	ShaderId = Memory->OpenGL->CreateProgram();
	Memory->OpenGL->AttachShader(ShaderId, _VertexShader);
	Memory->OpenGL->AttachShader(ShaderId, _FragmentShader);
	Memory->OpenGL->LinkProgram (ShaderId);
	Memory->OpenGL->GetShaderiv (ShaderId, GL_LINK_STATUS, &_HasSucceeded);
	if (!_HasSucceeded)
	{
		Memory->OpenGL->GetShaderInfoLog(ShaderId, _LogsSize, nullptr, _Logs);
		Memory->DebugPlatformLogMessage("ERROR: -------- SHADER:PROGRAM:LINKING_FAILED\n");
		Memory->DebugPlatformLogMessage(_Logs);
	}

	Memory->OpenGL->DeleteShader(_VertexShader);
	Memory->OpenGL->DeleteShader(_FragmentShader);

	return true;
}

void Shader::SetUniform_MatrixS4(int32 ShaderId, const char* Name, real32* Matrix, OpenGL* OpenGL)
{
	OpenGL->UniformMatrix4fv(OpenGL->GetUniformLocation(ShaderId, Name), 1, GL_FALSE, Matrix);
}