#include <Windows.h>
#include "jcd3d.h"
#include "jcdi.h"
#include "jcwin32.h"

#include "JCObject.h"
#include "JCEventDispatcher.h"
#include "JCDisplayObject.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace jcd3d;
using namespace jcdi;
using namespace jcwin32;

BOOL jcd3d::jcd3d_setup()
{
	JCObject* obj = new JCObject();
	obj->setName(L"jim\n");
	OutputDebugString(obj->getName());
	delete obj;

	if(!jcdi_initInput(jcd3d_hInstance, jcd3d_hwnd))
	{
		return FALSE;
	}

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
}

VOID jcd3d::jcd3d_release()
{
	jcdi_releaseInput();
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d_init(hInstance, 800, 600, TRUE, D3DDEVTYPE_HAL))
	{
		return 0;
	}
	jcd3d_loop();

	return 0;
}