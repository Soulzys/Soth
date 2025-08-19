#include <windows.h>
#include <gl/gl.h>
#include <stdint.h>

/* TODO

	- Complete the Win32_ReadFile function in order to read shader files
 
*/

typedef uint8_t  uint8  ;
typedef uint16_t uint16 ;
typedef uint32_t uint32 ;
typedef uint64_t uint64 ;

static bool g_Running;

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

static WGL_CHOOSE_PIXEL_FORMAT_ARB    wglChoosePixelFormatARB_    ;
static WGL_CREATE_CONTEXT_ATTRIBS_ARB wglCreateContextAttribsARB_ ;
static GL_GEN_VERTEX_ARRAYS           glGenVertexArrays_          ;
static GL_DELETE_VERTEX_ARRAYS        glDeleteVertexArrays_       ;
static GL_GEN_BUFFERS                 glGenBuffers_               ;
static GL_DELETE_BUFFERS              glDeleteBuffers_            ;
static GL_BIND_VERTEX_ARRAY           glBindVertexArray_          ;
static GL_BIND_BUFFER                 glBindBuffer_               ;
static GL_BUFFER_DATA                 glBufferData_               ;
static GL_VERTEX_ATTRIB_POINTER       glVertexAttribPointer_      ;
static GL_ENABLE_VERTEX_ATTRIB_ARRAY  glEnableVertexAttribArray_  ;
static GL_CREATE_SHADER               glCreateShader_             ;
static GL_CREATE_PROGRAM              glCreateProgram_            ;
static GL_DELETE_PROGRAM              glDeleteProgram_            ;
static GL_LINK_PROGRAM                glLinkProgram_              ;
static GL_USE_PROGRAM                 glUseProgram_               ;
static GL_SHADER_SOURCE               glShaderSource_             ;
static GL_COMPILE_SHADER              glCompileShader_            ;
static GL_GET_SHADERIV                glGetShaderiv_              ;
static GL_GET_SHADER_INFO_LOG         glGetShaderInfoLog_         ;
static GL_ATTACH_SHADER               glAttachShader_             ;
static GL_DELETE_SHADER               glDeleteShader_             ;



#if BUILD_ASSERT
#define ASSERT(Expression) if (!(Expression)) {*(int*)0 = 0;}
#else
#define ASSERT(Expression)
#endif


// Utilitary structs
//
struct ReadFileResult
{
	uint32 ContentSize ;
	void*  Content     ;
};

static uint32 Util_SafeTruncate_uint64(uint64 Value)
{
	ASSERT(Value <= 0xFFFFFFFFF);
	uint32 _Result = (uint32)Value;
	return _Result;
}

static BOOL Util_FreeFileMemory(void* Memory)
{
	if (Memory)
	{
		VirtualFree(Memory, 0, MEM_RELEASE);
		Memory = nullptr;
		return 1;
	}
	return 0;
}


// Utilitary functions
//
static ReadFileResult Win32_ReadFile(const char* Filename)
{
	ReadFileResult _Result = {};

	HANDLE _FileHandle = CreateFile(Filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (_FileHandle != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER _FileSize;
		if (GetFileSizeEx(_FileHandle, &_FileSize))
		{
			uint32 _FileSize32 = Util_SafeTruncate_uint64(_FileSize.QuadPart);
			_Result.Content = VirtualAlloc(0, _FileSize32, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			if (_Result.Content)
			{
				DWORD _BytesRead;
				if (ReadFile(_FileHandle, _Result.Content, _FileSize32, &_BytesRead, 0) && (_BytesRead == _FileSize32))
				{
					_Result.ContentSize = _FileSize32;
				}
				else
				{
					Util_FreeFileMemory(_Result.Content);
				}
			}
			else
			{
				// LOGGING
			}
		}
		else
		{
			// LOGGING
		}

		CloseHandle(_FileHandle);
	}
	else
	{
		// LOGGING
	}
	
	return _Result;
}


/*
static const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
static const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
*/

static unsigned int _shaderProgram;
static unsigned int _VBO;
static unsigned int _VAO;




static bool Win32_LoadOpenGLFunctions()
{
	wglChoosePixelFormatARB_    = (WGL_CHOOSE_PIXEL_FORMAT_ARB   )(wglGetProcAddress("wglChoosePixelFormatARB"   ));
	if (!wglChoosePixelFormatARB_)    return false;

	wglCreateContextAttribsARB_ = (WGL_CREATE_CONTEXT_ATTRIBS_ARB)(wglGetProcAddress("wglCreateContextAttribsARB"));
	if (!wglCreateContextAttribsARB_) return false;

	glGenVertexArrays_          = (GL_GEN_VERTEX_ARRAYS          )(wglGetProcAddress("glGenVertexArrays"         ));
	if (!glGenVertexArrays_)          return false;

	glDeleteVertexArrays_       = (GL_DELETE_VERTEX_ARRAYS       )(wglGetProcAddress("glDeleteVertexArrays"      ));
	if (!glDeleteVertexArrays_)       return false;

	glGenBuffers_               = (GL_GEN_BUFFERS                )(wglGetProcAddress("glGenBuffers"              ));
	if (!glGenBuffers_)               return false;

	glDeleteBuffers_            = (GL_DELETE_BUFFERS             )(wglGetProcAddress("glDeleteBuffers"           ));
	if (!glDeleteBuffers_)            return false;

	glBindVertexArray_          = (GL_BIND_VERTEX_ARRAY          )(wglGetProcAddress("glBindVertexArray"         ));
	if (!glBindVertexArray_)          return false;

	glBindBuffer_               = (GL_BIND_BUFFER                )(wglGetProcAddress("glBindBuffer"              ));
	if (!glBindBuffer_)               return false;

	glBufferData_               = (GL_BUFFER_DATA                )(wglGetProcAddress("glBufferData"              ));
	if (!glBufferData_)               return false;

	glVertexAttribPointer_      = (GL_VERTEX_ATTRIB_POINTER      )(wglGetProcAddress("glVertexAttribPointer"     ));
	if (!glVertexAttribPointer_)      return false;

	glEnableVertexAttribArray_  = (GL_ENABLE_VERTEX_ATTRIB_ARRAY )(wglGetProcAddress("glEnableVertexAttribArray" ));
	if (!glEnableVertexAttribArray_)  return false;

	glCreateShader_             = (GL_CREATE_SHADER              )(wglGetProcAddress("glCreateShader"            ));
	if (!glCreateShader_)             return false;

	glCreateProgram_            = (GL_CREATE_PROGRAM             )(wglGetProcAddress("glCreateProgram"           ));
	if (!glCreateProgram_)            return false;

	glDeleteProgram_            = (GL_DELETE_PROGRAM             )(wglGetProcAddress("glDeleteProgram"           ));
	if (!glDeleteProgram_)            return false;

	glLinkProgram_              = (GL_LINK_PROGRAM               )(wglGetProcAddress("glLinkProgram"             ));
	if (!glLinkProgram_)              return false;

	glUseProgram_               = (GL_USE_PROGRAM                )(wglGetProcAddress("glUseProgram"              ));
	if (!glUseProgram_)               return false;

	glShaderSource_             = (GL_SHADER_SOURCE              )(wglGetProcAddress("glShaderSource"            ));
	if (!glShaderSource_)             return false;

	glCompileShader_            = (GL_COMPILE_SHADER             )(wglGetProcAddress("glCompileShader"           ));
	if (!glCompileShader_)            return false;

	glGetShaderiv_              = (GL_GET_SHADERIV               )(wglGetProcAddress("glGetShaderiv"             ));
	if (!glGetShaderiv_)              return false;

	glGetShaderInfoLog_         = (GL_GET_SHADER_INFO_LOG        )(wglGetProcAddress("glGetShaderInfoLog"        ));
	if (!glGetShaderInfoLog_)         return false;

	glAttachShader_             = (GL_ATTACH_SHADER              )(wglGetProcAddress("glAttachShader"            ));
	if (!glAttachShader_)             return false;

	glDeleteShader_             = (GL_DELETE_SHADER              )(wglGetProcAddress("glDeleteShader"            ));
	if (!glDeleteShader_)             return false;

	return true;
}

// >TOTHINK: returning WindowHandle via the function's return value rather than via a out parameter ? 
static bool Win32_InitOpenGL(HINSTANCE Instance, WNDCLASS* Window, HWND& WindowHandle)
{
	if (!Window) return false;

	// >NOTE: read about the necessity of the sacrificial objects (window, context...) : https://mariuszbartosik.com/opengl-4-x-initialization-in-windows-without-a-framework/
	HWND _SacrificialWindow = CreateWindowEx
	(
		0                    , 
		"Core"               ,
		"Sacrificial Window" , 
		WS_OVERLAPPEDWINDOW  ,
		CW_USEDEFAULT        ,
		CW_USEDEFAULT        , 
		CW_USEDEFAULT        , 
		CW_USEDEFAULT        , 
		0                    , 
		0                    , 
		Instance             , 
		0
	);

	HDC _SacrificialDC = GetDC(_SacrificialWindow); // Handle to Device Context

	PIXELFORMATDESCRIPTOR _SacrificialPFD = {};
	_SacrificialPFD.nSize      = sizeof(_SacrificialPFD);
	_SacrificialPFD.nVersion   = 1;
	_SacrificialPFD.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	_SacrificialPFD.iPixelType = PFD_TYPE_RGBA;
	_SacrificialPFD.cColorBits = 32;
	_SacrificialPFD.cAlphaBits = 8;
	_SacrificialPFD.cDepthBits = 24;

	int _SacrificialPFDID = ChoosePixelFormat(_SacrificialDC, &_SacrificialPFD);
	if (_SacrificialPFDID != 0)
	{
		if (SetPixelFormat(_SacrificialDC, _SacrificialPFDID, &_SacrificialPFD))
		{
			HGLRC _SacrificialRC = wglCreateContext(_SacrificialDC); // Handle to OpenGL Rendering Context
			if (_SacrificialRC != 0)
			{
				if (wglMakeCurrent(_SacrificialDC, _SacrificialRC))
				{
					if (Win32_LoadOpenGLFunctions())
					{
						// The real window
						WindowHandle = CreateWindowEx
						(
							0,
							Window->lpszClassName,
							"Soth",
							WS_OVERLAPPEDWINDOW | WS_VISIBLE,
							CW_USEDEFAULT,
							CW_USEDEFAULT,
							CW_USEDEFAULT,
							CW_USEDEFAULT,
							0,
							0,
							Instance,
							0
						);
						HDC _DeviceContext = GetDC(WindowHandle);

						int _PixelAttribs[] =
						{
							WGL_DRAW_TO_WINDOW_ARB , GL_TRUE                   ,
							WGL_SUPPORT_OPENGL_ARB , GL_TRUE                   ,
							WGL_DOUBLE_BUFFER_ARB  , GL_TRUE                   ,
							WGL_PIXEL_TYPE_ARB     , WGL_TYPE_RGBA_ARB         ,
							WGL_ACCELERATION_ARB   , WGL_FULL_ACCELERATION_ARB ,
							WGL_COLOR_BITS_ARB     , 32                        ,
							WGL_ALPHA_BITS_ARB     , 8                         ,
							WGL_DEPTH_BITS_ARB     , 24                        ,
							WGL_STENCIL_BITS_ARB   , 8                         ,
							WGL_SAMPLE_BUFFERS_ARB , GL_TRUE                   ,
							WGL_SAMPLES_ARB        , 4                         ,
							0 // Null terminate the array
						};

						int _PixelFormatID;
						UINT _NumFormats;
						if (wglChoosePixelFormatARB_(_DeviceContext, _PixelAttribs, nullptr, 1, &_PixelFormatID, &_NumFormats))
						{
							if (_NumFormats == 0) return false;

							PIXELFORMATDESCRIPTOR _PFD = {};
							DescribePixelFormat(_DeviceContext, _PixelFormatID, sizeof(_PFD), &_PFD);
							SetPixelFormat(_DeviceContext, _PixelFormatID, &_PFD);

							// Requesting OpenGL 4.0
							// >TOTHINK: perhaps cover v3.3 as a backup ?
							int _ContextAttribs[] =
							{
								WGL_CONTEXT_MAJOR_VERSION_ARB , 4 ,
								WGL_CONTEXT_MINOR_VERSION_ARB , 0 ,
								0
							};

							HGLRC _RenderingContext = wglCreateContextAttribsARB_(_DeviceContext, 0, _ContextAttribs);
							if (_RenderingContext)
							{
								wglMakeCurrent(nullptr, nullptr);
								wglDeleteContext(_SacrificialRC);
								ReleaseDC(_SacrificialWindow, _SacrificialDC);
								DestroyWindow(_SacrificialWindow);

								if (wglMakeCurrent(_DeviceContext, _RenderingContext))
								{
									// TODO: load OpenGL functions
									SetWindowText(WindowHandle, (LPCSTR)glGetString(GL_VERSION));



									//
									// Testing code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
									//

									const char* _VertexShaderPath = "R:\\resources\\shader.vs";
									const char* _FragmentShaderPath = "R:\\resources\\shader.fs";
									//if (_FileMemory.Content)
									//{
									//	Util_FreeFileMemory(_FileMemory.Content);
									//}

									ReadFileResult _FileMemory = Win32_ReadFile(_VertexShaderPath);
									unsigned int _vertexShader = glCreateShader_(GL_VERTEX_SHADER);
									//glShaderSource_(_vertexShader, 1, &vertexShaderSource, nullptr);
									glShaderSource_(_vertexShader, 1, &(const char*)_FileMemory.Content, nullptr);
									glCompileShader_(_vertexShader);

									int _success = 0;
									char _infoLogs[512];
									glGetShaderiv_(_vertexShader, GL_COMPILE_STATUS, &_success);
									if (!_success)
									{
										glGetShaderInfoLog_(_vertexShader, 512, nullptr, _infoLogs);
										OutputDebugString("Vertex shader has issue...");
									}

									_FileMemory = Win32_ReadFile(_FragmentShaderPath);
									unsigned int _fragmentShader = glCreateShader_(GL_FRAGMENT_SHADER);
									//glShaderSource_(_fragmentShader, 1, &fragmentShaderSource, nullptr);
									glShaderSource_(_fragmentShader, 1, &(const char*)_FileMemory.Content, nullptr);
									glCompileShader_(_fragmentShader);
									glGetShaderiv_(_fragmentShader, GL_COMPILE_STATUS, &_success);
									if (!_success)
									{
										glGetShaderInfoLog_(_fragmentShader, 512, nullptr, _infoLogs);
										OutputDebugString("Fragment shader has issue...");
									}

									_shaderProgram = glCreateProgram_();
									glAttachShader_(_shaderProgram, _vertexShader);
									glAttachShader_(_shaderProgram, _fragmentShader);
									glLinkProgram_(_shaderProgram);
									glGetShaderiv_(_shaderProgram, GL_COMPILE_STATUS, &_success);
									if (!_success)
									{
										glGetShaderInfoLog_(_shaderProgram, 512, nullptr, _infoLogs);
										OutputDebugString("Program shader has issue...");
									}
									
									glDeleteShader_(_vertexShader);
									glDeleteShader_(_fragmentShader);
									Util_FreeFileMemory(_FileMemory.Content);

									//

									float _vertices[] = 
									{
										-0.5f, -0.5f, 0.0f, 
										0.5f, -0.5f, 0.0f, 
										0.0f, 0.5f, 0.0f
									};

									
									glGenVertexArrays_(1, &_VAO);
									glGenBuffers_(1, &_VBO);
									glBindVertexArray_(_VAO);

									glBindBuffer_(GL_ARRAY_BUFFER, _VBO);
									glBufferData_(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

									glVertexAttribPointer_(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
									glEnableVertexAttribArray_(0);

									glBindBuffer_(GL_ARRAY_BUFFER, 0);
									glBindVertexArray_(0);
								}
								else
								{
									// LOGGING
									return false;
								}
							}
							else
							{
								// LOGGING
								return false;
							}
						}
						else
						{
							// LOGGING
							return false;
						}
					}
					else
					{
						// LOGGING
						return false;
					}
				}
				else
				{
					// LOGGING
					return false;
				}
			}
			else
			{
				// LOGGING
				return false;
			}
		}
		else
		{
			// LOGGING
			return false;
		}
	}
	else
	{
		// LOGING
		return false;
	}

	return true;
}

LRESULT CALLBACK Win32_WindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
	LRESULT _Result = 0;

	switch (Message)
	{
		case WM_CLOSE: 
		case WM_DESTROY:
		{
			g_Running = false;
		} break;

		case WM_KEYDOWN:
		{
			uint32 _VKCode = (uint32)WParam;

			if (_VKCode == VK_ESCAPE)
			{
				g_Running = false;
			}

		} break;

		default:
		{
			_Result = DefWindowProc(Window, Message, WParam, LParam);
		} break;
	}

	return _Result;
}

int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CmdLine, int CmdShow)
{
	WNDCLASS _WindowClass      = {}                                 ;
	_WindowClass.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW ;
	_WindowClass.lpfnWndProc   = Win32_WindowCallback               ;
	_WindowClass.hInstance     = Instance                           ;
	_WindowClass.lpszClassName = "Soth"                             ;

	if (RegisterClass(&_WindowClass))
	{
		HWND _WindowHandle = nullptr;
		if (Win32_InitOpenGL(Instance, &_WindowClass, _WindowHandle))
		{
			HDC _DC = GetDC(_WindowHandle);
			g_Running = true;

			while (g_Running)
			{
				MSG _Message;
				while (PeekMessage(&_Message, 0, 0, 0, PM_REMOVE))
				{
					if (_Message.message == WM_QUIT)
					{
						g_Running = false;
					}


					glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
					glClear(GL_COLOR_BUFFER_BIT);

					glUseProgram_(_shaderProgram);
					glBindVertexArray_(_VAO);
					glDrawArrays(GL_TRIANGLES, 0, 3);


					SwapBuffers(_DC);

					TranslateMessage(&_Message);
					DispatchMessage(&_Message);					
				}
			}
		}
		else
		{
			// LOGGING
		}
	}
	else
	{
		// LOGGING
	}



	glDeleteVertexArrays_(1, &_VAO);
	glDeleteBuffers_(1, &_VBO);
	glDeleteProgram_(_shaderProgram);

	return (0);
}