#ifndef __JCDD_H__
#define __JCDD_H__

#include <Windows.h>
#include <WindowsX.h>
#include <ddraw.h>
#include <map>
#include "JCDD_Util.h"

namespace JCDD_NS
{
	typedef VOID(*MAIN_LOOP_INVOKE_FUNC)(VOID);
	
	typedef class JCDD
	{
	public:
		JCDD();
		~JCDD();

	public:
		enum JCDD_ERROR_ID initialize(
			INT wndX, INT wndY, INT wndWidth, INT wndHeight, 
			INT nCmdshow, LPWCH wndClassName, LPWCH wndTitle, 
			HINSTANCE hInstance, BOOL fullscreen, UINT colorKey, UINT backColor, DWORD fps, 
			WNDPROC wndProc, MAIN_LOOP_INVOKE_FUNC mainLoopInvokeFunc);

		VOID run();
		VOID stop();

		HWND getWnd() const;
		HINSTANCE getHInstance() const;

		BOOL createOffscreenSurface(INT surfaceID, INT width, INT height, UINT colorKey);
		VOID deleteOffscreenSurface(INT surfaceID);
		BOOL containsTheOffscreenSurface(INT surfaceID);
		LPJCDD_Surface getOffscreenSurface(INT surfaceID);

		LPDIRECTDRAWSURFACE7 getBackBufferSurface();

	private:
		INT wndWidth;
		INT wndHeight;
		INT wndX;
		INT wndY;
		INT nCmdshow;
		INT screenBitCount;
		DWORD fps;
		UINT colorKey;
		UINT backColor;
		BOOL fullscreen;
		BOOL initialized;
		BOOL running;
		RECT wndRect;
		WNDPROC wndProc;
		MAIN_LOOP_INVOKE_FUNC mainLoopInvokeFunc;
		MSG msg;
		HWND hWnd;
		HINSTANCE hInstance;
		LPWCH wndClassName;
		LPWCH wndTitle;
		LPDIRECTDRAW7 lpdd;
		LPDIRECTDRAWSURFACE7 lpddsPrimary;
		LPDIRECTDRAWSURFACE7 lpddsBackBuffer;
		LPDIRECTDRAWCLIPPER lpddcBackBuffer;
		std::map<INT, LPJCDD_Surface> lpddsOffscreen;
		DDSURFACEDESC2 ddsd;

	private:
		VOID mainLoop();
	} *LPJCDD, *IJCDD;
};

#endif