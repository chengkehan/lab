// 8bit	palette

#include <cstdio>
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
PALETTEENTRY palette[256];
LPDIRECTDRAWPALETTE lpddpal;

CHAR printBuffer[1024];

const INT RECT_SIZE(8);
CHAR rectGraphics[RECT_SIZE][RECT_SIZE] = {
	{1, 1, 1, 0, 0, 1, 1, 1}, 
	{1, 1, 1, 0, 0, 1, 1, 1}, 
	{1, 1, 1, 0, 0, 1, 1, 1}, 
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0}, 
	{1, 1, 1, 0, 0, 1, 1, 1}, 
	{1, 1, 1, 0, 0, 1, 1, 1}, 
	{1, 1, 1, 0, 0, 1, 1, 1}
};
int rectX(0);
int rectY(0);
int rectVX(2);
int rectVY(2);

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

	for (int i = 1; i < 255; ++i)
	{
		palette[i].peRed = rand() & 256;
		palette[i].peGreen = rand() & 256;
		palette[i].peBlue = rand() & 256;
		palette[i].peFlags = PC_NOCOLLAPSE;
	}
	palette[0].peRed = 0;
	palette[0].peGreen = 0;
	palette[0].peBlue = 0;
	palette[0].peFlags = PC_NOCOLLAPSE;
	palette[255].peRed = 255;
	palette[255].peGreen = 255;
	palette[255].peBlue = 255;
	palette[255].peFlags = PC_NOCOLLAPSE;

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
	if(FAILED(lpddsPrimary->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL)))
	{
		OutputDebugString(L"error lock\n");
		return;
	}

	int pitch = (int)ddsd.lPitch;
	UCHAR* buffer = (UCHAR*)ddsd.lpSurface;
	memset(buffer, 0, pitch * WND_HEIGHT);
	if(rectX < 0)
	{
		rectX = 0;
		rectVX *= -1;
	}
	if(rectX + RECT_SIZE > WND_WIDTH)
	{
		rectX = WND_WIDTH - RECT_SIZE;
		rectVX *= -1;
	}
	if(rectY < 0)
	{
		rectY = 0;
		rectVY *= -1;
	}
	if(rectY + RECT_SIZE > WND_HEIGHT)
	{
		rectY = WND_HEIGHT - RECT_SIZE;
		rectVY *= -1;
	}
	for (int row = 0; row < RECT_SIZE; ++row)
	{
		for (int col = 0; col < RECT_SIZE; ++col)
		{
			if(rectGraphics[row][col] != 0)
			{
				UCHAR color = 255;
				int x = rectX + col;
				int y = rectY + row;
				buffer[x + y * pitch] = color;
			}
			
		}
	}
	rectX += rectVX;
	rectY += rectVY;

	if(FAILED(lpddsPrimary->Unlock(NULL)))
	{
		OutputDebugString(L"error unlock\n");
		return;
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