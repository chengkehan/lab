#ifndef __JC_WIN32__
#define __JC_WIN32__

#include <Windows.h>

#define jcwin32_messageBoxErrorM(msg) { MessageBox(NULL, L##msg, L"Error", MB_OK); }

namespace jcwin32
{
	VOID jcwin32_exit(INT exitCode = 0);

	INT jcwin32_cursorShow();
	INT jcwin32_cursorHide();

	BOOL jcwin32_getWindowRect(HWND hwnd, RECT* lpRect);
	BOOL jcwin32_getClientRect(HWND hwnd, RECT* lpRect);

	BOOL jcwin32_clientToScreen(HWND hwnd, POINT* lpPoint);

	BOOL jcwin32_clipCursor(RECT* lpRect);
	BOOL jcwin32_getClipCursor(RECT* lpRect);
}

#endif