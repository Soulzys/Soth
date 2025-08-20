// From wglext.h
//
#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_ACCELERATION_ARB              0x2003
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_ALPHA_BITS_ARB                0x201B
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_STENCIL_BITS_ARB              0x2023
#define WGL_FULL_ACCELERATION_ARB         0x2027
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_SAMPLE_BUFFERS_ARB            0x2041
#define WGL_SAMPLES_ARB                   0x2042
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define GL_ARRAY_BUFFER                   0x8892
#define GL_STATIC_DRAW                    0x88E4
#define GL_VERTEX_SHADER                  0x8B31
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_COMPILE_STATUS                 0x8B81

// From glext.h
//
// Required functions to initialize OpengGL > 3.0
typedef BOOL   (WINAPI*   WGL_CHOOSE_PIXEL_FORMAT_ARB   ) (HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
typedef HGLRC  (WINAPI*   WGL_CREATE_CONTEXT_ATTRIBS_ARB) (HDC hDC, HGLRC hShareContext, const int* attribList);
// Required functions for the game proper
typedef void   (APIENTRY* GL_GEN_VERTEX_ARRAYS          ) (GLsizei n, GLuint* arrays);
typedef void   (APIENTRY* GL_DELETE_VERTEX_ARRAYS       ) (GLsizei n, const GLuint* arrays);
typedef void   (APIENTRY* GL_GEN_BUFFERS                ) (GLsizei n, GLuint* buffers);
typedef void   (APIENTRY* GL_DELETE_BUFFERS             ) (GLsizei n, const GLuint* buffers);
typedef void   (APIENTRY* GL_BIND_VERTEX_ARRAY          ) (GLuint array);
typedef void   (APIENTRY* GL_BIND_BUFFER                ) (GLenum target, GLuint buffer);
typedef void   (APIENTRY* GL_BUFFER_DATA                ) (GLenum target, ptrdiff_t size, const void* data, GLenum usage);
typedef void   (APIENTRY* GL_VERTEX_ATTRIB_POINTER      ) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
typedef void   (APIENTRY* GL_ENABLE_VERTEX_ATTRIB_ARRAY ) (GLuint index);
typedef GLuint (APIENTRY* GL_CREATE_SHADER              ) (GLenum type);
typedef GLuint (APIENTRY* GL_CREATE_PROGRAM             ) (void);
typedef void   (APIENTRY* GL_DELETE_PROGRAM             ) (GLuint program);
typedef void   (APIENTRY* GL_LINK_PROGRAM               ) (GLuint program);
typedef void   (APIENTRY* GL_USE_PROGRAM                ) (GLuint program);
typedef void   (APIENTRY* GL_SHADER_SOURCE              ) (GLuint shader, GLsizei count, const char** string, const GLint* length);
typedef void   (APIENTRY* GL_COMPILE_SHADER             ) (GLuint shader);
typedef void   (APIENTRY* GL_GET_SHADERIV               ) (GLuint shader, GLenum pname, GLint* params);
typedef void   (APIENTRY* GL_GET_SHADER_INFO_LOG        ) (GLuint shader, GLsizei bufSize, GLsizei* length, char* infoLog);
typedef void   (APIENTRY* GL_ATTACH_SHADER              ) (GLuint program, GLuint shader);
typedef void   (APIENTRY* GL_DELETE_SHADER              ) (GLuint shader);

namespace OpenGL
{
	static WGL_CHOOSE_PIXEL_FORMAT_ARB    ChoosePixelFormatARB    ;
	static WGL_CREATE_CONTEXT_ATTRIBS_ARB CreateContextAttribsARB ;
	static GL_GEN_VERTEX_ARRAYS           GenVertexArrays         ;
	static GL_DELETE_VERTEX_ARRAYS        DeleteVertexArrays      ;
	static GL_GEN_BUFFERS                 GenBuffers              ;
	static GL_DELETE_BUFFERS              DeleteBuffers           ;
	static GL_BIND_VERTEX_ARRAY           BindVertexArray         ;
	static GL_BIND_BUFFER                 BindBuffer              ;
	static GL_BUFFER_DATA                 BufferData              ;
	static GL_VERTEX_ATTRIB_POINTER       VertexAttribPointer     ;
	static GL_ENABLE_VERTEX_ATTRIB_ARRAY  EnableVertexAttribArray ;
	static GL_CREATE_SHADER               CreateShader            ;
	static GL_CREATE_PROGRAM              CreateProgram           ;
	static GL_DELETE_PROGRAM              DeleteProgram           ;
	static GL_LINK_PROGRAM                LinkProgram             ;
	static GL_USE_PROGRAM                 UseProgram              ;
	static GL_SHADER_SOURCE               ShaderSource            ;
	static GL_COMPILE_SHADER              CompileShader           ;
	static GL_GET_SHADERIV                GetShaderiv             ;
	static GL_GET_SHADER_INFO_LOG         GetShaderInfoLog        ;
	static GL_ATTACH_SHADER               AttachShader            ;
	static GL_DELETE_SHADER               DeleteShader            ;


	static bool LoadOpenGLFunctions()
	{
		ChoosePixelFormatARB     = (WGL_CHOOSE_PIXEL_FORMAT_ARB   )(wglGetProcAddress("wglChoosePixelFormatARB"   ));
		if (!ChoosePixelFormatARB)     return false;

		CreateContextAttribsARB  = (WGL_CREATE_CONTEXT_ATTRIBS_ARB)(wglGetProcAddress("wglCreateContextAttribsARB"));
		if (!CreateContextAttribsARB)  return false;

		GenVertexArrays          = (GL_GEN_VERTEX_ARRAYS          )(wglGetProcAddress("glGenVertexArrays"         ));
		if (!GenVertexArrays)          return false;

		DeleteVertexArrays       = (GL_DELETE_VERTEX_ARRAYS       )(wglGetProcAddress("glDeleteVertexArrays"      ));
		if (!DeleteVertexArrays)       return false;

		GenBuffers               = (GL_GEN_BUFFERS                )(wglGetProcAddress("glGenBuffers"              ));
		if (!GenBuffers)               return false;

		DeleteBuffers            = (GL_DELETE_BUFFERS             )(wglGetProcAddress("glDeleteBuffers"           ));
		if (!DeleteBuffers)            return false;

		BindVertexArray          = (GL_BIND_VERTEX_ARRAY          )(wglGetProcAddress("glBindVertexArray"         ));
		if (!BindVertexArray)          return false;

		BindBuffer               = (GL_BIND_BUFFER                )(wglGetProcAddress("glBindBuffer"              ));
		if (!BindBuffer)               return false;

		BufferData               = (GL_BUFFER_DATA                )(wglGetProcAddress("glBufferData"              ));
		if (!BufferData)               return false;

		VertexAttribPointer      = (GL_VERTEX_ATTRIB_POINTER      )(wglGetProcAddress("glVertexAttribPointer"     ));
		if (!VertexAttribPointer)      return false;

		EnableVertexAttribArray  = (GL_ENABLE_VERTEX_ATTRIB_ARRAY )(wglGetProcAddress("glEnableVertexAttribArray" ));
		if (!EnableVertexAttribArray)  return false;

		CreateShader             = (GL_CREATE_SHADER              )(wglGetProcAddress("glCreateShader"            ));
		if (!CreateShader)             return false;

		CreateProgram            = (GL_CREATE_PROGRAM             )(wglGetProcAddress("glCreateProgram"           ));
		if (!CreateProgram)            return false;

		DeleteProgram            = (GL_DELETE_PROGRAM             )(wglGetProcAddress("glDeleteProgram"           ));
		if (!DeleteProgram)            return false;

		LinkProgram              = (GL_LINK_PROGRAM               )(wglGetProcAddress("glLinkProgram"             ));
		if (!LinkProgram)              return false;

		UseProgram               = (GL_USE_PROGRAM                )(wglGetProcAddress("glUseProgram"              ));
		if (!UseProgram)               return false;

		ShaderSource             = (GL_SHADER_SOURCE              )(wglGetProcAddress("glShaderSource"            ));
		if (!ShaderSource)             return false;

		CompileShader            = (GL_COMPILE_SHADER             )(wglGetProcAddress("glCompileShader"           ));
		if (!CompileShader)            return false;

		GetShaderiv              = (GL_GET_SHADERIV               )(wglGetProcAddress("glGetShaderiv"             ));
		if (!GetShaderiv)              return false;

		GetShaderInfoLog         = (GL_GET_SHADER_INFO_LOG        )(wglGetProcAddress("glGetShaderInfoLog"        ));
		if (!GetShaderInfoLog)         return false;

		AttachShader             = (GL_ATTACH_SHADER              )(wglGetProcAddress("glAttachShader"            ));
		if (!AttachShader)             return false;

		DeleteShader             = (GL_DELETE_SHADER              )(wglGetProcAddress("glDeleteShader"            ));
		if (!DeleteShader)             return false;

		return true;
	}
}