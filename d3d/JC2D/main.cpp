#include <Windows.h>
#include <fstream>
#include "jcd3d.h"
#include "jcdi.h"
#include "jcwin32.h"
#include "JCDisplayObjectContainer.h"
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
JCDisplayObjectContainer* lpContainer = NULL;
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
		reader.close();
	}
	else
	{
		jcwin32_messageBoxErrorM("Read texture file failed");
		reader.close();
		return FALSE;
	}
	

	lpContainer = new JCDisplayObjectContainer(jcd3d_lpd3dd);

	lpCursor = new JCDisplayObject(jcd3d_lpd3dd);
	lpCursor->setTexture(lpTexture);
	//lpCursor->setAlphaEnabled(TRUE);
	//lpCursor->setAlpha(0.5f);
	lpCursor->setRefX(lpCursor->getWidth() * 0.5f);
	lpCursor->setRefY(lpCursor->getHeight() * 0.5f);

	lpContainer->addChild(lpCursor);

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

	//lpCursor->setX((FLOAT)jcdi_mouseX);
	//lpCursor->setY((FLOAT)jcdi_mouseY);
	lpContainer->setX((FLOAT)jcdi_mouseX);
	lpContainer->setY((FLOAT)jcdi_mouseY);
	if(jcdi_mouseLeftButtonDown)
	{
		lpCursor->setRotation(lpCursor->getRotation() + 0.1f);
		lpContainer->setRotation(lpContainer->getRotation() - 0.05f);

		static FLOAT scaleA = 1.0f;
		if(lpContainer->getScaleX() < 1.0f || lpContainer->getScaleX() > 3.0f)
		{
			scaleA *= -1.0f;
		}
		lpContainer->setScaleX(lpContainer->getScaleX() + 0.05f * scaleA);
		lpContainer->setScaleY(lpContainer->getScaleY() + 0.05f * scaleA);
	}
	lpContainer->render();
}

VOID jcd3d::jcd3d_release()
{
	jccommon_deleteM(lpCursor);
	jccommon_deleteM(lpContainer);
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