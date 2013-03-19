#include "jcd3d.h"
#include "PatricleSys1.h"

using namespace jcd3d;
using namespace jcwin32;

ParticleSys1* lpParticleSys = NULL;
IDirect3DTexture9* lpTexture = NULL;

BOOL jcd3d::jcd3d_setup()
{
	jcd3d_setProjectionPerspectiveTransform(jcd3d_lpd3dd, 800, 600);
	jcd3d_setViewTransform(jcd3d_lpd3dd, 0.0f, 0.0f, -4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	assert(SUCCEEDED(D3DXCreateTextureFromFileA(jcd3d_lpd3dd, "smoke.bmp", &lpTexture)));
	lpParticleSys = new ParticleSys1(jcd3d_lpd3dd, lpTexture);

	return TRUE;
}

VOID jcd3d::jcd3d_release()
{
	if(lpParticleSys != NULL)
	{
		delete lpParticleSys;
		lpParticleSys = NULL;
	}
	if(lpTexture != NULL)
	{
		lpTexture->Release();
		lpTexture = NULL;
	}
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(jcd3d_keyDown(VK_ESCAPE))
	{
		jcwin32_exit();
		return;
	}

	lpParticleSys->update(0.01f);
	lpParticleSys->render();
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