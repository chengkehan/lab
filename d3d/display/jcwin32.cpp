#include "jcwin32.h"

VOID jcwin32::jcwin32_exit(INT exitCode)
{
	PostQuitMessage(exitCode);
}

INT jcwin32::jcwin32_cursorShow()
{
	return ShowCursor(TRUE);
}

INT jcwin32::jcwin32_cursorHide()
{
	return ShowCursor(FALSE);
}

BOOL jcwin32::jcwin32_getWindowRect(HWND hwnd, RECT* lpRect)
{
	return GetWindowRect(hwnd, lpRect);
}

BOOL jcwin32::jcwin32_getClientRect(HWND hwnd, RECT* lpRect)
{
	return GetClientRect(hwnd, lpRect);
}

BOOL jcwin32::jcwin32_clientToScreen(HWND hwnd, POINT* lpPoint)
{
	return ClientToScreen(hwnd, lpPoint);
}

BOOL jcwin32::jcwin32_clipCursor(RECT* lpRect)
{
	return ClipCursor(lpRect);
}

BOOL jcwin32::jcwin32_getClipCursor(RECT* lpRect)
{
	return GetClipCursor(lpRect);
}