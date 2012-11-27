#include <Windows.h>

#include "JCD3D.h"
#include "JCDI.h"
#include "JCTextureManager.h"
#include "JCDisplayObject.h"
#include "jccommon.h"
#include "jcwin32.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace jcwin32;

JCD3D jc3d;
JCDI jcdi;
JCTextureManager textureManager;
CONST INT TEXTURE_ID_CURSOR = 0;
JCDisplayObject* lpCursor;

VOID gameWindowModeCallback(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	jcdi.mouseLockOnWindow();
}

BOOL gameSetup()
{
	if(!jcdi.initInput(jc3d.getHInstance(), jc3d.getHWnd()))
	{
		return FALSE;
	}

	if(!textureManager.loadFileTexture(TEXTURE_ID_CURSOR, L"cursor.png", jc3d.getDirect3DDevice()))
	{
		return FALSE;
	}

	jc3d.setMessageCallback(WM_MOVE, gameWindowModeCallback);
	gameWindowModeCallback(NULL, 0, 0, 0);
	jcwin32_cursorHide();

	lpCursor = new JCDisplayObject(jc3d.getDirect3DDevice());
	lpCursor->setTexture(textureManager.getTexture(TEXTURE_ID_CURSOR));

	return TRUE;
}

VOID gameRelease()
{
	jccommon_deleteM(lpCursor);
}

VOID gameFrame(DWORD timeDelta)
{
	jcdi.updateInput();

	if(jcdi.keyDown(DIK_ESCAPE))
	{
		PostQuitMessage(0);
		return;
	}

	lpCursor->setX((FLOAT)jcdi.getMouseX());
	lpCursor->setY((FLOAT)jcdi.getMouseY());
	lpCursor->render();
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	jc3d.setup = gameSetup;
	jc3d.release = gameRelease;
	jc3d.frame = gameFrame;
	if(!jc3d.init(hInstance))
	{
		return 0;
	}
	jc3d.run();

	return 0;
}