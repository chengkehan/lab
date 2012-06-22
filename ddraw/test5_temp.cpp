// 8bit page flip

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

HWND g_hwnd;
HINSTANCE g_hInstance;

LPDIRECTDRAW7 lpdd(NULL);
DDSURFACEDESC2 ddsd;
LPDIRECTDRAWSURFACE7 lpddsPrimary(NULL);
LPDIRECTDRAWSURFACE7 lpddsBack(NULL);
PALETTEENTRY palette[256];
LPDIRECTDRAWPALETTE lpddpal;
DDSURFACEDESC2 ddsd;

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
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	if(FAILED(lpdd->CreateSurface(&ddsd, &lpddsPrimary, NULL)))
	{
		return FALSE;
	}
	
	ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
	if(FAILED(lpddsPrimary->GetAttachedSurface(&ddsd.ddsCaps, &lpddsBack)))
	{
		return FALSE;
	}
	
	for(int i = 1; i < 255; ++i)
	{
		palette[i].peRed = rand() % 256;
		palette[i].peGreen = rand() % 256;
		palette[i].peBlue = rand() % 256;
		palette[i].peFlags = PC_NOCOLLAPSE;
	}
	palette[0].peRed = 0;
	palette[0].peGreen = 0;
	palette[0].peBlue = 0;
	palette[0].peFlags = PC_NOCOLLAPSE;
	palette[256].peRed = 255;
	palette[256].peGreen = 255;
	palette[256].peBlue = 255;
	palette[256].peFlags = PC_NOCOLLAPSE;

	if(FAILED(lpdd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256 | DDPCAPS_INITIALIZE, palette, &lpddpal, NULL)))
	{
		return FALSE;
	}
	
	if(FAILED(lpddsPrimary->SetPalette(lpddpal)))
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
	
	DDRAW_INIT_STRUCT(ddsd);
	
	if(FAILED(lpddsBack->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL)))
	{
		return;
	}
	
	UCHAR* buffer = (UCHAR*)ddsd.lpSurface;
	
	if(ddsd.lPitch == WND_WIDTH)
	{
		memset(buffer, 0, WND_WIDTH, WND_HEIGHT);
	}
	else
	{
		UCHAR* bufferTmp = buffer;
		for(int y = 0; y < WND_HEIGHT; ++y)
		{
			memset(bufferTmp, 0, WND_WIDTH);
			bufferTmp += ddsd.lPitch;
		}
	}
	
	for(int i = 0; i < 2500; i++)
	{
		int x = rand() % WND_WIDTH;
		int y = rand() % WND_HEIGHT;
		int color = rand() % 256;
		buffer[x + y * ddsp.lPitch] = color;
	}
	
	if(FAILED(lpddsBack->Unlock(NULL)))
	{
		return;
	}
	
	lpddsPrimary->Flip(NULL, DDFLIP_WAIT);
	
	Sleep(100);
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