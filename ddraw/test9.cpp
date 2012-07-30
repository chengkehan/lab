// read and show 32bit bmp image

#include <Windows.h>
#include <ddraw.h>
#include <iostream>
#include <fstream>
#include "BitmapData.h"

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? TRUE : FALSE)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? FALSE : TRUE)
#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }

#define RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))

#define WND_CLASS_NAME L"wndClassName"
#define WND_TITLE L"DDraw Test"
#define WND_X 0
#define WND_Y 0
#define WND_WIDTH 1024
#define WND_HEIGHT 768
#define SCREEN_BPP 32

using namespace std;

HWND g_hwnd;
HINSTANCE g_hInstance;

LPDIRECTDRAW7 lpdd(NULL);
DDSURFACEDESC2 ddsd;
LPDIRECTDRAWSURFACE7 lpddsPrimary(NULL);
LPDIRECTDRAWSURFACE7 lpddsBack(NULL);
LPDIRECTDRAWPALETTE lpddpal(NULL);

BitmapData* bmpd;

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

	ifstream istream;
	istream.open(L"E:\\lab_git\\ddraw\\myImage32.bmp", ifstream::in | ifstream::binary);
	if(istream.good())
	{
		istream.seekg(0, ios::end);
		INT bmpFileNumBytes = (INT)istream.tellg();
		istream.seekg(0, ios::beg);
		CHAR* bmpFileBytes = (CHAR*)malloc(sizeof(CHAR) * bmpFileNumBytes);
		if(bmpFileBytes == NULL)
		{
			return FALSE;
		}
		istream.read(bmpFileBytes, bmpFileNumBytes);

		bmpd = new BitmapData();
		bmpd->create(bmpFileBytes);

		free(bmpFileBytes);
		istream.close();

		return TRUE;
	}
	else
	{
		istream.close();
		return FALSE;
	}
}

VOID GameRelease()
{
	if(bmpd != NULL)
	{
		delete bmpd;
		bmpd = NULL;
	}

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

	DDRAW_INIT_STRUCT(ddsd);
	if(FAILED(lpddsBack->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL)))
	{
		OutputDebugString(L"attached back surface lock failed");
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

	INT x = 0;
	INT y = 0;
	BitmapDataColorIterator32Bit i32(bmpd);
	while(i32.hasNext())
	{
		BitmapDataXRGB* color = i32.next();
		backBuffer[x + lPitch * y] = RGB32BIT(0, color->r, color->g, color->b);
		if(++x == bmpd->bmpInfoHeader.biWidth)
		{
			x = 0;
			++y;
		}
	}

	if(FAILED(lpddsBack->Unlock(NULL)))
	{
		OutputDebugString(L"attached back surface unlock failed");
		return;
	}

	lpddsPrimary->Flip(NULL, DDFLIP_WAIT);

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