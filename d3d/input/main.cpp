#include <Windows.h>
#include <cstdio>

#include "jcd3d.h"
#include "jcdi.h"
#include "jccommon.h"
#include "jcwin32.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace jcd3d;
using namespace jcdi;
using namespace jccommon;
using namespace jcwin32;

const INT WINDOW_WIDTH = 800;
const INT WINDOW_HEIGHT = 600;

HINSTANCE g_hInstance = NULL;

BOOL jcd3d::jcd3d_setup()
{
	//jcwin32_cursorHide();

	/*if(!jcdi_initKeyboard(g_hInstance, jcd3d_hwnd))
	{
		return FALSE;
	}
	if(!jcdi_initMouse(g_hInstance, jcd3d_hwnd))
	{
		return FALSE;
	}*/
	if(!jcdi_initInput(g_hInstance, jcd3d_hwnd))
	{
		return FALSE;
	}

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	/*jcdi_keyboardUpdate();
	jcdi_mouseUpdate();*/
	jcdi_updateInput();

	if(jcdi_keyDown(DIK_ESCAPE))
	{
		jcwin32_exit();
	}

	CHAR buffer[256];
	sprintf(buffer, "x:%d, y:%d\n", jcdi_mouseX, jcdi_mouseY);
	OutputDebugStringA(buffer);
}

VOID jcd3d::jcd3d_release()
{
	g_hInstance = NULL;
	/*jcdi_releaseKeyboard();
	jcdi_releaseMouse();*/
	jcdi_releaseInput();
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	g_hInstance = hInstance;
	if(!jcd3d_init(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE, D3DDEVTYPE_HAL))
	{
		return 0;
	}
	jcd3d_loop();

	return 0;
}