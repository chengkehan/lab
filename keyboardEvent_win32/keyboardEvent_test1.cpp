#include <Windows.h>
#include <WindowsX.h>
#include <cstdio>

#define WND_CLASS_NAME L"myWndClassName"
#define WND_TITLE L"My Window Title"
#define WND_X 0
#define WND_Y 0
#define WND_WIDTH 800
#define WND_HEIGHT 600

HWND hwnd;
char buffer[1024];

VOID MainLoop()
{

}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
		case WM_CHAR:
		{
			// Print ascii code
			sprintf(buffer, "WM_CHAR %d\n", wparam);
			OutputDebugStringA(buffer);
			break;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
			break;
		}

		default:
		{
			break;
		}
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	WNDCLASSEX wndClass;
	memset(&wndClass, 0, sizeof(WNDCLASSEX));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.hInstance = hInstance;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszClassName = WND_CLASS_NAME;

	if(!RegisterClassEx(&wndClass))
	{
		return 0;
	}

	hwnd = CreateWindowEx(
		NULL, 
		WND_CLASS_NAME, WND_TITLE, 
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
		WND_X, WND_Y, WND_WIDTH, WND_HEIGHT, 
		NULL, NULL, hInstance, NULL);
	if(hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	MSG msg;
	while(TRUE)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		MainLoop();
	}

	return 0;
}