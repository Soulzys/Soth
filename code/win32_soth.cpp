#include <windows.h>
#include <gl/gl.h>
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






static void Win32_LoadOpenGLFunctions()
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
}

static void Win32_InitOpenGL(HWND Window)
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
};

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
		HWND _WindowHandle = CreateWindowEx
		(
			0                                ,
			_WindowClass.lpszClassName       , 
			"Soth"                           ,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE , 
			CW_USEDEFAULT                    , 
			CW_USEDEFAULT                    , 
			CW_USEDEFAULT                    , 
			CW_USEDEFAULT                    , 
			0                                ,
			0                                ,
			Instance                         , 
			0
		);

		if (_WindowHandle)
		{
			OutputDebugStringA("Got our window handle !\n");
			Win32_InitOpenGL(_WindowHandle);

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