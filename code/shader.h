#pragma once

struct OpenGL;

namespace Shader
{
	bool CreateShader(void* VertexShaderCode, void* FragmentShaderCode, unsigned int& ShaderId, GameMemory* Memory);
	void SetUniform_MatrixS4(int32 ShaderId, const char* Name, real32* Matrix, OpenGL* OpenGL);
}