#include "win32_soth.h"
#include <windows.h>
#include <gl/gl.h>
#include "win32_opengl.h"


/* TODO

	- 
 
*/

#define PI32                  3.14159265359
#define WINDOW_WIDTH_DEFAULT  1200
#define WINDOW_HEIGHT_DEFAULT 800

static Win32_ClientSize g_ClientSize;




//#include "utils.cpp"
#include "soth.cpp"
#include "shader.cpp"

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
DEBUG_PLATFORM_READ_FILE(DebugPlatformReadFile)
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

DEBUG_PLATFORM_LOG_VECTOR(DebugPlatformLogVector)
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

DEBUG_PLATFORM_LOG_MATRIXS4(DebugPlatformLogMatrixS4)
{
	real32 _00 = M[0][0];
	real32 _23 = M[2][3];

	OutputDebugString("------- START ---- DEBUG MATRIX\n");
	DebugPlatformLogVector(Vec3(M[0][0], M[0][1], M[0][2]), M[0][3], Precision, "\t");
	DebugPlatformLogVector(Vec3(M[1][0], M[1][1], M[1][2]), M[1][3], Precision, "\t");
	DebugPlatformLogVector(Vec3(M[2][0], M[2][1], M[2][2]), M[2][3], Precision, "\t");
	DebugPlatformLogVector(Vec3(M[3][0], M[3][1], M[3][2]), M[3][3], Precision, "\t");
	OutputDebugString("------- END ---- DEBUG MATRIX\n");
}

DEBUG_PLATFORM_LOG_MESSAGE(DebugPlatformLogMessage)
{
	OutputDebugString(Message);
}


struct Win32GameCodeFunctions
{
	HMODULE DLL;
	game_update* Update;
	game_exit* Exit;
	bool32 IsValid;
};

internal Win32GameCodeFunctions Win32LoadGameCode()
{
	Win32GameCodeFunctions _GameCode = {};
	_GameCode.DLL = LoadLibrary("soth.dll");
	
	if (_GameCode.DLL)
	{
		_GameCode.Update = (game_update*)GetProcAddress(_GameCode.DLL, "GameUpdate");
		_GameCode.Exit   = (game_exit*  )GetProcAddress(_GameCode.DLL, "GameExit"  );

		if (!_GameCode.Update)
		{
			OutputDebugString("NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n\n");
		}

		_GameCode.IsValid = _GameCode.Update && _GameCode.Exit;
	}

	if (!_GameCode.IsValid)
	{
		_GameCode.Update = GameUpdateStub;
		_GameCode.Exit = GameExitStub;
	}

	return _GameCode;
}


internal bool Win32LoadOpenGLFunctions(OpenGL* OpenGL)
{
	OpenGL->GenVertexArrays               = (GL_GEN_VERTEX_ARRAYS          )(wglGetProcAddress("glGenVertexArrays"         ));
	if (!OpenGL->GenVertexArrays)          return false;

	OpenGL->DeleteVertexArrays            = (GL_DELETE_VERTEX_ARRAYS       )(wglGetProcAddress("glDeleteVertexArrays"      ));
	if (!OpenGL->DeleteVertexArrays)       return false;

	OpenGL->GenBuffers                    = (GL_GEN_BUFFERS                )(wglGetProcAddress("glGenBuffers"              ));
	if (!OpenGL->GenBuffers)               return false;

	OpenGL->DeleteBuffers                 = (GL_DELETE_BUFFERS             )(wglGetProcAddress("glDeleteBuffers"           ));
	if (!OpenGL->DeleteBuffers)            return false;

	OpenGL->BindVertexArray               = (GL_BIND_VERTEX_ARRAY          )(wglGetProcAddress("glBindVertexArray"         ));
	if (!OpenGL->BindVertexArray)          return false;

	OpenGL->BindBuffer                    = (GL_BIND_BUFFER                )(wglGetProcAddress("glBindBuffer"              ));
	if (!OpenGL->BindBuffer)               return false;

	OpenGL->BufferData                    = (GL_BUFFER_DATA                )(wglGetProcAddress("glBufferData"              ));
	if (!OpenGL->BufferData)               return false;

	OpenGL->VertexAttribPointer           = (GL_VERTEX_ATTRIB_POINTER      )(wglGetProcAddress("glVertexAttribPointer"     ));
	if (!OpenGL->VertexAttribPointer)      return false;

	OpenGL->EnableVertexAttribArray       = (GL_ENABLE_VERTEX_ATTRIB_ARRAY )(wglGetProcAddress("glEnableVertexAttribArray" ));
	if (!OpenGL->EnableVertexAttribArray)  return false;

	OpenGL->CreateShader                  = (GL_CREATE_SHADER              )(wglGetProcAddress("glCreateShader"            ));
	if (!OpenGL->CreateShader)             return false;

	OpenGL->CreateProgram                 = (GL_CREATE_PROGRAM             )(wglGetProcAddress("glCreateProgram"           ));
	if (!OpenGL->CreateProgram)            return false;

	OpenGL->DeleteProgram                 = (GL_DELETE_PROGRAM             )(wglGetProcAddress("glDeleteProgram"           ));
	if (!OpenGL->DeleteProgram)            return false;

	OpenGL->LinkProgram                   = (GL_LINK_PROGRAM               )(wglGetProcAddress("glLinkProgram"             ));
	if (!OpenGL->LinkProgram)              return false;

	OpenGL->UseProgram                    = (GL_USE_PROGRAM                )(wglGetProcAddress("glUseProgram"              ));
	if (!OpenGL->UseProgram)               return false;

	OpenGL->ShaderSource                  = (GL_SHADER_SOURCE              )(wglGetProcAddress("glShaderSource"            ));
	if (!OpenGL->ShaderSource)             return false;

	OpenGL->CompileShader                 = (GL_COMPILE_SHADER             )(wglGetProcAddress("glCompileShader"           ));
	if (!OpenGL->CompileShader)            return false;

	OpenGL->GetShaderiv                   = (GL_GET_SHADERIV               )(wglGetProcAddress("glGetShaderiv"             ));
	if (!OpenGL->GetShaderiv)              return false;

	OpenGL->GetShaderInfoLog              = (GL_GET_SHADER_INFO_LOG        )(wglGetProcAddress("glGetShaderInfoLog"        ));
	if (!OpenGL->GetShaderInfoLog)         return false;

	OpenGL->AttachShader                  = (GL_ATTACH_SHADER              )(wglGetProcAddress("glAttachShader"            ));
	if (!OpenGL->AttachShader)             return false;

	OpenGL->DeleteShader                  = (GL_DELETE_SHADER              )(wglGetProcAddress("glDeleteShader"            ));
	if (!OpenGL->DeleteShader)             return false;

	OpenGL->GetUniformLocation            = (GL_GET_UNIFORM_LOCATION       )(wglGetProcAddress("glGetUniformLocation"      ));
	if (!OpenGL->GetUniformLocation)       return false;

	OpenGL->UniformMatrix4fv              = (GL_UNIFORM_MATRIX_4FV         )(wglGetProcAddress("glUniformMatrix4fv"        ));
	if (!OpenGL->UniformMatrix4fv)         return false;

	//OpenGL->DrawArrays = (GL_DRAW_ARRAYS)(wglGetProcAddress("glDrawArrays"));
	//if (!OpenGL->DrawArrays)
	//{
	//	OutputDebugString("WTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTF\n");
	//	return false;
	//}
	return true;
}



// >TOTHINK: returning WindowHandle via the function's return value rather than via a out parameter ? 
//internal bool Win32_InitOpenGL(HINSTANCE Instance, WNDCLASS* Window, HWND& WindowHandle, OpenGL* OpenGL)
internal bool Win32InitWindow(HINSTANCE Instance, WNDCLASS* Window, HWND& WindowHandle, OpenGL* OpenGL, GameScreenSize* ScreenSize)
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
					OpenGL_Win32 _openGLWin32 = {};
					_openGLWin32.WglChoosePixelFormatARB = (BOOL(WINAPI*)(HDC, const int*, const FLOAT*, UINT, int*, UINT*))wglGetProcAddress("wglChoosePixelFormatARB");
					ASSERT(_openGLWin32.WglChoosePixelFormatARB);
					_openGLWin32.WglCreateContextAttribsARB = (HGLRC(WINAPI*)(HDC, HGLRC, const int*))wglGetProcAddress("wglCreateContextAttribsARB");
					ASSERT(_openGLWin32.WglCreateContextAttribsARB);

					if (Win32LoadOpenGLFunctions(OpenGL))
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
							// >NOTE: We can pass a struct here that we can then retrieve from the main Windows callback ! 
							//        It's pretty cool to be able to do that, but I think I should come back to using global variables instead.
							//        Seems like a lot of work for Windows just go avoid global variables...
							ScreenSize 
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
						//if (_wglChoosePixelFormatARB(_DeviceContext, _PixelAttribs, nullptr, 1, &_PixelFormatID, &_NumFormats))
						if (_openGLWin32.WglChoosePixelFormatARB(_DeviceContext, _PixelAttribs, nullptr, 1, &_PixelFormatID, &_NumFormats))
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
							//HGLRC _RenderingContext = _wglCreateContextAttribsARB(_DeviceContext, 0, _ContextAttribs);
							HGLRC _RenderingContext = _openGLWin32.WglCreateContextAttribsARB(_DeviceContext, 0, _ContextAttribs);
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
	GameScreenSize* _ScreenSize = nullptr;

	// Runs only once very early when the windows is created
	if (Message == WM_NCCREATE)
	{
		CREATESTRUCT* _CreateStruct = (CREATESTRUCT*)LParam;
		_ScreenSize = (GameScreenSize*)_CreateStruct->lpCreateParams;
		SetWindowLongPtr(Window, GWLP_USERDATA, (LONG_PTR)_ScreenSize);
	}
	else
	{
		_ScreenSize = (GameScreenSize*)GetWindowLongPtr(Window, GWLP_USERDATA);
	}

	switch (Message)
	{
		//case WM_SIZE:
		//{
		//	g_ClientSize.Width  = LOWORD(LParam);
		//	g_ClientSize.Height = HIWORD(LParam);
		//	glViewport(0, 0, g_ClientSize.Width, g_ClientSize.Height);
		//	OutputDebugString("--------- I was resized !!\n");
		//} break;

		case WM_SIZE:
		{
			if (_ScreenSize)
			{
				_ScreenSize->Width = LOWORD(LParam);
				_ScreenSize->Height = HIWORD(LParam);
			}
		} break;

		case WM_CLOSE: 
		case WM_DESTROY:
		{
			g_Running = false;
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
				else if (_VKCode == VK_SPACE ) Win32ProcessKeyboardMessage(&InputController->Jump     , _CurrentKeyState);
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
	Win32GameCodeFunctions _GameCodeFunctions = Win32LoadGameCode();

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
		OpenGL _OpenGL = {};
		GameScreenSize _ScreenSize = {};

		if (Win32InitWindow(Instance, &_WindowClass, _WindowHandle, &_OpenGL, &_ScreenSize))
		{
			HDC _DC = GetDC(_WindowHandle);

			GameMemory _GameMemory               = {};
			_GameMemory.PermanentStorageSize     = MEGABYTES(64);
			_GameMemory.PermanentStorage         = VirtualAlloc(0, _GameMemory.PermanentStorageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			_GameMemory.OpenGL                   = &_OpenGL;
			_GameMemory.DebugPlatformReadFile    = DebugPlatformReadFile;
			_GameMemory.DebugPlatformLogVector   = DebugPlatformLogVector;
			_GameMemory.DebugPlatformLogMatrixS4 = DebugPlatformLogMatrixS4;
			_GameMemory.DebugPlatformLogMessage  = DebugPlatformLogMessage;

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

				//GameUpdate(&_GameMemory, _NewGameInput);
				_GameCodeFunctions.Update(&_GameMemory, _NewGameInput, &_ScreenSize);

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

	//GameExit();
	_GameCodeFunctions.Exit();
	return (0);
}