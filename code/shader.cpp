#include "shader.h"
//#include "openglcontroller.cpp"

bool Shader::CreateShader(void* VertexShaderCode, void* FragmentShaderCode, unsigned int& ShaderId)
{
	if (!VertexShaderCode || !FragmentShaderCode) return false;

	unsigned int _VertexShader = OpenGL::CreateShader(GL_VERTEX_SHADER);
	OpenGL::ShaderSource(_VertexShader, 1, &(const char*)VertexShaderCode, nullptr);
	OpenGL::CompileShader(_VertexShader);
	// TODO: log compile shader

	unsigned int _FragmentShader = OpenGL::CreateShader(GL_FRAGMENT_SHADER);
	OpenGL::ShaderSource(_FragmentShader, 1, &(const char*)FragmentShaderCode, nullptr);
	OpenGL::CompileShader(_FragmentShader);
	// TODO: log compile shader

	//int ShaderId;
	ShaderId = OpenGL::CreateProgram();
	OpenGL::AttachShader(ShaderId, _VertexShader);
	OpenGL::AttachShader(ShaderId, _FragmentShader);
	OpenGL::LinkProgram(ShaderId);
	// TODO: log compile shader

	OpenGL::DeleteShader(_VertexShader);
	OpenGL::DeleteShader(_FragmentShader);

	return true;
}