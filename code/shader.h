static void DebugLogMessage(const char* Message);

namespace Shader
{
	static bool CreateShader(void* VertexShaderCode, void* FragmentShaderCode, unsigned int& ShaderId);
	static void SetUniform_MatrixS4(int32 ShaderId, const char* Name, real32* Matrix);
}