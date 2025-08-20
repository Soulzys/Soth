#include <windows.h>
#include <gl/gl.h>
#include <stdint.h>
#include "shader.h"
#include "openglcontroller.cpp"

/* TODO

	- Complete the Win32_ReadFile function in order to read shader files
 
*/

typedef uint8_t  uint8  ;
typedef uint16_t uint16 ;
typedef uint32_t uint32 ;
typedef uint64_t uint64 ;

static bool g_Running;





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

static unsigned int _shaderProgram;
static unsigned int _VBO;
static unsigned int _VAO;






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
					if (OpenGL::LoadOpenGLFunctions())
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
						if (OpenGL::ChoosePixelFormatARB(_DeviceContext, _PixelAttribs, nullptr, 1, &_PixelFormatID, &_NumFormats))
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

							HGLRC _RenderingContext = OpenGL::CreateContextAttribsARB(_DeviceContext, 0, _ContextAttribs);
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
									unsigned int _vertexShader = OpenGL::CreateShader(GL_VERTEX_SHADER);
									//glShaderSource_(_vertexShader, 1, &vertexShaderSource, nullptr);
									OpenGL::ShaderSource(_vertexShader, 1, &(const char*)_FileMemory.Content, nullptr);
									OpenGL::CompileShader(_vertexShader);

									int _success = 0;
									char _infoLogs[512];
									OpenGL::GetShaderiv(_vertexShader, GL_COMPILE_STATUS, &_success);
									if (!_success)
									{
										OpenGL::GetShaderInfoLog(_vertexShader, 512, nullptr, _infoLogs);
										OutputDebugString("Vertex shader has issue...");
									}

									_FileMemory = Win32_ReadFile(_FragmentShaderPath);
									unsigned int _fragmentShader = OpenGL::CreateShader(GL_FRAGMENT_SHADER);
									//glShaderSource_(_fragmentShader, 1, &fragmentShaderSource, nullptr);
									OpenGL::ShaderSource(_fragmentShader, 1, &(const char*)_FileMemory.Content, nullptr);
									OpenGL::CompileShader(_fragmentShader);
									OpenGL::GetShaderiv(_fragmentShader, GL_COMPILE_STATUS, &_success);
									if (!_success)
									{
										OpenGL::GetShaderInfoLog(_fragmentShader, 512, nullptr, _infoLogs);
										OutputDebugString("Fragment shader has issue...");
									}

									_shaderProgram = OpenGL::CreateProgram();
									OpenGL::AttachShader(_shaderProgram, _vertexShader);
									OpenGL::AttachShader(_shaderProgram, _fragmentShader);
									OpenGL::LinkProgram(_shaderProgram);
									OpenGL::GetShaderiv(_shaderProgram, GL_COMPILE_STATUS, &_success);
									if (!_success)
									{
										OpenGL::GetShaderInfoLog(_shaderProgram, 512, nullptr, _infoLogs);
										OutputDebugString("Program shader has issue...");
									}
									
									OpenGL::DeleteShader(_vertexShader);
									OpenGL::DeleteShader(_fragmentShader);
									Util_FreeFileMemory(_FileMemory.Content);

									//

									float _vertices[] = 
									{
										-0.5f, -0.5f, 0.0f, 
										0.5f, -0.5f, 0.0f, 
										0.0f, 0.5f, 0.0f
									};

									
									OpenGL::GenVertexArrays(1, &_VAO);
									OpenGL::GenBuffers(1, &_VBO);
									OpenGL::BindVertexArray(_VAO);

									OpenGL::BindBuffer(GL_ARRAY_BUFFER, _VBO);
									OpenGL::BufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

									OpenGL::VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
									OpenGL::EnableVertexAttribArray(0);

									OpenGL::BindBuffer(GL_ARRAY_BUFFER, 0);
									OpenGL::BindVertexArray(0);
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

					OpenGL::UseProgram(_shaderProgram);
					OpenGL::BindVertexArray(_VAO);
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



	OpenGL::DeleteVertexArrays(1, &_VAO);
	OpenGL::DeleteBuffers(1, &_VBO);
	OpenGL::DeleteProgram(_shaderProgram);

	return (0);
}