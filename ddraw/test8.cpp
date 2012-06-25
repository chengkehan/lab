// 32bit fullscreen software clipper 

#include <Windows.h>
#include <ddraw.h>
#include <ctime>
#include <cstdio>

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? TRUE : FALSE)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? FALSE : TRUE)
#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }

#define RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))

#define WND_CLASS_NAME L"wndClassName"
#define WND_TITLE L"DDraw Test"
#define WND_X 0
#define WND_Y 0
#define WND_WIDTH 800
#define WND_HEIGHT 600
#define SCREEN_BPP 32

HWND g_hwnd;
HINSTANCE g_hInstance;

LPDIRECTDRAW7 lpdd(NULL);
DDSURFACEDESC2 ddsd;
LPDIRECTDRAWSURFACE7 lpddsPrimary(NULL);
LPDIRECTDRAWSURFACE7 lpddsBack(NULL);

typedef struct _ICON
{
	INT x;
	INT y;
	INT vx;
	INT vy;
	UINT color;
	const CHAR* icon;
} ICON, *LPICON;

const INT NUM_ICONS = 100;
LPICON icons(NULL);

const CHAR ICON_SIZE = 8;
const CHAR ICON_1[] = {0,0,0,0,0,0,0,0,
					   0,0,1,1,1,1,0,0,
					   0,1,0,1,1,0,1,0,
					   0,1,1,1,1,1,1,0,
					   0,1,0,1,1,0,1,0,
					   0,1,1,0,0,1,1,0,
					   0,0,1,1,1,1,0,0,
					   0,0,0,0,0,0,0,0};

const CHAR ICON_2[] = {0,0,0,0,0,0,0,0,
					   0,0,1,1,1,1,0,0,
					   0,1,0,1,1,0,1,0,
					   0,1,1,1,1,1,1,0,
					   0,1,1,0,0,1,1,0,
					   0,1,0,1,1,0,1,0,
					   0,0,1,1,1,1,0,0,
					   0,0,0,0,0,0,0,0};

BOOL GameInitialize()
{
	//srand((UINT)time(NULL));

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

	icons = (LPICON)malloc(sizeof(ICON) * NUM_ICONS);
	for (int i = 0; i < NUM_ICONS; ++i)
	{
		icons[i].x = rand() % WND_WIDTH;
		icons[i].y = rand() % WND_HEIGHT;
		icons[i].vx = -1 + rand() % 2;
		icons[i].vy = -1 + rand() % 2;
		icons[i].color = RGB32BIT(0, rand() % 255, rand() % 255, rand() % 255);
		if(rand() % 10 < 5)
		{
			icons[i].icon = ICON_1;
		}
		else
		{
			icons[i].icon = ICON_2;
		}
	}

	return TRUE;
}

VOID GameRelease()
{
	if(icons != NULL)
	{
		free(icons);
		icons = NULL;
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

	DDRAW_INIT_STRUCT(ddsd);

	if(FAILED(lpddsBack->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL)))
	{
		OutputDebugString(L"backBuffer lock failed");
		return;
	}

	UINT* backBuffer = (UINT*)ddsd.lpSurface;
	UINT* backBufferTmp = backBuffer;
	INT lPitch = ddsd.lPitch >> 2;

	if(WND_WIDTH == lPitch)
	{
		memset(backBuffer, 0, WND_WIDTH * WND_HEIGHT * 4);
	}
	else
	{
		for (INT y = 0; y < WND_HEIGHT; ++y)
		{
			memset(backBufferTmp, 0, ddsd.lPitch);
			backBufferTmp += lPitch;
		}
	}

	for (INT i = 0; i < NUM_ICONS; ++i)
	{
		ICON icon = icons[i];

		if(icon.x <= -ICON_SIZE || icon.y <= -ICON_SIZE || icon.x >= WND_WIDTH || icon.y >= WND_HEIGHT)
		{
			continue;
		}

		INT ix1 = icon.x < 0 ? -icon.x : 0;
		INT iy1 = icon.y < 0 ? -icon.y : 0;
		INT ix2 = icon.x > WND_WIDTH - ICON_SIZE ? ICON_SIZE - (icon.x + ICON_SIZE - WND_WIDTH) : ICON_SIZE;
		INT iy2 = icon.y > WND_HEIGHT - ICON_SIZE ? ICON_SIZE - (icon.y + ICON_SIZE - WND_HEIGHT) : ICON_SIZE;

		INT dx = ix2 - ix1;
		INT dy = iy2 - iy1;

		backBufferTmp = backBuffer;
		backBufferTmp += icon.x + ix1 + (icon.y + iy1) * lPitch;
		for (int iy = 0; iy < dy; ++iy)
		{
			for (int ix = 0; ix < dx; ++ix)
			{
				if(icon.icon[ix1 + ix + (iy1 + iy) * ICON_SIZE] == 1)
				{
					backBufferTmp[ix + iy * lPitch] = icon.color;
				}
			}
		}
	}

	if(FAILED(lpddsBack->Unlock(NULL)))
	{
		OutputDebugString(L"backBuffer unlock failed");
		return;
	}

	lpddsPrimary->Flip(NULL, DDFLIP_WAIT);

	for (int i = 0; i < NUM_ICONS; ++i)
	{
		icons[i].x += icons[i].vx;
		icons[i].y += icons[i].vy;
		if(icons[i].x <= -ICON_SIZE)
		{
			icons[i].x = WND_WIDTH;
		}
		else if(icons[i].x >= WND_WIDTH)
		{
			icons[i].x = -ICON_SIZE;
		}
		if(icons[i].y <= -ICON_SIZE)
		{
			icons[i].y = WND_HEIGHT;
		}
		else if(icons[i].y >= WND_HEIGHT)
		{
			icons[i].y = -ICON_SIZE;
		}
	}

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