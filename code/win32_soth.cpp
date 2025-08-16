#include <windows.h>
#include <gl/gl.h>

static void Win32_InitOpenGL(HWND Window)
{
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
	}
	else
	{
		// Handle error
	}
	ReleaseDC(Window, _WindowDC);
};

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MessageBox(0, "This is my new game", "Soth", MB_OK);

	Win32_InitOpenGL();
	return (0);
}