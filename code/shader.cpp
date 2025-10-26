#include "shader.h"
#include "soth.h"

bool Shader::CreateShader(void* VertexShaderCode, void* FragmentShaderCode, unsigned int& ShaderId, OpenGLController* OpenGLController)
{
	if (!VertexShaderCode || !FragmentShaderCode) return false;

	int _HasSucceeded;
	const int16 _LogsSize = 512;
	char _Logs[_LogsSize];

	unsigned int          _VertexShader = OpenGLController->CreateShader(GL_VERTEX_SHADER);
	OpenGLController->ShaderSource (_VertexShader, 1, &(const char*)VertexShaderCode, nullptr);
	OpenGLController->CompileShader(_VertexShader);
	OpenGLController->GetShaderiv  (_VertexShader, GL_COMPILE_STATUS, &_HasSucceeded);
	if (!_HasSucceeded)
	{
		OpenGLController->GetShaderInfoLog(_VertexShader, _LogsSize, nullptr, _Logs);
		DebugLogMessage("ERROR: -------- SHADER:VERTEX:COMPILATION_FAILED\n");
		DebugLogMessage(_Logs);
	}

	unsigned int          _FragmentShader = OpenGLController->CreateShader(GL_FRAGMENT_SHADER);
	OpenGLController->ShaderSource (_FragmentShader, 1, &(const char*)FragmentShaderCode, nullptr);
	OpenGLController->CompileShader(_FragmentShader);
	OpenGLController->GetShaderiv  (_FragmentShader, GL_COMPILE_STATUS, &_HasSucceeded);
	if (!_HasSucceeded)
	{
		OpenGLController->GetShaderInfoLog(_FragmentShader, _LogsSize, nullptr, _Logs);
		DebugLogMessage("ERROR: -------- SHADER:FRAGMENT:COMPILATION_FAILED\n");
		DebugLogMessage(_Logs);
	}

	ShaderId = OpenGLController->CreateProgram();
	OpenGLController->AttachShader(ShaderId, _VertexShader);
	OpenGLController->AttachShader(ShaderId, _FragmentShader);
	OpenGLController->LinkProgram (ShaderId);
	OpenGLController->GetShaderiv (ShaderId, GL_LINK_STATUS, &_HasSucceeded);
	if (!_HasSucceeded)
	{
		OpenGLController->GetShaderInfoLog(ShaderId, _LogsSize, nullptr, _Logs);
		DebugLogMessage("ERROR: -------- SHADER:PROGRAM:LINKING_FAILED\n");
		DebugLogMessage(_Logs);
	}

	OpenGLController->DeleteShader(_VertexShader);
	OpenGLController->DeleteShader(_FragmentShader);

	return true;
}

void Shader::SetUniform_MatrixS4(int32 ShaderId, const char* Name, real32* Matrix, OpenGLController* OpenGLController)
{
	OpenGLController->UniformMatrix4fv(OpenGLController->GetUniformLocation(ShaderId, Name), 1, GL_FALSE, Matrix);
}