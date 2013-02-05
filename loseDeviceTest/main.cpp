#include "jcd3d.h"
#include "jcwin32.h"

using namespace jcd3d;
using namespace jcwin32;

BOOL jcd3d::jcd3d_setup()
{
	return TRUE;
}

VOID jcd3d::jcd3d_release()
{

}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(jcd3d_keyDown(VK_ESCAPE))
	{
		jcwin32_exit();
		return;
	}
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d_init(hInstance))
	{
		return 0;
	}
	jcd3d_loop();

	return 0;
}