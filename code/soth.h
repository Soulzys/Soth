#pragma once

#if BUILD_ASSERT
#define ASSERT(Expression) if (!(Expression)) {*(int*)0 = 0;}
#else
#define ASSERT(Expression)
#endif

#define internal static

#define KILOBYTES(Value)   (         (Value) * 1024LL)
#define MEGABYTES(Value)   (KILOBYTES(Value) * 1024LL)
#define GIGABYTES(Value)   (MEGABYTES(Value) * 1024LL)

#define ARRAY_COUNT(Array) (sizeof(Array) / sizeof((Array)[0]))

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
#define GL_LINK_STATUS                    0x8B82


enum KeyState
{
	UP   = 0, 
	DOWN = 1
};


//
//


struct ReadFileResult
{
	uint32 ContentSize ;
	void*  Content     ;
};


//
//

typedef void   (APIENTRY* GL_GEN_VERTEX_ARRAYS          ) (GLsizei n, GLuint* arrays);
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
typedef GLint  (APIENTRY* GL_GET_UNIFORM_LOCATION       ) (GLuint program, const char* name);
typedef void   (APIENTRY* GL_UNIFORM_MATRIX_4FV         ) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

struct OpenGL
{
	GL_GEN_VERTEX_ARRAYS           GenVertexArrays         ;
	GL_DELETE_VERTEX_ARRAYS        DeleteVertexArrays      ;
	GL_GEN_BUFFERS                 GenBuffers              ;
	GL_DELETE_BUFFERS              DeleteBuffers           ;
	GL_BIND_VERTEX_ARRAY           BindVertexArray         ;
	GL_BIND_BUFFER                 BindBuffer              ;
	GL_BUFFER_DATA                 BufferData              ;
	GL_VERTEX_ATTRIB_POINTER       VertexAttribPointer     ;
	GL_ENABLE_VERTEX_ATTRIB_ARRAY  EnableVertexAttribArray ;
	GL_CREATE_SHADER               CreateShader            ;
	GL_CREATE_PROGRAM              CreateProgram           ;
	GL_DELETE_PROGRAM              DeleteProgram           ;
	GL_LINK_PROGRAM                LinkProgram             ;
	GL_USE_PROGRAM                 UseProgram              ;
	GL_SHADER_SOURCE               ShaderSource            ;
	GL_COMPILE_SHADER              CompileShader           ;
	GL_GET_SHADERIV                GetShaderiv             ;
	GL_GET_SHADER_INFO_LOG         GetShaderInfoLog        ;
	GL_ATTACH_SHADER               AttachShader            ;
	GL_DELETE_SHADER               DeleteShader            ;
	GL_GET_UNIFORM_LOCATION        GetUniformLocation      ;
	GL_UNIFORM_MATRIX_4FV          UniformMatrix4fv        ;
};


struct GameState
{
	uint16 VBO           ;
	uint16 VAO           ;
	uint16 ShaderProgram ;
	MatrixS4 PlayerPos   ;
};

struct GameMemory
{
	bool32 IsInitialized        ;
	uint64 PermanentStorageSize ;
	void*  PermanentStorage     ;

	OpenGL* OpenGL;
};

struct GameKeyState
{
	int      HalfTransitionCount;
	KeyState State;
};

struct GameInputController
{
	union
	{
		GameKeyState Keys[3];

		struct
		{
			GameKeyState MoveLeft  ;
			GameKeyState MoveRight ;
			GameKeyState Jump      ;
		};
	};

	void CopyState(GameInputController* Input);
};

static ReadFileResult Win32_ReadFile(const char* Filename);
static void DebugLogVector(const Vec3& V, real32 W = 1.0f, uint8 Precision = 3, const char* Extra = "");
static void DebugLogMatrixS4(const MatrixS4& M, uint8 Precision = 3);
//static void DebugLogMatrix(const MatrixS4& M);
