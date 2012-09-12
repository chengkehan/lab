#include <cstdio>
#include "jcd3d.h"

const INT WND_WIDTH = 800;
const INT WND_HEIGHT = 600;

LPDIRECT3DDEVICE9 lpd3dd;
LPDIRECT3DVERTEXBUFFER9 lpd3dvb;

struct JCVertex
{
	JCVertex(float px, float py, float pz, D3DCOLOR pColor)
	{
		x = px;
		y = py;
		z = pz;
		color = pColor;
	}

	float x, y, z;
	D3DCOLOR color;

	static const DWORD fvf;
};
const DWORD JCVertex::fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;

BOOL jcd3d::jcd3d_setup()
{
	HRESULT hVbResult = lpd3dd->CreateVertexBuffer(
		3 * sizeof(JCVertex), D3DUSAGE_WRITEONLY, JCVertex::fvf, D3DPOOL_MANAGED, &lpd3dvb, NULL
		);
	if(FAILED(hVbResult))
	{
		return FALSE;
	}

	JCVertex* ptr;
	if(FAILED(lpd3dvb->Lock(0, 0, (VOID**)&ptr, 0)))
	{
		return FALSE;
	}

	ptr[0] = JCVertex(-1.0f, 0.0f, 2.0f, 0xFF0000);
	ptr[1] = JCVertex(0.0f, 1.0f, 2.0f, 0x00FF00);
	ptr[2] = JCVertex(1.0f, 0.0f, 2.0f, 0x0000FF);

	if(FAILED(lpd3dvb->Unlock()))
	{
		return FALSE;
	}

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * .5, (FLOAT)WND_WIDTH / (FLOAT)WND_HEIGHT, 1.0f, 1000.0f);
	if(lpd3dd->SetTransform(D3DTS_PROJECTION, &proj))
	{
		return FALSE;
	}

	if(FAILED(lpd3dd->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME)))
	{
		return FALSE;
	}
	if(FAILED(lpd3dd->SetRenderState(D3DRS_LIGHTING, FALSE)))
	{
		return FALSE;
	}

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(lpd3dd)
	{
		lpd3dd->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x000000, 1.0f, 0);
		lpd3dd->BeginScene();

		lpd3dd->SetStreamSource(0, lpd3dvb, 0, sizeof(JCVertex));
		lpd3dd->SetFVF(JCVertex::fvf);

		lpd3dd->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		lpd3dd->EndScene();
		lpd3dd->Present(NULL, NULL, NULL, NULL);
	}
}

VOID jcd3d::jcd3d_release()
{
	jcd3d::jcd3d_releaseHandle(&lpd3dvb);
	jcd3d::jcd3d_releaseHandle(&lpd3dd);
}

LRESULT CALLBACK jcd3d::jcd3d_wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
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
	if(!jcd3d::jcd3d_init(hInstance, WND_WIDTH, WND_HEIGHT, true, D3DDEVTYPE_HAL, &lpd3dd))
	{
		return 0;
	}

	jcd3d::jcd3d_loop(jcd3d::jcd3d_display);

	jcd3d::jcd3d_release();

	return 0;
}