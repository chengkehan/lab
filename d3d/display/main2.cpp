#include <Windows.h>
#include <fstream>
#include "jcd3d.h"
#include "jcdi.h"
#include "jcwin32.h"
#include "JCDisplayObject.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace std;
using namespace jcd3d;
using namespace jcdi;
using namespace jcwin32;

IDirect3DTexture9* lpTexture = NULL;
JCDisplayObject* lpCursor = NULL;

VOID windowMoveCallback()
{
	jcdi_mouseLockOnWindow();
}

BOOL jcd3d::jcd3d_setup()
{
	ifstream reader;
	reader.open(L"cursor.png", ios::ios_base::in | ios::ios_base::binary);
	if(reader.good())
	{
		reader.seekg(0, ios::end);
		int bytesAvailable = (INT)reader.tellg();
		reader.seekg(0, ios::beg);

		char* imgBuffer = new char[bytesAvailable];
		reader.read(imgBuffer, bytesAvailable);
		if(FAILED(D3DXCreateTextureFromFileInMemory(jcd3d_lpd3dd, imgBuffer, bytesAvailable, &lpTexture)))
		{
			jcwin32_messageBoxErrorM("CreateTexture Failed");
		}
		delete[] imgBuffer;
	}
	reader.close();

	lpCursor = new JCDisplayObject(jcd3d_lpd3dd);
	lpCursor->setTexture(lpTexture);
	//lpCursor->setAlphaEnabled(TRUE);
	lpCursor->setWidth(100);
	lpCursor->setHeight(100);
	lpCursor->setRefXY(lpCursor->getWidth() * 0.5f, lpCursor->getHeight() * 0.5f);

	if(!jcdi_initInput(jcd3d_hInstance, jcd3d_hwnd))
	{
		return FALSE;
	}
	jcdi_mouseSpeed = 1.5f;
	jcwin32_cursorHide();

	jcd3d_windowMoveCallback = windowMoveCallback;
	windowMoveCallback();

	jcd3d_setProjectionPerspectiveTransform(jcd3d_lpd3dd, jcd3d_windowWidth, jcd3d_windowHeight);
	jcd3d_setViewTransform(jcd3d_lpd3dd, 0.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	jcd3d_initRenderState(jcd3d_lpd3dd, D3DCULL_CCW, FALSE, TRUE, D3DSHADE_GOURAUD, D3DFILL_SOLID, FALSE);

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	jcdi_updateInput();
	if(jcdi_keyDown(DIK_ESCAPE))
	{
		jcwin32_exit();
		return;
	}

	lpCursor->setXY((FLOAT)jcdi_mouseX, (FLOAT)jcdi_mouseY);
	lpCursor->setRotation(lpCursor->getRotation() + 0.1f);
	lpCursor->renderUpdate(FALSE, TRUE);
	lpCursor->render();
}

VOID jcd3d::jcd3d_release()
{
	jccommon_deleteM(lpCursor);
	jccommon_releaseComM(lpTexture);
	jcdi_releaseInput();
	jcwin32_cursorShow();
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d_init(hInstance, 100, 200))
	{
		return 0;
	}
	jcd3d_loop();

	return 0;
}