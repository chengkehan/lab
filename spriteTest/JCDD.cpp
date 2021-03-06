#include "JCDD.h"

namespace JCDD_NS
{
	JCDD::JCDD():
		wndX(0), wndY(0), wndWidth(0), wndHeight(0), 
		screenBitCount(0), fullscreen(FALSE), initialized(FALSE), running(FALSE), 
		hWnd(NULL), hInstance(NULL), 
		wndClassName(NULL), wndTitle(NULL), 
		lpdd(NULL), lpddsPrimary(NULL), lpddsBackBuffer(NULL) {}

	JCDD::~JCDD()
	{
		running = false;
		hWnd = NULL;
		hInstance = NULL;
		wndClassName = NULL;
		wndTitle = NULL;
		mainLoopInvokeFunc = NULL;

		for (std::map<INT, LPJCDD_Surface>::iterator it = lpddsOffscreen.begin(); it != lpddsOffscreen.end(); ++it)
		{
			LPJCDD_Surface lpjcdds = it->second;
			LPDIRECTDRAWSURFACE7 surface = lpjcdds->getSurface();
			jcdd_release(&surface);
			jcdd_delete(&lpjcdds);
		}
		
		jcdd_release(&lpddcBackBuffer);
		jcdd_release(&lpddsBackBuffer);
		jcdd_release(&lpddsPrimary);
		jcdd_release(&lpdd);
	}

	enum JCDD_ERROR_ID JCDD::initialize(
		INT wndX, INT wndY, INT wndWidth, INT wndHeight, 
		INT nCmdshow, LPWCH wndClassName, LPWCH wndTitle, 
		HINSTANCE hInstance, BOOL fullscreen, UINT colorKey, BOOL useColorKey, UINT backColor, DWORD fps, 
		WNDPROC wndProc, MAIN_LOOP_INVOKE_FUNC mainLoopInvokeFunc)
	{
		if(initialized)
		{
			return JCDD_ERROR_ID_INITIALIZE_REPEATING;
		}
		initialized = TRUE;

		if(mainLoopInvokeFunc == NULL)
		{
			return JCDD_ERROR_ID_NULL_INPUT_VALUE;
		}

		this->wndX = wndX;
		this->wndY = wndY;
		this->wndWidth = wndWidth;
		this->wndHeight = wndHeight;
		this->colorKey = colorKey;
		this->useColorKey = useColorKey;
		this->backColor = backColor;
		this->fps = 1000 / fps;
		this->wndClassName = wndClassName;
		this->wndTitle = wndTitle;
		this->wndProc = wndProc;
		this->hInstance = hInstance;
		this->fullscreen = fullscreen;
		this->mainLoopInvokeFunc = mainLoopInvokeFunc;

		WNDCLASSEX wndClass;
		memset(&wndClass, 0, sizeof(WNDCLASSEX));
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = wndProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra	= 0;
		wndClass.hInstance	= hInstance;
		wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = wndClassName;
		if (!RegisterClassEx(&wndClass))
		{
			return JCDD_ERROR_ID_REGISTER_WND_CLASS;
		}

		DWORD dwStyle = fullscreen ? WS_POPUP | WS_VISIBLE : WS_OVERLAPPED | WS_VISIBLE;
		hWnd = CreateWindowEx(NULL, wndClassName, wndTitle, dwStyle, wndX, wndY, wndWidth, wndHeight, NULL, NULL, hInstance, NULL);
		if (hWnd == NULL)
		{
			return JCDD_ERROR_ID_CREATE_WND;
		}

		ShowWindow(hWnd, nCmdshow);
		if(!UpdateWindow(hWnd))
		{
			return JCDD_ERROR_ID_UPDATE_WND;
		}

		if(!fullscreen)
		{
			wndRect.left = 0;
			wndRect.right = wndWidth;
			wndRect.top = 0;
			wndRect.bottom = wndHeight;
			if(!AdjustWindowRectEx(&wndRect, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL, GetWindowExStyle(hWnd)))
			{
				return JCDD_ERROR_ID_ADJUST_WND;
			}
			if(!MoveWindow(hWnd, wndX, wndY, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, FALSE))
			{
				return JCDD_ERROR_ID_MOVE_WND;
			}
		}

		if(FAILED(DirectDrawCreateEx(NULL, (LPVOID*)&lpdd, IID_IDirectDraw7, NULL)))
		{
			return JCDD_ERROR_ID_CREATE_DDRAW;
		}

		DWORD dwFlags = fullscreen ? DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT : DDSCL_NORMAL;
		if(FAILED(lpdd->SetCooperativeLevel(hWnd, dwFlags)))
		{
			return JCDD_ERROR_ID_COOPERATIVE_LEVEL;
		}

		if(fullscreen)
		{
			if(FAILED(lpdd->SetDisplayMode(wndWidth, wndHeight, JCDD_SCREEN_BIT_COUNT_32, 0, 0)))
			{
				return JCDD_ERROR_ID_DISPLAY_MODE;
			}
		}

		jcdd_initStruct(&ddsd);
		if(fullscreen)
		{
			ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
			ddsd.dwBackBufferCount = 1;
			ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
		}
		else
		{
			ddsd.dwFlags = DDSD_CAPS;
			ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		}

		HRESULT cse;
		if(FAILED((cse = lpdd->CreateSurface(&ddsd, &lpddsPrimary, NULL))))
		{
			return JCDD_ERROR_ID_PRIMARY_SURFACE;
		}

		jcdd_initStruct(&ddsd);
		if(fullscreen)
		{
			ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
			if(FAILED(lpddsPrimary->GetAttachedSurface(&ddsd.ddsCaps, &lpddsBackBuffer)))
			{
				return JCDD_ERROR_ID_BACKBUFFER_SURFACE;
			}

			if(useColorKey)
			{
				DDCOLORKEY ck;
				ck.dwColorSpaceHighValue = colorKey;
				ck.dwColorSpaceLowValue = colorKey;
				if(FAILED(lpddsBackBuffer->SetColorKey(DDCKEY_SRCBLT, &ck)))
				{
					return JCDD_ERROR_ID_BACKBUFFER_SURFACE_COLOR_KEY;
				}
			}
		}
		else
		{
			DDPIXELFORMAT pixelformat;
			jcdd_initStruct(&pixelformat);
			if(FAILED(lpddsPrimary->GetPixelFormat(&pixelformat)))
			{
				return JCDD_ERROR_ID_PIXEL_FORMAT;
			}
			if(pixelformat.dwRGBBitCount != JCDD_SCREEN_BIT_COUNT_32)
			{
				return JCDD_ERROR_ID_ILLEGAL_PIXEL_FORMAT;
			}

			if(!jcdd_createOffscreenSurface(lpdd, &lpddsBackBuffer, wndWidth, wndHeight, colorKey, useColorKey))
			{
				return JCDD_ERROR_ID_BACKBUFFER_SURFACE;
			}
		}
		screenBitCount = JCDD_SCREEN_BIT_COUNT_32;

		RECT rectList[1] = {{0, 0, wndWidth, wndWidth}};
		if(!jcdd_createClipper(lpdd, lpddsBackBuffer, &lpddcBackBuffer, 1, rectList))
		{
			return JCDD_ERROR_ID_BACKBUFFER_CLIPPER;
		}

		return JCDD_ERROR_ID_NO_ERROR;
	}

	VOID JCDD::run()
	{
		running = TRUE;
		mainLoop();
	}

	VOID JCDD::stop()
	{
		running = FALSE;
	}

	VOID JCDD::mainLoop()
	{
		DWORD timePoint = GetTickCount();
		while(running)
		{
			DWORD timePoint2 = GetTickCount();
			if(timePoint2 - timePoint > fps)
			{
				timePoint = timePoint2;

				if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
				{
					if (msg.message == WM_QUIT)
					{
						break;
					}

					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				if(!jcdd_colorFillSurface(lpddsBackBuffer, wndWidth, wndHeight, backColor))
				{
					OutputDebugString(L"Fill canvas failed");
					continue;
				}

				mainLoopInvokeFunc();

				if(fullscreen)
				{
					if(FAILED(lpddsPrimary->Flip(NULL, DDFLIP_WAIT)))
					{
						OutputDebugString(L"Flip Error!!!");
					}
				}
				else
				{
					RECT clientRect;
					GetWindowRect(hWnd, &clientRect);
					RECT srcRect = {0, 0, wndWidth, wndHeight};
					INT destX = clientRect.left - wndRect.left;
					INT destY = clientRect.top - wndRect.top;
					RECT destRect = {destX, destY, destX + wndWidth, destY + wndHeight};
					DWORD bltFlags = useColorKey ? DDBLT_WAIT | DDBLT_KEYSRC : DDBLT_WAIT;
					if(FAILED(lpddsPrimary->Blt(&destRect, lpddsBackBuffer, &srcRect, bltFlags, NULL)))
					{
						OutputDebugString(L"Blt Error!!!");
					}
				}
			}
		}
	}

	HWND JCDD::getWnd() const
	{
		return hWnd;
	}

	HINSTANCE JCDD::getHInstance() const
	{
		return hInstance;
	}

	UINT JCDD::getColorKey() const
	{
		return colorKey;
	}

	BOOL JCDD::getUseColorKey() const
	{
		return useColorKey;
	}

	BOOL JCDD::createOffscreenSurface(INT surfaceID, INT width, INT height)
	{
		if(containsTheOffscreenSurface(surfaceID))
		{
			return FALSE;
		}

		LPDIRECTDRAWSURFACE7 lpdds = NULL;
		jcdd_createOffscreenSurface(lpdd, &lpdds, width, height, colorKey, useColorKey);
		lpddsOffscreen.insert(std::pair<INT, LPJCDD_Surface>(surfaceID, new JCDD_Surface(width, height, lpdds)));

		return TRUE;
	}

	VOID JCDD::deleteOffscreenSurface(INT surfaceID)
	{
		std::map<INT, LPJCDD_Surface>::iterator it = lpddsOffscreen.find(surfaceID);
		if(it != lpddsOffscreen.end())
		{
			LPJCDD_Surface lpjcdds = it->second;
			LPDIRECTDRAWSURFACE7 surface = lpjcdds->getSurface();
			jcdd_release(&surface);
			jcdd_delete(&lpjcdds);
			lpddsOffscreen.erase(surfaceID);
		}
	}

	BOOL JCDD::containsTheOffscreenSurface(INT surfaceID)
	{
		std::map<INT, LPJCDD_Surface>::iterator it = lpddsOffscreen.find(surfaceID);
		if(it == lpddsOffscreen.end())
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	LPJCDD_Surface JCDD::getOffscreenSurface(INT surfaceID)
	{
		std::map<INT, LPJCDD_Surface>::iterator it = lpddsOffscreen.find(surfaceID);
		if(it == lpddsOffscreen.end())
		{
			return NULL;
		}
		else
		{
			return it->second;
		}
	}

	LPDIRECTDRAWSURFACE7 JCDD::getBackBufferSurface()
	{
		return lpddsBackBuffer;
	}
};