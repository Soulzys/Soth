#ifndef	AGNOSTIC_OPENGL_H
#define AGNOSTIC_OPENGL_H

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
#define GL_TRUE                           1
#define GL_FALSE                          0
#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_2_BYTES                        0x1407
#define GL_3_BYTES                        0x1408
#define GL_4_BYTES                        0x1409
#define GL_DOUBLE                         0x140A

#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006
#define GL_QUADS                          0x0007
#define GL_QUAD_STRIP                     0x0008
#define GL_POLYGON                        0x0009

#define GL_VERSION                        0x1F02

#define GL_COLOR_BUFFER_BIT               0x00004000

#define GL_VIEWPORT                       0x0BA2


typedef unsigned int         GLenum;
typedef unsigned char        GLboolean;
typedef unsigned int         GLbitfield;
typedef signed char          GLbyte;
typedef short                GLshort;
typedef int                  GLint;
typedef int                  GLsizei;
typedef unsigned char        GLubyte;
typedef unsigned short       GLushort;
typedef unsigned int         GLuint;
typedef float                GLfloat;
typedef float                GLclampf;
typedef double               GLdouble;
typedef double               GLclampd;
typedef void                 GLvoid;

typedef signed long long int GLsizeiptr;


#ifndef APIENTRY 
#define APIENTRY
#endif

#ifdef _WIN32
#define GLAPI extern "C" __declspec(dllimport)
#else
#define GLAPI extern "C"
#endif


GLAPI void APIENTRY glDrawArrays(GLenum mode, GLint first, GLsizei count);
GLAPI void APIENTRY glGetIntegerv(GLenum pname, GLint* params);
GLAPI void APIENTRY glViewport(GLint x, GLint y, GLsizei width, GLsizei height);


typedef void   (APIENTRY* GL_GEN_VERTEX_ARRAYS          ) (GLsizei n, GLuint* arrays);
typedef void   (APIENTRY* GL_GEN_VERTEX_ARRAYS          ) (GLsizei n, GLuint* arrays);
typedef void   (APIENTRY* GL_DELETE_VERTEX_ARRAYS       ) (GLsizei n, const GLuint* arrays);
typedef void   (APIENTRY* GL_GEN_BUFFERS                ) (GLsizei n, GLuint* buffers);
typedef void   (APIENTRY* GL_DELETE_BUFFERS             ) (GLsizei n, const GLuint* buffers);
typedef void   (APIENTRY* GL_BIND_VERTEX_ARRAY          ) (GLuint array);
typedef void   (APIENTRY* GL_BIND_BUFFER                ) (GLenum target, GLuint buffer);
typedef void   (APIENTRY* GL_BUFFER_DATA                ) (GLenum target, GLsizeiptr size, const void* data, GLenum usage);
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
typedef void (APIENTRY* GL_DRAW_ARRAYS)(GLenum mode, GLint first, GLsizei count);

//void APIENTRY glDrawArrays(GLenum mode, GLint first, GLsizei count)


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
	GL_DRAW_ARRAYS                 DrawArrays              ;
};


#endif