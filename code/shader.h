static void DebugLogMessage(const char* Message);

namespace Shader
{
	static bool CreateShader(void* VertexShaderCode, void* FragmentShaderCode, unsigned int& ShaderId);
}