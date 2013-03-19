#include "d3dUtility.h"
#include "camera.h"
#include "ParticleSystem.h"
#include <cstdlib>
#include <ctime>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")

#pragma warning(disable : 4100)

const int WIDTH  = 1300;
const int HEIGHT = 900;

IDirect3DDevice9*	g_device;
cParticleSystem*	g_snow;
cCamera				g_camera(AIR_CRAFT);

////////////////////////////////////////////

BOOL jcd3d_setProjectionPerspectiveTransform(LPDIRECT3DDEVICE9 lpd3dd, INT windowWidth, INT windowHeight)
{
	if(lpd3dd == NULL)
	{
		return NULL;
	}
	else
	{
		D3DXMATRIX out;
		D3DXMatrixPerspectiveFovLH(&out, D3DX_PI * 0.5f, (FLOAT)windowWidth / (FLOAT)windowHeight, 1.0f, 1000.0f);
		if(FAILED(lpd3dd->SetTransform(D3DTS_PROJECTION, &out)))
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
}

BOOL jcd3d_setProjectionOrthoTransform(LPDIRECT3DDEVICE9 lpd3dd, INT windowWidth, INT windowHeight)
{
	if(lpd3dd == NULL)
	{
		return NULL;
	}
	else
	{
		D3DXMATRIX out;
		D3DXMatrixOrthoLH(&out, (FLOAT)windowWidth, (FLOAT)windowHeight, 1.0f, 1000.0f);
		if(FAILED(lpd3dd->SetTransform(D3DTS_PROJECTION, &out)))
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
}

BOOL jcd3d_setViewTransform(LPDIRECT3DDEVICE9 lpd3dd, D3DXVECTOR3* lpeye, D3DXVECTOR3* lptarget, D3DXVECTOR3* lpup)
{
	if(lpd3dd == NULL || lpeye == NULL || lptarget == NULL || lpup == NULL)
	{
		return FALSE;
	}
	else
	{
		D3DXMATRIX out;
		D3DXMatrixLookAtLH(&out, lpeye, lptarget, lpup);
		if(FAILED(lpd3dd->SetTransform(D3DTS_VIEW, &out)))
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
}
BOOL jcd3d_setViewTransform(LPDIRECT3DDEVICE9 lpd3dd, FLOAT eyeX, FLOAT eyeY, FLOAT eyeZ, FLOAT targetX, FLOAT targetY, FLOAT targetZ, FLOAT upX, FLOAT upY, FLOAT upZ)
{
	if(lpd3dd == NULL)
	{
		return FALSE;
	}
	else
	{
		D3DXMATRIX out;
		D3DXVECTOR3 eye(eyeX, eyeY, eyeZ);
		D3DXVECTOR3 target(targetX, targetY, targetZ);
		D3DXVECTOR3 up(upX, upY, upZ);
		D3DXMatrixLookAtLH(&out, &eye, &target, &up);
		if(FAILED(lpd3dd->SetTransform(D3DTS_VIEW, &out)))
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool setup()
{	
	srand((unsigned int)time(NULL));

	// create snow system

	cBoundingBox bounding_box;
	bounding_box.m_min = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);
	bounding_box.m_max = D3DXVECTOR3(10.0f, 10.0f, 10.0f);

	g_snow = new cSnow(&bounding_box, 1);
	g_snow->init(g_device, "logo.jpg");
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void cleanup()
{	
	delete g_snow;

	// pass NULL for the first parameter to instruct cleanup
	draw_basic_scene(NULL, 0.0f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

bool display(float time_delta)
{
	jcd3d_setProjectionPerspectiveTransform(g_device, WIDTH, HEIGHT);
	jcd3d_setViewTransform(g_device, 0.0f, 0.0f, -3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	g_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
	g_device->BeginScene();
	g_snow->update(time_delta);
	g_snow->render();
	g_device->EndScene();
	g_device->Present(NULL, NULL, NULL, NULL);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM word_param, LPARAM long_param)
{
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if(word_param == VK_ESCAPE)
			DestroyWindow(hwnd);
		break;
	}

	return DefWindowProc(hwnd, msg, word_param, long_param);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE inst, HINSTANCE, PSTR cmd_line, int cmd_show)
{
	if(! init_d3d(inst, WIDTH, HEIGHT, true, D3DDEVTYPE_HAL, &g_device))
	{
		MessageBox(NULL, L"init_d3d() - failed.", 0, MB_OK);
		return 0;
	}

	if(! setup())
	{
		MessageBox(NULL, L"Steup() - failed.", 0, MB_OK);
		return 0;
	}

	enter_msg_loop(display);

	cleanup();
	g_device->Release();

	return 0;
}

