#include <windows.h>
#include <gl/gl.h>
#include "../include/glext.h"
#include "../include/wglext.h"
#include <stdint.h>

typedef uint8_t  uint8  ;
typedef uint16_t uint16 ;
typedef uint32_t uint32 ;
typedef uint64_t uint64 ;

static bool g_Running;


#define WGL_CHOOSE_PIXEL_FORMAT_ARB(name) BOOL WINAPI name(HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormat, UINT* nNumFormats)
typedef WGL_CHOOSE_PIXEL_FORMAT_ARB(wgl_choose_pixel_format_arb);
WGL_CHOOSE_PIXEL_FORMAT_ARB(wglChoosePixelFormatArbStub)
{
	return(0);
}
static wgl_choose_pixel_format_arb* wglChoosePixelFormatARB_ = wglChoosePixelFormatArbStub;
#define wglChoosePixelFormatARB wglChoosePixelFormatARB_;


typedef BOOL (WINAPI* PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);


PFNWGLCHOOSEPIXELFORMATARBPROC toto;






/*static void Win32_LoadOpenGLFunctions()
{
	//wglChoosePixelFormatARB = wglGetProcAddress("wglChoosePixelFormatARB");
	toto = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	if (!toto)
	{
		OutputDebugString("\n\nWe've loaded the OpenGL 4.0 function !\n\n");
	}
	else
	{
		OutputDebugString("\n\nWe've failed loading the OpenGL 4.0 function... :(\n\n");
	}
}*/

/*static void Win32_InitOpenGL(HWND Window)
{
	if (!Window)
		return;
	HDC _WindowDC = GetDC(Window);

	PIXELFORMATDESCRIPTOR _DesiredPixelFormat = {};
	_DesiredPixelFormat.nSize = sizeof(_DesiredPixelFormat);
	_DesiredPixelFormat.nVersion = 1;
	_DesiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	_DesiredPixelFormat.cColorBits = 32;
	_DesiredPixelFormat.cAlphaBits = 8;

	int _SuggestedPixelFormatIndex = ChoosePixelFormat(_WindowDC, &_DesiredPixelFormat);
	PIXELFORMATDESCRIPTOR _SuggestedPixelFormat = {};
	DescribePixelFormat(_WindowDC, _SuggestedPixelFormatIndex, sizeof(_DesiredPixelFormat), &_DesiredPixelFormat);
	SetPixelFormat(_WindowDC, _SuggestedPixelFormatIndex, &_SuggestedPixelFormat);

	HGLRC _OpenGLRC = wglCreateContext(_WindowDC);
	if (wglMakeCurrent(_WindowDC, _OpenGLRC))
	{

		Win32_LoadOpenGLFunctions();

		OutputDebugString("Got it\n");
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		SwapBuffers(_WindowDC);
	}
	else
	{
		// Handle error
		OutputDebugString("Not got it\n");
	}
	ReleaseDC(Window, _WindowDC);
};*/

static bool Win32_InitOpenGL_2(HINSTANCE Instance, WNDCLASS* Window)
{
	if (!Window) return false;

	//HWND _WindowHandle = CreateWindowEx
	//(
	//	0,
	//	_WindowClass.lpszClassName,
	//	"Soth",
	//	WS_OVERLAPPEDWINDOW | WS_VISIBLE,
	//	CW_USEDEFAULT,
	//	CW_USEDEFAULT,
	//	CW_USEDEFAULT,
	//	CW_USEDEFAULT,
	//	0,
	//	0,
	//	Instance,
	//	0
	//);

	HWND _SacrificialWindow = CreateWindowEx
	(
		0, 
		"Core",
		"Sacrificial Window", 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		nullptr, 
		nullptr, 
		Instance, 
		nullptr
	);

	HDC _SacrificialDC = GetDC(_SacrificialWindow); // Handle to Device Context
	PIXELFORMATDESCRIPTOR _SacrificialPFD = {};
	_SacrificialPFD.nSize = sizeof(_SacrificialPFD);
	_SacrificialPFD.nVersion = 1;
	_SacrificialPFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
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
					PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB_test = nullptr;
					wglChoosePixelFormatARB_test = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
					if (!wglChoosePixelFormatARB_test)
					{
						OutputDebugString("\nUtterly failed to load function :(\n\n");
						return false;
					}

					PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB_test = nullptr;
					wglCreateContextAttribsARB_test = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
					if (!wglCreateContextAttribsARB_test)
					{
						OutputDebugString("\nUtterly failed to load function :(\n\n");
						return false;
					}


					// The real window
					HWND _WindowHandle = CreateWindowEx
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
					HDC _DeviceContext = GetDC(_WindowHandle);

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
						0
					};

					int _PixelFormatID;
					UINT _NumFormats;
					if (wglChoosePixelFormatARB_test(_DeviceContext, _PixelAttribs, nullptr, 1, &_PixelFormatID, &_NumFormats))
					{
						if (_NumFormats == 0) return false;

						PIXELFORMATDESCRIPTOR _PFD = {};
						DescribePixelFormat(_DeviceContext, _PixelFormatID, sizeof(_PFD), &_PFD);
						SetPixelFormat(_DeviceContext, _PixelFormatID, &_PFD);

						// Requesting OpenGL 4.0
						int _ContextAttribs[] =
						{
							WGL_CONTEXT_MAJOR_VERSION_ARB , 4 , 
							WGL_CONTEXT_MINOR_VERSION_ARB , 0 , 
							0 // Null terminate the array
						};

						HGLRC _RenderingContext = wglCreateContextAttribsARB_test(_DeviceContext, 0, _ContextAttribs);
						if (_RenderingContext)
						{
							wglMakeCurrent(nullptr, nullptr);
							wglDeleteContext(_SacrificialRC);
							ReleaseDC(_SacrificialWindow, _SacrificialDC);
							DestroyWindow(_SacrificialWindow);

							if (wglMakeCurrent(_DeviceContext, _RenderingContext))
							{
								// TODO: load OpenGL functions
								SetWindowText(_WindowHandle, (LPCSTR)glGetString(GL_VERSION));

								glClearColor(0.129f, 0.586f, 0.949f, 1.0f); // rgb(33,150,243)
								glClear(GL_COLOR_BUFFER_BIT);
								SwapBuffers(_DeviceContext);
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
		if (Win32_InitOpenGL_2(Instance, &_WindowClass))
		//{
		//	OutputDebugString("\n\nSuccess !!!!\n\n");
		//}
		//else
		//{
		//	return(0);
		//}

		//HWND _WindowHandle = CreateWindowEx
		//(
		//	0                                ,
		//	_WindowClass.lpszClassName       , 
		//	"Soth"                           ,
		//	WS_OVERLAPPEDWINDOW | WS_VISIBLE , 
		//	CW_USEDEFAULT                    , 
		//	CW_USEDEFAULT                    , 
		//	CW_USEDEFAULT                    , 
		//	CW_USEDEFAULT                    , 
		//	0                                ,
		//	0                                ,
		//	Instance                         , 
		//	0
		//);

		//if (_WindowHandle)
		{
			OutputDebugStringA("Got our window handle !\n");
			//Win32_InitOpenGL(_WindowHandle);

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

	return (0);
}