#include "jcd3d.h"

const INT WIN_WIDTH = 800;
const INT WIN_HEIGHT = 600;

LPDIRECT3DDEVICE9 lpd3dd;
LPDIRECT3DVERTEXBUFFER9 lpquadvb;
LPDIRECT3DTEXTURE9 lpquadtx;

struct Vertex
{
	FLOAT x, y, z;
	FLOAT nx, ny, nz;
	FLOAT u, v;

	Vertex(FLOAT px, FLOAT py, FLOAT pz, FLOAT pnx, FLOAT pny, FLOAT pnz, FLOAT pu, FLOAT pv)
	{
		x = px;
		y = py;
		z = pz;
		nx = pnx;
		ny = pny;
		nz = pnz;
		u = pu;
		v = pv;
	}

	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

BOOL jcd3d::jcd3d_setup()
{
	if(FAILED(lpd3dd->CreateVertexBuffer(
		6 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &lpquadvb, NULL
	)))
	{
		return FALSE;
	}

	Vertex* v;
	if(FAILED(lpquadvb->Lock(0, 0, (VOID**)&v, 0)))
	{
		return FALSE;
	}

	v[0] = Vertex(-1.0f, -1.0f, 2.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex(-1.0f,  1.0f, 2.25f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex( 1.0f,  1.0f, 2.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	
	v[3] = Vertex(-1.0f, -1.0f, 2.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = Vertex( 1.0f,  1.0f, 2.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = Vertex( 1.0f, -1.0f, 2.25f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	if(FAILED(lpquadvb->Unlock()))
	{
		return FALSE;
	}

	if(FAILED(D3DXCreateTextureFromFile(
		lpd3dd, L"dx5_logo.bmp", &lpquadtx
	)))
	{
		return FALSE;
	}
	if(FAILED(lpd3dd->SetTexture(0, lpquadtx)))
	{
		return FALSE;
	}

	if(FAILED(lpd3dd->SetRenderState(D3DRS_ZENABLE, TRUE)))
	{
		return FALSE;
	}
	if(FAILED(lpd3dd->SetRenderState(D3DRS_LIGHTING, FALSE)))
	{
		return FALSE;
	}

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * .5f, (FLOAT)WIN_WIDTH / (FLOAT)WIN_HEIGHT, 1.0f, 1000.0f);
	if(FAILED(lpd3dd->SetTransform(D3DTS_PROJECTION, &proj)))
	{
		return FALSE;
	}

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(lpd3dd)
	{
		lpd3dd->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		lpd3dd->BeginScene();

		lpd3dd->SetStreamSource(0, lpquadvb, 0, sizeof(Vertex));
		lpd3dd->SetFVF(Vertex::FVF);
		lpd3dd->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		lpd3dd->EndScene();
		lpd3dd->Present(NULL, NULL, NULL, NULL);
	}
}

VOID jcd3d::jcd3d_release()
{
	jcd3d::jcd3d_releaseHandle(&lpquadtx);
	jcd3d::jcd3d_releaseHandle(&lpquadvb);
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
	if(!jcd3d::jcd3d_init(hInstance, WIN_WIDTH, WIN_HEIGHT, TRUE, D3DDEVTYPE_HAL, & lpd3dd))
	{
		return 0;
	}

	jcd3d::jcd3d_loop(jcd3d::jcd3d_display);
	jcd3d::jcd3d_release();

	return 0;
}