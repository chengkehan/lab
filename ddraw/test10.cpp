// clipper

#include <Windows.h>
#include <ddraw.h>

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? TRUE : FALSE)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? FALSE : TRUE)
#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }

#define WND_CLASS_NAME L"wndClassName"
#define WND_TITLE L"DDraw Test"
#define WND_X 0
#define WND_Y 0
#define WND_WIDTH 1024
#define WND_HEIGHT 768
#define SCREEN_BPP 8
#define MAX_NUM_COLORS 256

HWND g_hwnd;
HINSTANCE g_hInstance;

LPDIRECTDRAW7 lpdd(NULL);
DDSURFACEDESC2 ddsd;
LPDIRECTDRAWSURFACE7 lpddsPrimary(NULL);
LPDIRECTDRAWSURFACE7 lpddsBack(NULL);
PALETTEENTRY palette[MAX_NUM_COLORS];
LPDIRECTDRAWPALETTE lpddpal;
LPDIRECTDRAWCLIPPER lpddc;

LPDIRECTDRAWCLIPPER GetAttackedClipper(LPDIRECTDRAWSURFACE7 lpdds, INT numRect, LPRECT rectList)
{
	LPDIRECTDRAWCLIPPER lpddc;
	LPRGNDATA regionData;

	if(FAILED(lpdd->CreateClipper(0, &lpddc, NULL)))
	{
		return NULL;
	}

	regionData = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER) + numRect * sizeof(RECT));
	memcpy(regionData->Buffer, rectList, sizeof(RECT) * numRect);

	regionData->rdh.dwSize = sizeof(RGNDATAHEADER);
	regionData->rdh.iType = RDH_RECTANGLES;
	regionData->rdh.nCount = numRect;
	regionData->rdh.nRgnSize = sizeof(RECT) * numRect;
	regionData->rdh.rcBound.left = LONG_MAX;
	regionData->rdh.rcBound.top = LONG_MAX;
	regionData->rdh.rcBound.right = LONG_MIN;
	regionData->rdh.rcBound.bottom = LONG_MIN;

	for (INT i = 0; i < numRect; ++i)
	{
		if(rectList[i].left < regionData->rdh.rcBound.left)
		{
			regionData->rdh.rcBound.left = rectList[i].left;
		}
		if(rectList[i].top < regionData->rdh.rcBound.top)
		{
			regionData->rdh.rcBound.top = rectList[i].top;
		}
		if(rectList[i].right > regionData->rdh.rcBound.right)
		{
			regionData->rdh.rcBound.right = rectList[i].right;
		}
		if(rectList[i].bottom > regionData->rdh.rcBound.bottom)
		{
			regionData->rdh.rcBound.bottom = rectList[i].bottom;
		}
	}

	if(FAILED(lpddc->SetClipList(regionData, 0)))
	{
		free(regionData);
		return NULL;
	}

	if(FAILED(lpdds->SetClipper(lpddc)))
	{
		free(regionData);
		return NULL;
	}

	free(regionData);
	return lpddc;
}

BOOL GameInitialize()
{
	if(FAILED(DirectDrawCreateEx(NULL, (LPVOID*)&lpdd, IID_IDirectDraw7, NULL)))
	{
		return FALSE;
	}

	if(FAILED(lpdd->SetCooperativeLevel(g_hwnd, DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT)))
	{
		return FALSE;
	}

	if(FAILED(lpdd->SetDisplayMode(WND_WIDTH, WND_HEIGHT, SCREEN_BPP, 0, 0)))
	{
		return FALSE;
	}

	DDRAW_INIT_STRUCT(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.dwBackBufferCount = 1;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;

	if(FAILED(lpdd->CreateSurface(&ddsd, &lpddsPrimary, NULL)))
	{
		return FALSE;
	}

	ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;

	if(FAILED(lpddsPrimary->GetAttachedSurface(&ddsd.ddsCaps, &lpddsBack)))
	{
		return FALSE;
	}

	for (INT i = 0; i < MAX_NUM_COLORS; ++i)
	{
		palette[i].peRed = rand() % 0xFF;
		palette[i].peGreen = rand() & 0xFF;
		palette[i].peBlue = rand() & 0xFF;
		palette[i].peFlags = PC_NOCOLLAPSE;
	}

	if(FAILED(lpdd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256 | DDPCAPS_INITIALIZE, palette, &lpddpal, NULL)))
	{
		return FALSE;
	}

	if(FAILED(lpddsPrimary->SetPalette(lpddpal)))
	{
		return FALSE;
	}

	DDRAW_INIT_STRUCT(ddsd);
	if(FAILED(lpddsBack->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL)))
	{
		return FALSE;
	}
	
	UCHAR* backBuffer = (UCHAR*)ddsd.lpSurface;
	for (INT y = 0; y < WND_HEIGHT; ++y)
	{
		for (INT x = 0; x < WND_WIDTH; ++x)
		{
			backBuffer[y * ddsd.lPitch + x] = rand() % MAX_NUM_COLORS;
		}
	}

	if(FAILED(lpddsBack->Unlock(NULL)))
	{
		return FALSE;
	}

	RECT rectList[5] = {
		{100, 100, 200, 200}, 
		{200, 200, 300, 300}, 
		{300, 300, 400, 400}, 
		{400, 400, 500, 500}, 
		{500, 500, 600, 600}
	};
	lpddc = GetAttackedClipper(lpddsPrimary, 5, rectList);
	if(lpddc == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

VOID GameRelease()
{
	if(lpddpal != NULL)
	{
		lpddpal->Release();
		lpddpal = NULL;
	}

	if(lpddsBack != NULL)
	{
		lpddsBack->Release();
		lpddsBack = NULL;
	}

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

	RECT src;
	RECT dest;

	src.left = rand() % WND_WIDTH;
	src.right = rand() % WND_WIDTH;
	src.top = rand() % WND_HEIGHT;
	src.bottom = rand() % WND_HEIGHT;

	dest.left = rand() % WND_WIDTH;
	dest.right = rand() % WND_WIDTH;
	dest.top = rand() % WND_HEIGHT;
	dest.bottom = rand() % WND_HEIGHT;

	lpddsPrimary->Blt(&dest, lpddsBack, &src, DDBLT_WAIT, NULL);

	Sleep(30);
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

	g_hwnd = CreateWindowEx(NULL, WND_CLASS_NAME, WND_TITLE, WS_POPUP | WS_VISIBLE, WND_X, WND_Y, WND_WIDTH, WND_HEIGHT, NULL, NULL, hInstance, NULL);
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