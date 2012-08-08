#include "JCDD.h"

using namespace JCDD_NS;

LPJCDD lpjcdd(NULL);

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{	
		case WM_DESTROY: 
		{
			lpjcdd->stop();
			PostQuitMessage(0);
			return 0;
			break;
		}

		default:
		{
			break;
		}

	}

	return (DefWindowProc(hwnd, msg, wparam, lparam));
}

VOID mainLoopInvokeFunc()
{
	if(jcdd_keyDown(VK_ESCAPE))
	{
		SendMessage(lpjcdd->getWnd(), WM_CLOSE, 0, 0);
		return;
	}
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	lpjcdd = new JCDD();
	if(jcdd_failed(lpjcdd->initialize(10, 10, 800, 600, nShowCmd, L"MyWnd", L"MyWndTitle", hInstance, FALSE, 0x000000, 0x0099FF, 30, windowProc, mainLoopInvokeFunc)))
	{
		return 0;
	}
	lpjcdd->run();
	jcdd_delete(&lpjcdd);

	return 0;
}