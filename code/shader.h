#pragma once

//#include "soth.h"

struct OpenGL;

static void DebugLogMessage(const char* Message);

namespace Shader
{
	bool CreateShader(void* VertexShaderCode, void* FragmentShaderCode, unsigned int& ShaderId, OpenGL* OpenGL);
	void SetUniform_MatrixS4(int32 ShaderId, const char* Name, real32* Matrix, OpenGL* OpenGL);
}