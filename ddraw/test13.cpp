// real window client pixels ddraw

#include <Windows.h>
#include <WindowsX.h>
#include <ddraw.h>

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? TRUE : FALSE)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? FALSE : TRUE)
#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }

#define RGB16BIT555(r,g,b) ((b & 31) + ((g & 31) << 5) + ((r & 31) << 10))
#define RGB16BIT565(r,g,b) ((b & 31) + ((g & 63) << 5) + ((r & 31) << 11))
#define RGB24BIT(r,g,b) ((b) + ((g) << 8) + ((r) << 16) )
#define RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))


#define WND_CLASS_NAME L"wndClassName"
#define WND_TITLE L"DDraw Test"
#define WND_X 100
#define WND_Y 100
#define WND_WIDTH 1024
#define WND_HEIGHT 768
#define SCREEN_BPP 8

HWND g_hwnd;
HINSTANCE g_hInstance;

LPDIRECTDRAW7 lpdd(NULL);
DDSURFACEDESC2 ddsd;
LPDIRECTDRAWSURFACE7 lpddsPrimary(NULL);

INT pixelFormat(0);

RECT windowRect = {0, 0, WND_WIDTH, WND_HEIGHT};

BOOL GameInitialize()
{
	AdjustWindowRectEx(&windowRect, GetWindowStyle(g_hwnd), GetMenu(g_hwnd) != NULL, GetWindowExStyle(g_hwnd));
	MoveWindow(g_hwnd, WND_X, WND_Y, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, FALSE);

	if(FAILED(DirectDrawCreateEx(NULL, (LPVOID*)&lpdd, IID_IDirectDraw7, NULL)))
	{
		return FALSE;
	}

	if(FAILED(lpdd->SetCooperativeLevel(g_hwnd, DDSCL_NORMAL)))
	{
		return FALSE;
	}

	DDRAW_INIT_STRUCT(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	if(FAILED(lpdd->CreateSurface(&ddsd, &lpddsPrimary, NULL)))
	{
		return FALSE;
	}

	DDPIXELFORMAT ddpixelformat;
	DDRAW_INIT_STRUCT(ddpixelformat);

	lpddsPrimary->GetPixelFormat(&ddpixelformat);
	pixelFormat = ddpixelformat.dwRGBBitCount;

	return TRUE;
}

VOID GameRelease()
{
	if(lpddsPrimary != NULL)
	{
		lpddsPrimary->Release();
		lpddsPrimary = NULL;
	}

	if(lpdd != NULL)
	{
		lpdd->Release();
		lpdd = NULL;
	}
}

VOID GameLoop()
{
	if(KEYDOWN(VK_ESCAPE))
	{
		SendMessage(g_hwnd, WM_CLOSE, 0, 0);
		return;
	}

	DDSURFACEDESC2 ddsd;
	RECT client;

	GetWindowRect(g_hwnd, &client);

	DDRAW_INIT_STRUCT(ddsd);
	
	if(FAILED(lpddsPrimary->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL)))
	{
		return;
	}

	UCHAR* bufferPrimary = (UCHAR*)ddsd.lpSurface;
	INT x = client.left + (-windowRect.left) + rand() % WND_WIDTH;
	INT y = client.top + (-windowRect.top) + rand() % WND_HEIGHT;

	if(pixelFormat == 32)
	{
		UINT* bufferPrimary32 = (UINT*)bufferPrimary;
		bufferPrimary32[x + y * (ddsd.lPitch >> 2)] = RGB32BIT(0, rand() % 256, rand() % 256, rand() % 256);
	}
	else if(pixelFormat == 24)
	{
		bufferPrimary[x * 3 + 0 + y * ddsd.lPitch] = rand() % 256;
		bufferPrimary[x * 3 + 1 + y * ddsd.lPitch] = rand() % 256;
		bufferPrimary[x * 3 + 2 + y * ddsd.lPitch] = rand() % 256;
	}
	else if(pixelFormat == 16)
	{
		WORD* bufferPrimary16 = (WORD*)bufferPrimary;
		bufferPrimary16[x + y * (ddsd.lPitch >> 1)] = RGB32BIT(0, rand() % 256, rand() % 256, rand() % 256);
	}
	else // pixelFormat == 8
	{
		bufferPrimary[x + y * ddsd.lPitch] = RGB32BIT(0, rand() % 256, rand() % 256, rand() % 256);
	}

	if(FAILED(lpddsPrimary->Unlock(NULL)))
	{
		return;
	}

	Sleep(2);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT	ps;
	HDC hdc;

	switch(msg)
	{	
		case WM_PAINT: 
		{
			hdc = BeginPaint(hwnd,&ps);	 
			EndPaint(hwnd,&ps);
			return 0;
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

	return (DefWindowProc(hwnd, msg, wparam, lparam));

}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevHInstance, LPSTR lpCmdline, INT nCmdshow)
{
	g_hInstance = hInstance;

	WNDCLASSEX wndClass;
	memset(&wndClass, 0, sizeof(WNDCLASSEX));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra	= 0;
	wndClass.hInstance	= hInstance;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = WND_CLASS_NAME;

	if (!RegisterClassEx(&wndClass))
	{
		return 0;
	}

	g_hwnd = CreateWindowEx(
		NULL, WND_CLASS_NAME, WND_TITLE, 
		WS_OVERLAPPED | WS_VISIBLE, 
		WND_X, WND_Y, WND_WIDTH, WND_HEIGHT, 
		NULL, NULL, hInstance, NULL
	);
	if (g_hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(g_hwnd, nCmdshow);
	UpdateWindow(g_hwnd);

	if(!GameInitialize())
	{
		return 0;
	}

	MSG msg;
	while(TRUE)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		GameLoop();
	}

	GameRelease();

	return 0;
}