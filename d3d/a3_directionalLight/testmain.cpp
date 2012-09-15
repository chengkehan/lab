#include "jcd3d.h"

const INT WIN_WIDTH = 800;
const INT WIN_HEIGHT = 600;

LPDIRECT3DDEVICE9 lpd3dd = NULL;
LPD3DXMESH teapot;
D3DXMATRIX teapotPos;
D3DMATERIAL9 teapotMrl;

BOOL jcd3d::jcd3d_setup()
{
	if(FAILED(D3DXCreateTeapot(lpd3dd, &teapot, NULL)))
	{
		return FALSE;
	}

	D3DXMatrixTranslation(&teapotPos, 0.0f, 0.0f, 0.0f);
	jcd3d::jcd3d_initMaterial(
		&teapotMrl, 
		D3DXCOLOR(D3DCOLOR_XRGB(0xFF, 0x00, 0x00)), 
		D3DXCOLOR(D3DCOLOR_XRGB(0xFF, 0x00, 0x00)), 
		D3DXCOLOR(D3DCOLOR_XRGB(0xFF, 0x00, 0x00)), 
		D3DXCOLOR(D3DCOLOR_XRGB(0x00, 0x00, 0x00)), 
		2.0f
	);

	D3DXVECTOR3 lightDir(-5.0f, -5.0f, -5.0f);
	D3DXCOLOR lightColor = D3DXCOLOR(D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
	D3DLIGHT9 light;
	jcd3d::jcd3d_initDirectionalLight(&light, &lightDir, &lightColor);
	if(FAILED(lpd3dd->SetLight(0, &light)))
	{
		return FALSE;
	}
	if(FAILED(lpd3dd->LightEnable(0, TRUE)))
	{
		return FALSE;
	}

	if(FAILED(lpd3dd->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE)))
	{
		return FALSE;
	}
	if(FAILED(lpd3dd->SetRenderState(D3DRS_SPECULARENABLE, FALSE)))
	{
		return FALSE;
	}

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5, (FLOAT)WIN_WIDTH / (FLOAT)WIN_HEIGHT, 1.0f, 1000.0f);
	if(FAILED(lpd3dd->SetTransform(D3DTS_PROJECTION, &proj)))
	{
		return FALSE;
	}

	if(FAILED(lpd3dd->SetRenderState(D3DRS_LIGHTING, TRUE)))
	{
		return FALSE;
	}

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(lpd3dd)
	{
		static FLOAT r = 3.0f;
		static FLOAT angle = 0.0f;
		FLOAT px = cosf(angle) * r;
		FLOAT pz = sinf(angle) * r;
		angle += .01f;
		if(angle >= D3DX_PI * 2)
		{
			angle = 0;
		}

		D3DXVECTOR3 pos(px, 2.0f, pz);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX v;
		D3DXMatrixLookAtLH(&v, &pos, &target, &up);
		lpd3dd->SetTransform(D3DTS_VIEW, &v);

		lpd3dd->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		lpd3dd->BeginScene();

		lpd3dd->SetMaterial(&teapotMrl);
		lpd3dd->SetTransform(D3DTS_WORLD, &teapotPos);
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

	jcd3d::jcd3d_loop(jcd3d::jcd3d_display);
	jcd3d::jcd3d_release();

	return 0;
}