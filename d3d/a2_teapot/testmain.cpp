#include "jcd3d.h"

const INT WIN_WIDTH = 800;
const INT WIN_HEIGHT = 600;

LPDIRECT3DDEVICE9 lpd3dd = NULL;
LPD3DXMESH teapot = NULL;

BOOL jcd3d::jcd3d_setup()
{
	if(FAILED(D3DXCreateTeapot(lpd3dd, &teapot, NULL)))
	{
		return FALSE;
	}

	D3DXVECTOR3 position(0.0f, 2.0f, -3.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX v;
	D3DXMatrixLookAtLH(&v, &position, &target, &up);
	if(FAILED(lpd3dd->SetTransform(D3DTS_VIEW, &v)))
	{
		return FALSE;
	}

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * .5f, (FLOAT)WIN_WIDTH / (FLOAT)WIN_HEIGHT, 1.0f, 1000.0f);
	if(FAILED(lpd3dd->SetTransform(D3DTS_PROJECTION, &proj)))
	{
		return FALSE;
	}

	if(FAILED(lpd3dd->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME)))
	{
		return FALSE;
	}

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(lpd3dd)
	{
		D3DXMATRIX ry;
		static FLOAT y = 0.0f;
		D3DXMatrixRotationY(&ry, y);
		y += .01f;
		if(y >= D3DX_PI * 2)
		{
			y = 0.0f;
		}

		lpd3dd->SetTransform(D3DTS_WORLD, &ry);

		lpd3dd->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);
		lpd3dd->BeginScene();

		teapot->DrawSubset(0);

		lpd3dd->EndScene();
		lpd3dd->Present(NULL, NULL, NULL, NULL);
	}
}

VOID jcd3d::jcd3d_release()
{
	jcd3d::jcd3d_releaseHandle(&teapot);
	jcd3d::jcd3d_releaseHandle(&lpd3dd);
}

HRESULT CALLBACK jcd3d::jcd3d_wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	case WM_KEYDOWN:
		{
			if(wparam == VK_ESCAPE)
			{
				DestroyWindow(hwnd);
				break;
			}
		}
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d::jcd3d_init(hInstance, WIN_WIDTH, WIN_HEIGHT, true, D3DDEVTYPE_HAL, &lpd3dd))
	{
		return 0;
	}

	jcd3d::jcd3d_setup();
	jcd3d::jcd3d_loop(jcd3d::jcd3d_display);
	jcd3d::jcd3d_release();

	return 0;
}