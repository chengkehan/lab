#ifndef __JC_2D_H__
#define __JC_2D_H__

#include <Windows.h>

#include "JCD3D.h"
#include "JCDI.h"
#include "JCDisplayObject.h"
#include "JCDisplayObjectContainer.h"
#include "JCTextureManager.h"
#include "jccommon.h"
#include "jcSingleton.h"
#include "jcwin32.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class JC2D
{
JC_SINGLETON_DECLARE(JC2D)

public:
	BOOL init(HINSTANCE hInstance, JCD3D::SETUPCALLBACK setupCallback = NULL, JCD3D::RELEASECALLBACK releaseCallback = NULL, JCD3D::FRAMECALLBACK frameCallback = NULL, 
		INT windowX = 0, INT windowY = 0, INT windowWidth = 800, INT windowHeight = 600, BOOL windowd = TRUE);
	JCD3D* getJCD3D();
	JCDI* getJCDI();
	JCTextureManager* getTextureManager();
	JCDisplayObjectContainer* getStage();
	VOID run();
	VOID setExitWhileEscapeDown(BOOL value);
	BOOL getExitWhileEscapeDown();
	VOID setMouseVisible(BOOL value);
	BOOL getMouseVisible();
	VOID setMouseLockOnWindow(BOOL value);
	BOOL getMouseLockOnWindow();

private:
	JC2D();
	~JC2D();

	static VOID jc2dFrameCallback(DWORD timeDelta);
	static VOID jc2dMouseLockOnWindowProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

	JCD3D m_jc3d;
	JCDI m_jcdi;
	JCTextureManager m_textureManager;
	JCDisplayObjectContainer* m_stage;
	JCD3D::FRAMECALLBACK m_frameCallback;
	JCD3D::SETUPCALLBACK m_setupCallback;
	JCD3D::RELEASECALLBACK m_releaseCallback;
	BOOL m_exitWhileEscapeDown;
	BOOL m_mouseVisible;

};

#endif