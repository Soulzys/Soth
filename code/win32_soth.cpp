#include "win32_soth.h"
#include <windows.h>
#include <gl/gl.h>
#include <stdint.h>

/* TODO

	- 
 
*/

#define PI32                  3.14159265359
#define WINDOW_WIDTH_DEFAULT  1200
#define WINDOW_HEIGHT_DEFAULT 800

static Win32_ClientSize g_ClientSize;

typedef int8_t   int8   ;
typedef int16_t  int16  ;
typedef int32_t  int32  ;
typedef int64_t  int64  ;
typedef uint8_t  uint8  ;
typedef uint16_t uint16 ;
typedef uint32_t uint32 ;
typedef uint64_t uint64 ;

typedef float    real32 ;
typedef double   real64 ;

typedef int32    bool32 ;


#include "utils.cpp"
//#include "openglcontroller.cpp"
#include "shader.cpp"
#include "soth.cpp"

static bool g_Running;


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

//static unsigned int _shaderProgram;
//static unsigned int _VBO;
//static unsigned int _VAO;



Win32_ClientSize Win32_GetClientSize(HWND Window)
{
	Win32_ClientSize _Result = {};
	RECT             _R      = {};

	GetClientRect(Window, &_R);
	_Result.Width  = _R.right - _R.left   ;
	_Result.Height = _R.top   - _R.bottom ;

	return _Result;
}

struct DebugFloatingNumber
{
	uint64 IntegerPart;
	uint64 DecimalPart;
};

static DebugFloatingNumber DebugConvertFloatToInt(real32 F, uint8 Precision)
{
	DebugFloatingNumber _R = {};
	_R.IntegerPart = (uint64)F;
	F -= _R.IntegerPart;

	for (uint8 i = 0; i <= Precision; i++)
	{
		F *= 10;
	}

	_R.DecimalPart = (uint64)F;

	return _R;
}

static DebugFloatingNumber DebugConvertDoubleToInt(real64 F, uint8 Precision)
{
	DebugFloatingNumber _R = {};
	_R.IntegerPart = (uint64)F;
	F -= _R.IntegerPart;

	for (uint8 i = 0; i <= Precision; i++)
	{
		F *= 10;
	}

	_R.DecimalPart = (uint64)F;

	return _R;
}

static void DebugLogVector(const Vec3& V, real32 W, uint8 Precision, const char* Extra)
{
	DebugFloatingNumber _X = DebugConvertFloatToInt(V.X, Precision);
	DebugFloatingNumber _Y = DebugConvertFloatToInt(V.Y, Precision);
	DebugFloatingNumber _Z = DebugConvertFloatToInt(V.Z, Precision);
	DebugFloatingNumber _W = DebugConvertFloatToInt(W  , Precision);

	char _Buffer[256];
	wsprintf(_Buffer, "%s[%d.%d, %d.%d, %d.%d, %d.%d]\n", Extra, _X.IntegerPart, _X.DecimalPart, 
																 _Y.IntegerPart, _Y.DecimalPart, 
																 _Z.IntegerPart, _Z.DecimalPart, 
																 _W.IntegerPart, _W.DecimalPart);
	OutputDebugString(_Buffer);

	//char _tBuffer[256];
	//real64 _tr = cos(PI32);
	//DebugFloatingNumber _tt = DebugConvertDoubleToInt(_tr, 2);
	//wsprintf(_tBuffer, "%d.%d\n", _tt.IntegerPart, _tt.DecimalPart);
	//OutputDebugString(_tBuffer);
}

internal void DebugLogMatrixS4(const MatrixS4& M, uint8 Precision)
{
	real32 _00 = M[0][0];
	real32 _23 = M[2][3];

	OutputDebugString("------- START ---- DEBUG MATRIX\n");
	DebugLogVector(Vec3(M[0][0], M[0][1], M[0][2]), M[0][3], Precision, "\t");
	DebugLogVector(Vec3(M[1][0], M[1][1], M[1][2]), M[1][3], Precision, "\t");
	DebugLogVector(Vec3(M[2][0], M[2][1], M[2][2]), M[2][3], Precision, "\t");
	DebugLogVector(Vec3(M[3][0], M[3][1], M[3][2]), M[3][3], Precision, "\t");
	OutputDebugString("------- END ---- DEBUG MATRIX\n");
}

internal void DebugLogMessage(const char* Message)
{
	OutputDebugString(Message);
}




internal bool Win32LoadOpenGLFunctions(OpenGLController* OpenGLController)
{
	OpenGLController->GenVertexArrays               = (GL_GEN_VERTEX_ARRAYS          )(wglGetProcAddress("glGenVertexArrays"         ));
	if (!OpenGLController->GenVertexArrays)          return false;

	OpenGLController->DeleteVertexArrays            = (GL_DELETE_VERTEX_ARRAYS       )(wglGetProcAddress("glDeleteVertexArrays"      ));
	if (!OpenGLController->DeleteVertexArrays)       return false;

	OpenGLController->GenBuffers                    = (GL_GEN_BUFFERS                )(wglGetProcAddress("glGenBuffers"              ));
	if (!OpenGLController->GenBuffers)               return false;

	OpenGLController->DeleteBuffers                 = (GL_DELETE_BUFFERS             )(wglGetProcAddress("glDeleteBuffers"           ));
	if (!OpenGLController->DeleteBuffers)            return false;

	OpenGLController->BindVertexArray               = (GL_BIND_VERTEX_ARRAY          )(wglGetProcAddress("glBindVertexArray"         ));
	if (!OpenGLController->BindVertexArray)          return false;

	OpenGLController->BindBuffer                    = (GL_BIND_BUFFER                )(wglGetProcAddress("glBindBuffer"              ));
	if (!OpenGLController->BindBuffer)               return false;

	OpenGLController->BufferData                    = (GL_BUFFER_DATA                )(wglGetProcAddress("glBufferData"              ));
	if (!OpenGLController->BufferData)               return false;

	OpenGLController->VertexAttribPointer           = (GL_VERTEX_ATTRIB_POINTER      )(wglGetProcAddress("glVertexAttribPointer"     ));
	if (!OpenGLController->VertexAttribPointer)      return false;

	OpenGLController->EnableVertexAttribArray       = (GL_ENABLE_VERTEX_ATTRIB_ARRAY )(wglGetProcAddress("glEnableVertexAttribArray" ));
	if (!OpenGLController->EnableVertexAttribArray)  return false;

	OpenGLController->CreateShader                  = (GL_CREATE_SHADER              )(wglGetProcAddress("glCreateShader"            ));
	if (!OpenGLController->CreateShader)             return false;

	OpenGLController->CreateProgram                 = (GL_CREATE_PROGRAM             )(wglGetProcAddress("glCreateProgram"           ));
	if (!OpenGLController->CreateProgram)            return false;

	OpenGLController->DeleteProgram                 = (GL_DELETE_PROGRAM             )(wglGetProcAddress("glDeleteProgram"           ));
	if (!OpenGLController->DeleteProgram)            return false;

	OpenGLController->LinkProgram                   = (GL_LINK_PROGRAM               )(wglGetProcAddress("glLinkProgram"             ));
	if (!OpenGLController->LinkProgram)              return false;

	OpenGLController->UseProgram                    = (GL_USE_PROGRAM                )(wglGetProcAddress("glUseProgram"              ));
	if (!OpenGLController->UseProgram)               return false;

	OpenGLController->ShaderSource                  = (GL_SHADER_SOURCE              )(wglGetProcAddress("glShaderSource"            ));
	if (!OpenGLController->ShaderSource)             return false;

	OpenGLController->CompileShader                 = (GL_COMPILE_SHADER             )(wglGetProcAddress("glCompileShader"           ));
	if (!OpenGLController->CompileShader)            return false;

	OpenGLController->GetShaderiv                   = (GL_GET_SHADERIV               )(wglGetProcAddress("glGetShaderiv"             ));
	if (!OpenGLController->GetShaderiv)              return false;

	OpenGLController->GetShaderInfoLog              = (GL_GET_SHADER_INFO_LOG        )(wglGetProcAddress("glGetShaderInfoLog"        ));
	if (!OpenGLController->GetShaderInfoLog)         return false;

	OpenGLController->AttachShader                  = (GL_ATTACH_SHADER              )(wglGetProcAddress("glAttachShader"            ));
	if (!OpenGLController->AttachShader)             return false;

	OpenGLController->DeleteShader                  = (GL_DELETE_SHADER              )(wglGetProcAddress("glDeleteShader"            ));
	if (!OpenGLController->DeleteShader)             return false;

	OpenGLController->GetUniformLocation            = (GL_GET_UNIFORM_LOCATION       )(wglGetProcAddress("glGetUniformLocation"      ));
	if (!OpenGLController->GetUniformLocation)       return false;

	OpenGLController->UniformMatrix4fv              = (GL_UNIFORM_MATRIX_4FV         )(wglGetProcAddress("glUniformMatrix4fv"        ));
	if (!OpenGLController->UniformMatrix4fv)         return false;

	return true;
}



// >TOTHINK: returning WindowHandle via the function's return value rather than via a out parameter ? 
internal bool Win32_InitOpenGL(HINSTANCE Instance, WNDCLASS* Window, HWND& WindowHandle, OpenGLController* OpenGLController)
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
					BOOL  (WINAPI* _wglChoosePixelFormatARB   )(HDC, const int*, const FLOAT*, UINT, int*, UINT*) = (BOOL  (WINAPI*)(HDC, const int*, const FLOAT*, UINT, int*, UINT*))wglGetProcAddress("wglChoosePixelFormatARB");
					HGLRC (WINAPI* _wglCreateContextAttribsARB)(HDC, HGLRC, const int*                          ) = (HGLRC (WINAPI*)(HDC, HGLRC, const int*                          ))wglGetProcAddress("wglCreateContextAttribsARB");

					if (_wglChoosePixelFormatARB)
					{
						OutputDebugString("------------ YEEEEEEEEEEEEEEEEEEEEEEEEEEEEES");
					}
					else
					{
						OutputDebugString("------------ :((((((((((((((((((");
					}

					//if (OpenGL::LoadOpenGLFunctions())
					if (Win32LoadOpenGLFunctions(OpenGLController))
					{
						//OpenGL::LoadOpenGLFunctions();
						RECT _R = {};
						_R.top = 0;
						_R.bottom = WINDOW_HEIGHT_DEFAULT;
						_R.right = WINDOW_WIDTH_DEFAULT;
						_R.left = 0;

						AdjustWindowRectEx(&_R, WS_OVERLAPPEDWINDOW | WS_VISIBLE, false, 0);

						int _Width = _R.right - _R.left;
						int _Height = _R.top - _R.bottom;

						// The real window
						WindowHandle = CreateWindowEx
						(
							0,
							Window->lpszClassName,
							"Soth",
							WS_OVERLAPPEDWINDOW | WS_VISIBLE,
							CW_USEDEFAULT,
							CW_USEDEFAULT,
							_R.right - _R.left,
							_R.bottom - _R.top,
							0,
							0,
							Instance,
							0
						);
						HDC _DeviceContext = GetDC(WindowHandle);

						RECT _ClientRect = {};
						RECT _WindowRect = {};
						GetClientRect(WindowHandle, &_ClientRect);
						GetWindowRect(WindowHandle, &_WindowRect);

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
						//if (OpenGL::ChoosePixelFormatARB(_DeviceContext, _PixelAttribs, nullptr, 1, &_PixelFormatID, &_NumFormats))
						if (_wglChoosePixelFormatARB(_DeviceContext, _PixelAttribs, nullptr, 1, &_PixelFormatID, &_NumFormats))
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

							//HGLRC _RenderingContext = OpenGL::CreateContextAttribsARB(_DeviceContext, 0, _ContextAttribs);
							HGLRC _RenderingContext = _wglCreateContextAttribsARB(_DeviceContext, 0, _ContextAttribs);
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

									return true;
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
}

LRESULT CALLBACK Win32WindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
	LRESULT _Result = 0;

	switch (Message)
	{
		case WM_SIZE:
		{
			g_ClientSize.Width  = LOWORD(LParam);
			g_ClientSize.Height = HIWORD(LParam);
			glViewport(0, 0, g_ClientSize.Width, g_ClientSize.Height);
			OutputDebugString("--------- I was resized !!\n");
		} break;

		case WM_CLOSE: 
		case WM_DESTROY:
		{
			g_Running = false;
		} break;

		case WM_KEYDOWN:
		{
			uint32 _VKCode = (uint32)WParam;

			//if (_VKCode == VK_ESCAPE)
			//{
			//	g_Running = false;
			//}

			//if (_VKCode == 'D')
			//{
			//	OutputDebugString("D !\n");
			//}

		} break;

		default:
		{
			_Result = DefWindowProc(Window, Message, WParam, LParam);
		} break;
	}

	return _Result;
}

static void Win32ProcessKeyboardMessage(GameKeyState* NewKeyState, KeyState CurrentKeyState)
{
	ASSERT(NewKeyState->State != CurrentKeyState);

	NewKeyState->State = CurrentKeyState;
	NewKeyState->HalfTransitionCount++;
	OutputDebugString("Key pressed !\n");
}

static void Win32ProcessPendingMessages(GameInputController* InputController)
{
	MSG _Message;
	while (PeekMessage(&_Message, 0, 0, 0, PM_REMOVE))
	{
		switch (_Message.message)
		{
			case WM_QUIT:
			{
				g_Running = false;
			} break;
			case WM_SYSKEYDOWN :
			case WM_SYSKEYUP   :
			case WM_KEYDOWN    :
			case WM_KEYUP      :
			{
				uint32   _VKCode           = (uint32)_Message.wParam;
				KeyState _PreviousKeyState = ((_Message.lParam & (1 << 30)) != 0) ? KeyState::DOWN : KeyState::UP;
				KeyState _CurrentKeyState  = ((_Message.lParam & (1 << 31)) == 0) ? KeyState::DOWN : KeyState::UP;

				// We're not interested in key repeating itself over and over
				if (_PreviousKeyState == _CurrentKeyState) break;

				if      (_VKCode == 'A'      ) Win32ProcessKeyboardMessage(&InputController->MoveLeft , _CurrentKeyState);
				else if (_VKCode == 'D'      ) Win32ProcessKeyboardMessage(&InputController->MoveRight, _CurrentKeyState);
				else if (_VKCode == VK_ESCAPE) g_Running = false;
			} break;

			default:
			{
				TranslateMessage(&_Message);
				DispatchMessage (&_Message);
			} break;
		}
	}
}

int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CmdLine, int CmdShow)
{
	WNDCLASS _WindowClass      = {}                                 ;
	_WindowClass.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW ;
	_WindowClass.lpfnWndProc   = Win32WindowCallback                ;
	_WindowClass.hInstance     = Instance                           ;
	_WindowClass.lpszClassName = "Soth"                             ;

	g_ClientSize = {};
	g_ClientSize.Width = WINDOW_WIDTH_DEFAULT;
	g_ClientSize.Height = WINDOW_HEIGHT_DEFAULT;

	if (RegisterClass(&_WindowClass))
	{
		HWND _WindowHandle = nullptr;
		OpenGLController _OpenGLController = {};

		if (Win32_InitOpenGL(Instance, &_WindowClass, _WindowHandle, &_OpenGLController))
		{
			HDC _DC = GetDC(_WindowHandle);

			GameMemory _GameMemory = {};
			_GameMemory.PermanentStorageSize = MEGABYTES(64);
			_GameMemory.PermanentStorage = 	VirtualAlloc(0, _GameMemory.PermanentStorageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			_GameMemory.OpenGLController = &_OpenGLController;

			GameInputController  _GameInputs[2] = {};
			GameInputController* _OldGameInput  = &_GameInputs[0];
			GameInputController* _NewGameInput  = &_GameInputs[1];

			g_Running = true;

			while (g_Running)
			{
				*_NewGameInput = {}; // Reset to 0
				_NewGameInput->CopyState(_OldGameInput);

				Win32ProcessPendingMessages(_NewGameInput);

				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				UpdateGame(&_GameMemory, _NewGameInput);

				SwapBuffers(_DC);

				// Swap input controllers
				GameInputController* _Temp = _NewGameInput;
				_NewGameInput = _OldGameInput;
				_OldGameInput = _Temp;
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

	ExitGame();
	return (0);
}