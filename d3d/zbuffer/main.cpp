#include "jcd3d.h"

using namespace jcd3d;

const INT WIN_WIDTH = 800;
const INT WIN_HEIGHT = 600;

IDirect3DDevice9* lpd3dd = NULL;

IDirect3DVertexBuffer9* lprect1vb = NULL;
IDirect3DIndexBuffer9* lprect1ib = NULL;
IDirect3DVertexBuffer9* lprect2vb = NULL;
IDirect3DIndexBuffer9* lprect2ib = NULL;

struct Vertex
{
	FLOAT x, y, z;
	D3DCOLOR color;

	Vertex(FLOAT px, FLOAT py, FLOAT pz, D3DCOLOR pcolor)
	{
		x = px; y = py; z = pz;
		color = pcolor;
	}

	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

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

BOOL jcd3d::jcd3d_setup()
{
	// rect1
	lpd3dd->CreateVertexBuffer(4 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &lprect1vb, NULL);
	Vertex* vb1;
	lprect1vb->Lock(0, 0, (VOID**)&vb1, 0);
	vb1[0] = Vertex(-1.0f, 1.0f, 2.0f, 0xFF0000);
	vb1[1] = Vertex(1.0f, 1.0f, 2.0f, 0x00FF00);
	vb1[2] = Vertex(1.0f, -1.0f, 2.0f, 0x0000FF);
	vb1[3] = Vertex(-1.0f, -1.0f, 2.0f, 0xFFFFFF);
	lprect1vb->Unlock();

	lpd3dd->CreateIndexBuffer(6 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &lprect1ib, NULL);
	WORD* ib1;
	lprect1ib->Lock(0, 0, (VOID**)&ib1, 0);
	ib1[0] = 0; ib1[1] = 1; ib1[2] = 2;
	ib1[3] = 2; ib1[4] = 3; ib1[5] = 0;
	lprect1ib->Unlock();

	// rect2
	lpd3dd->CreateVertexBuffer(4 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &lprect2vb, NULL);
	Vertex* vb2;
	lprect2vb->Lock(0, 0, (VOID**)&vb2, 0);
	vb2[0] = Vertex(-2.5f, 2.5f, 3.0f, 0x0000FF);
	vb2[1] = Vertex(2.5f, 2.5f, 3.0f, 0xFFFFFF);
	vb2[2] = Vertex(2.5f, -2.5f, 3.0f, 0xFF0000);
	vb2[3] = Vertex(-2.5f, -2.5f, 3.0f, 0x00FF00);
	lprect2vb->Unlock();

	lpd3dd->CreateIndexBuffer(6 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &lprect2ib, NULL);
	WORD* ib2;
	lprect2ib->Lock(0, 0, (VOID**)&ib2, 0);
	ib2[0] = 0; ib2[1] = 1; ib2[2] = 2;
	ib2[3] = 2; ib2[4] = 3; ib2[5] = 0;
	lprect2ib->Unlock();

	// view transform
	D3DXMATRIX view;
	D3DXVECTOR3 eye(0.0f, 0.0f, -2.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&view, &eye, &target, &up);
	lpd3dd->SetTransform(D3DTS_VIEW, &view);
	
	// projection transform
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, (FLOAT)WIN_WIDTH / (FLOAT)WIN_HEIGHT, 1.0f, 1000.0f);
	lpd3dd->SetTransform(D3DTS_PROJECTION, &proj);

	lpd3dd->SetRenderState(D3DRS_ZENABLE, TRUE);
	lpd3dd->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	lpd3dd->SetRenderState(D3DRS_LIGHTING, FALSE);
	lpd3dd->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	lpd3dd->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(lpd3dd != NULL)
	{
		if(jcd3d_keyDown('A'))
		{
			lpd3dd->SetRenderState(D3DRS_ZENABLE, TRUE);
		}
		if(jcd3d_keyDown('S'))
		{
			lpd3dd->SetRenderState(D3DRS_ZENABLE, FALSE);
		}

		lpd3dd->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		lpd3dd->BeginScene();

		lpd3dd->SetStreamSource(0, lprect1vb, 0, sizeof(Vertex));
		lpd3dd->SetIndices(lprect1ib);
		lpd3dd->SetFVF(Vertex::FVF);
		lpd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		lpd3dd->SetStreamSource(0, lprect2vb, 0, sizeof(Vertex));
		lpd3dd->SetIndices(lprect2ib);
		lpd3dd->SetFVF(Vertex::FVF);
		lpd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		lpd3dd->EndScene();
		lpd3dd->Present(NULL, NULL, NULL, NULL);
	}
}

VOID jcd3d::jcd3d_release()
{
	jcd3d_releaseHandle(&lprect1vb);
	jcd3d_releaseHandle(&lprect1ib);
	jcd3d_releaseHandle(&lprect2vb);
	jcd3d_releaseHandle(&lprect2ib);
	jcd3d_releaseHandle(&lpd3dd);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d_init(hInstance, WIN_WIDTH, WIN_HEIGHT, TRUE, D3DDEVTYPE_HAL, &lpd3dd))
	{
		return 0;
	}

	jcd3d_loop(jcd3d_display);
	jcd3d_release();

	return 0;
}