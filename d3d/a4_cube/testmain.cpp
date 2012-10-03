#include "jcd3d.h"
#include "cube.h"

const INT WIN_WIDTH = 800;
const INT WIN_HEIGHT = 600;

LPDIRECT3DDEVICE9 lpd3dd = NULL;
Cube* cube = NULL;
LPDIRECT3DTEXTURE9 lptex = NULL;
D3DMATERIAL9* lpmtrl = NULL;

BOOL jcd3d::jcd3d_setup()
{
	cube = new Cube(lpd3dd);
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	light.Direction = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	lpd3dd->SetLight(0, &light);
	lpd3dd->LightEnable(0, TRUE);

	lpmtrl = new D3DMATERIAL9();
	lpmtrl->Ambient = D3DXCOLOR(D3DCOLOR_XRGB(255, 255, 255));
	lpmtrl->Diffuse = D3DXCOLOR(D3DCOLOR_XRGB(255, 255, 255));
	lpmtrl->Specular = D3DXCOLOR(D3DCOLOR_XRGB(255, 255, 255));
	lpmtrl->Emissive = D3DXCOLOR(D3DCOLOR_XRGB(0, 0, 0));
	lpmtrl->Power = 2.0f;

	lpd3dd->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	//lpd3dd->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	//lpd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	D3DXCreateTextureFromFile(lpd3dd, L"cube.jpg", &lptex);

	lpd3dd->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	lpd3dd->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	lpd3dd->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveLH(
		&proj, D3DX_PI * 0.5f, (FLOAT)WIN_WIDTH / (FLOAT)WIN_HEIGHT, 1.0f, 1000.0f
	);
	lpd3dd->SetTransform(D3DTS_PROJECTION, &proj);

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(lpd3dd != NULL)
	{
		lpd3dd->Clear(0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0x00000000, 1.0f, 0);
		lpd3dd->BeginScene();

		static float angle  = (3.0f * D3DX_PI) / 2.0f;
		static float height = 2.0f;

		if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
			angle -= 0.5f * 0.05f;

		if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
			angle += 0.5f * 0.05f;

		if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
			height += 5.0f * 0.05f;

		if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
			height -= 5.0f * 0.05f;

		D3DXVECTOR3 position( cosf(angle) * 3.0f, height, sinf(angle) * 3.0f );
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);

		lpd3dd->SetTransform(D3DTS_VIEW, &V);

		cube->draw(NULL, lpmtrl, lptex);

		lpd3dd->EndScene();
		lpd3dd->Present(NULL, NULL, NULL, NULL);
	}
}

VOID jcd3d::jcd3d_release()
{
	jcd3d::jcd3d_releaseHandle(&lpd3dd);
	if(cube != NULL)
	{
		delete cube;
		cube = NULL;
	}
	if(lpmtrl != NULL)
	{
		delete lpmtrl;
		lpmtrl = NULL;
	}
}

HRESULT CALLBACK jcd3d::jcd3d_wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_DESTROY:
		{
			DestroyWindow(hwnd);
			break;
		}
	case WM_KEYDOWN:
		{
			if(wparam == VK_ESCAPE)
			{
				PostQuitMessage(0);
				break;
			}
		}
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d::jcd3d_init(hInstance, WIN_WIDTH, WIN_HEIGHT, TRUE, D3DDEVTYPE_HAL, &lpd3dd))
	{
		return 0;
	}

	jcd3d::jcd3d_loop(jcd3d::jcd3d_display);
	jcd3d::jcd3d_release();

	return 0;
}