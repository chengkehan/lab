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