#include <windows.h>
#include <gl/gl.h>
#include <stdint.h>

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

// From glext.h
//
typedef BOOL (WINAPI*   WGL_CHOOSE_PIXEL_FORMAT_ARB   ) (HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
typedef HGLRC(WINAPI*   WGL_CREATE_CONTEXT_ATTRIBS_ARB) (HDC hDC, HGLRC hShareContext, const int* attribList);
typedef void (APIENTRY* GL_GEN_VERTEX_ARRAYS          ) (GLsizei n, GLuint* arrays);
typedef void (APIENTRY* GL_GEN_BUFFERS                ) (GLsizei n, GLuint* buffers);
typedef void (APIENTRY* GL_BIND_VERTEX_ARRAY          ) (GLuint array);
typedef void (APIENTRY* GL_BIND_BUFFER                ) (GLenum target, GLuint buffer);
typedef void (APIENTRY* GL_BUFFER_DATA                ) (GLenum target, ptrdiff_t size, const void* data, GLenum usage);
typedef void (APIENTRY* GL_VERTEX_ATTRIB_POINTER      ) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
typedef void (APIENTRY* GL_ENABLE_VERTEX_ATTRIB_ARRAY ) (GLuint index);

static WGL_CHOOSE_PIXEL_FORMAT_ARB    wglChoosePixelFormatARB_    ;
static WGL_CREATE_CONTEXT_ATTRIBS_ARB wglCreateContextAttribsARB_ ;
static GL_GEN_VERTEX_ARRAYS           glGenVertexArrays_          ;
static GL_GEN_BUFFERS                 glGenBuffers_               ;
static GL_BIND_VERTEX_ARRAY           glBindVertexArray_          ;
static GL_BIND_BUFFER                 glBindBuffer_               ;
static GL_BUFFER_DATA                 glBufferData_               ;
static GL_VERTEX_ATTRIB_POINTER       glVertexAttribPointer_      ;
static GL_ENABLE_VERTEX_ATTRIB_ARRAY  glEnableVertexAttribArray_  ;




static bool Win32_LoadOpenGLFunctions()
{
	wglChoosePixelFormatARB_    = (WGL_CHOOSE_PIXEL_FORMAT_ARB   )(wglGetProcAddress("wglChoosePixelFormatARB"   ));
	if (!wglChoosePixelFormatARB_)    return false;

	wglCreateContextAttribsARB_ = (WGL_CREATE_CONTEXT_ATTRIBS_ARB)(wglGetProcAddress("wglCreateContextAttribsARB"));
	if (!wglCreateContextAttribsARB_) return false;

	glGenVertexArrays_          = (GL_GEN_VERTEX_ARRAYS          )(wglGetProcAddress("glGenVertexArrays"         ));
	if (!glGenVertexArrays_)          return false;

	glGenBuffers_               = (GL_GEN_BUFFERS                )(wglGetProcAddress("glGenBuffers"              ));
	if (!glGenBuffers_)               return false;

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

	return true;
}

// TOTHINK: returning WindowHandle via the function's return value rather than via a out parameter ? 
static bool Win32_InitOpenGL_2(HINSTANCE Instance, WNDCLASS* Window, HWND& WindowHandle)
{
	if (!Window) return false;

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
							// TOTHINK: perhaps cover v3.3 as a backup ?
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
		if (Win32_InitOpenGL_2(Instance, &_WindowClass, _WindowHandle))
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


					glClearColor(0.129f, 0.586f, 0.949f, 1.0f); // rgb(33,150,243)
					glClear(GL_COLOR_BUFFER_BIT);
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

	return (0);
}