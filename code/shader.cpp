#include "shader.h"

bool Shader::CreateShader(void* VertexShaderCode, void* FragmentShaderCode, unsigned int& ShaderId)
{
	if (!VertexShaderCode || !FragmentShaderCode) return false;

	int _HasSucceeded;
	const int16 _LogsSize = 512;
	char _Logs[_LogsSize];

	unsigned int          _VertexShader = OpenGL::CreateShader(GL_VERTEX_SHADER);
	OpenGL::ShaderSource (_VertexShader, 1, &(const char*)VertexShaderCode, nullptr);
	OpenGL::CompileShader(_VertexShader);
	OpenGL::GetShaderiv  (_VertexShader, GL_COMPILE_STATUS, &_HasSucceeded);
	if (!_HasSucceeded)
	{
		OpenGL::GetShaderInfoLog(_VertexShader, _LogsSize, nullptr, _Logs);
		DebugLogMessage("ERROR: -------- SHADER:VERTEX:COMPILATION_FAILED\n");
		DebugLogMessage(_Logs);
	}

	unsigned int          _FragmentShader = OpenGL::CreateShader(GL_FRAGMENT_SHADER);
	OpenGL::ShaderSource (_FragmentShader, 1, &(const char*)FragmentShaderCode, nullptr);
	OpenGL::CompileShader(_FragmentShader);
	OpenGL::GetShaderiv  (_FragmentShader, GL_COMPILE_STATUS, &_HasSucceeded);
	if (!_HasSucceeded)
	{
		OpenGL::GetShaderInfoLog(_FragmentShader, _LogsSize, nullptr, _Logs);
		DebugLogMessage("ERROR: -------- SHADER:FRAGMENT:COMPILATION_FAILED\n");
		DebugLogMessage(_Logs);
	}

	ShaderId = OpenGL::CreateProgram();
	OpenGL::AttachShader(ShaderId, _VertexShader);
	OpenGL::AttachShader(ShaderId, _FragmentShader);
	OpenGL::LinkProgram (ShaderId);
	OpenGL::GetShaderiv (ShaderId, GL_LINK_STATUS, &_HasSucceeded);
	if (!_HasSucceeded)
	{
		OpenGL::GetShaderInfoLog(ShaderId, _LogsSize, nullptr, _Logs);
		DebugLogMessage("ERROR: -------- SHADER:PROGRAM:LINKING_FAILED\n");
		DebugLogMessage(_Logs);
	}

	OpenGL::DeleteShader(_VertexShader);
	OpenGL::DeleteShader(_FragmentShader);

	return true;
}
