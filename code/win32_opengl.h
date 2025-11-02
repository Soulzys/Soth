#ifndef WIN32_OPENGL_H
#define WIN32_OPENGL_H

#include "agnostic_opengl.h"


typedef BOOL  (WINAPI* WGL_CHOOSE_PIXEL_FORMAT_ARB    ) (HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
typedef HGLRC (WINAPI* WGL_CREATE_CONTEXT_ATTRIBS_ARB ) (HDC hDC, HGLRC hShareContext, const int* attribList);


struct OpenGL_Win32
{
	WGL_CHOOSE_PIXEL_FORMAT_ARB    WglChoosePixelFormatARB    ;
	WGL_CREATE_CONTEXT_ATTRIBS_ARB WglCreateContextAttribsARB ;
};


#endif